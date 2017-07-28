/* UsbCamLib.cpp Ver1.11 */
/* ※　iPokr制御用C++クラスのソースファイルです */

#include "StdAfx.h"
#include "UsbCamLib.h"


UsbCamLib::UsbCamLib(void)
{
}


UsbCamLib::~UsbCamLib(void)
{
}


int UsbCamLib::Init(void)
{
//バッファクリア
	ZeroMemory(&mUsbCam,  sizeof(UCF_TAG));
	ZeroMemory(&mCamInfo, sizeof(UCI_TAG));

	pSampleGrabberCB = NULL;
	mConnectCamNo = -1;
	mMiconVer = -1;

//COMライブラリの初期化
	HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if(FAILED(hr))
		return UCLIB_ERR;

	return UCLIB_OK;
}


int UsbCamLib::SetParam(PVOID pBuf1, PVOID pBuf2, void *hFuncAddress, void *hProcAddress)
{
	mpBuf  = pBuf1;		//ビデオキャプチャ用設定
	mpCBuf = pBuf2;		//補正データ保存用設定

	mhFuncAdrs = hFuncAddress;	//コールバック関数アドレス
	mhProcAdrs = hProcAddress;	//コールバック関数パラメータ

	mSizeX = UCLIB_SIZE_X_DEFAULT;
	mSizeY = UCLIB_SIZE_Y_DEFAULT;
	mOffsetX = UCLIB_OFFSET_X_DEFAULT;
	mOffsetY = UCLIB_OFFSET_Y_DEFAULT;

	return UCLIB_OK;
}


int UsbCamLib::Open(int CameraNo, int flg)
{
	HRESULT hr;
	ICreateDevEnum	*lpDevEnum;
	IEnumMoniker	*lpEnum;
	IMoniker		*lpMoniker;

//範囲のチェック
	if((CameraNo < 0) || (CameraNo >= UCLIB_CAMERA_MAX))
		return UCLIB_ERR;

//フィルタグラフマネージャ作成 pGraph

	hr = CoCreateInstance(CLSID_FilterGraph, 0 ,CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void **)&mUsbCam.pGraph);
	if(FAILED(hr))
		return UCLIB_ERR;

//システムデバイス列挙子の作成
	hr = CoCreateInstance(CLSID_SystemDeviceEnum, 0 ,CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, (void **)&lpDevEnum);
	if(FAILED(hr))
		return UCLIB_ERR;

//列挙子の取得
	hr = lpDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &lpEnum, 0);
	if(FAILED(hr))
		return UCLIB_ERR;

//モニカの取得
	unsigned long cFetched;
	wchar_t SrcName[32];
	int i, lDetectFlg;

	lDetectFlg = 0;
	mCamNum = 0;
	for(i = 0; i < UCLIB_CAMERA_MAX; i++){
		if(lpEnum->Next(1, &lpMoniker, &cFetched) == S_OK){
			
			//DisplayNameの取得
			LPOLESTR strMonikerName = 0;
			hr = lpMoniker->GetDisplayName(NULL, NULL, &strMonikerName);
			if(FAILED(hr))
				return UCLIB_ERR;

			//DisplayNameにiPokrを見つけて登録
			if(wcsstr(strMonikerName, L"@device:pnp") && wcsstr(strMonikerName, L"vid_04b4&pid_00f2")){

				//オブジェクト初期化 pCap
				lpMoniker->BindToObject(0, 0, IID_IBaseFilter, (void **)&mUsbCam.pCap[mCamNum]);
				mUsbCam.pCap[mCamNum]->SetSyncSource(NULL);
				mCamInfo[mCamNum].isExist = TRUE;

				if(mCamNum == CameraNo){
					//グラフにフィルタを追加
					swprintf_s(SrcName,32,L"Video Capture %d", mCamNum);
					hr = mUsbCam.pGraph->AddFilter(mUsbCam.pCap[mCamNum], SrcName);
					if(FAILED(hr))
						return UCLIB_ERR;
					lDetectFlg = 1;

				}

				mCamNum ++;
			}
			lpMoniker->Release();
		}
	}
	lpEnum->Release();
	lpDevEnum->Release();

//ターゲットのカメラが見つからなかった
	if(!lDetectFlg)
		return UCLIB_ERR;

//サンプルグラバの生成 pF,pGrab
	hr = CoCreateInstance(CLSID_SampleGrabber, 0, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (LPVOID *)&mUsbCam.pF);
	if(FAILED(hr))
		return UCLIB_ERR;
	hr = mUsbCam.pF->QueryInterface(IID_ISampleGrabber, (void **)&mUsbCam.pGrab);
	if(FAILED(hr))
		return UCLIB_ERR;

//フィルタグラフへの追加
	wchar_t GrabName[32];
	swprintf_s(GrabName,32, L"Grabber");
	hr = mUsbCam.pGraph->AddFilter(mUsbCam.pF, GrabName);
	if(FAILED(hr))
		return UCLIB_ERR;

//サンプルグラバの接続
	IPin *lpSrc;
	IPin *lpDst;

	// ピンの取得
	lpSrc = _GetPin(mUsbCam.pCap[CameraNo], PINDIR_OUTPUT);
	lpDst = _GetPin(mUsbCam.pF, PINDIR_INPUT);

// ピンの接続
	hr = mUsbCam.pGraph->Connect(lpSrc, lpDst);
	if(FAILED(hr))
		return UCLIB_ERR;
	lpSrc->Release();
	lpDst->Release();

//グラバのモード設定
	hr = mUsbCam.pGrab->SetBufferSamples(FALSE);
	if(FAILED(hr))
		return UCLIB_ERR;
	hr = mUsbCam.pGrab->SetOneShot(FALSE);
	if(FAILED(hr))
		return UCLIB_ERR;
//コールバック関数の登録
	pSampleGrabberCB = new SampleGrabberCB();
	hr = mUsbCam.pGrab->SetCallback(pSampleGrabberCB, 1);
	if(FAILED(hr))
		return UCLIB_ERR;


//画像取り込みバッファ設定
	char state = 1;
	int count = 1500;
	pSampleGrabberCB->SetPtr(mpBuf, mpCBuf, &state, mhFuncAdrs, mhProcAdrs);
	pSampleGrabberCB->SetROI(mSizeX, mSizeY, mOffsetX, mOffsetY);


/** Ver1.03b **/
	//ファームウェアバーションチェック
	GetMiconVersion(CameraNo);


//補正データをロードしない
	if(flg == UCLIB_CAPTUREMODE_THROUGH)
		return UCLIB_OK;

//トリガーモード確認
	int trg = GetVal(CameraNo, 4);
	if(trg != 0)
		SetVal(CameraNo, 4, 0);


//補正データ取り込み処理
	//転送スタート
	Run(CameraNo);

	//モード変更(補正データ送出)
	SpiCtl(CameraNo, 88);

	//補正データ取り込み待ち
	while(state){
		Sleep(5);
		count --;
		if(!count)
			return UCLIB_ERR_CORRECT_DATA;
	}

	//転送ストップ
	Stop(CameraNo);

//トリガーモードを復元
	if(trg != 0)
		SetVal(CameraNo, 4, trg);

	return UCLIB_OK;

}


int UsbCamLib::Close(int CameraNo)
{
	mConnectCamNo = -1;

//メモリ解放
	if(mUsbCam.pGraph){
		mUsbCam.pGraph->Release();
		mUsbCam.pGraph = NULL;
	}

	if(mUsbCam.pF){
		mUsbCam.pF->Release();
		mUsbCam.pF = NULL;
	}

	for(int i = 0; i < UCLIB_CAMERA_MAX; i ++){
		if(mUsbCam.pCap[i]){
			mUsbCam.pCap[i]->Release();
			mUsbCam.pCap[i] = NULL;
		}
	}

	if(mUsbCam.pGrab){
		mUsbCam.pGrab->Release();
		mUsbCam.pGrab = NULL;
	}

	if(pSampleGrabberCB)
		delete pSampleGrabberCB;

//COMライブラリを閉じる
	CoUninitialize();

	return UCLIB_OK;
}


//キャプチャ開始
int UsbCamLib::Run(int CameraNo)
{
	HRESULT hr;
	IMediaControl *lpMediaControl;


	hr = mUsbCam.pGraph->QueryInterface(IID_IMediaControl, (void **)&lpMediaControl);
	if(FAILED(hr))
		return UCLIB_ERR;

	lpMediaControl->Run();
	lpMediaControl->Release();

	return UCLIB_OK;
}


//キャプチャ停止
int UsbCamLib::Stop(int CameraNo)
{
	HRESULT hr;
	IMediaControl *lpMediaControl;

	hr = mUsbCam.pGraph->QueryInterface(IID_IMediaControl, (void **)&lpMediaControl);
	if(FAILED(hr))
		return UCLIB_ERR;

	lpMediaControl->Stop();
	lpMediaControl->Release();

	return UCLIB_OK;
}

//キャプチャ一時停止
int UsbCamLib::Pause(int CameraNo)
{
	HRESULT hr;
	IMediaControl *lpMediaControl;

	hr = mUsbCam.pGraph->QueryInterface(IID_IMediaControl, (void **)&lpMediaControl);
	if(FAILED(hr))
		return UCLIB_ERR;

	lpMediaControl->Pause();
	lpMediaControl->Release();

	return UCLIB_OK;
}


//カメラレジスタセット
int UsbCamLib::SetVal(int CameraNo, int Adrs, int Data)
{
	IAMVideoProcAmp *lpVPAmp;

	HRESULT hr = mUsbCam.pCap[CameraNo]->QueryInterface(IID_IAMVideoProcAmp,(void **)&lpVPAmp);
	if(FAILED(hr))
		return UCLIB_ERR;

	lpVPAmp->Set(UCLIB_CONTROL_CODE_ADR, Adrs, NULL);
	lpVPAmp->Set(UCLIB_CONTROL_CODE_DAT, Data, NULL);


	/** Ver1.03b **/
	Patch000(lpVPAmp, Adrs);


	lpVPAmp->Release();

	return UCLIB_OK;
}


//カメラレジスタゲット
int UsbCamLib::GetVal(int CameraNo, int Adrs)
{
	IAMVideoProcAmp *lpVPAmp;
	long Val, Flags;

	HRESULT hr = mUsbCam.pCap[CameraNo]->QueryInterface(IID_IAMVideoProcAmp,(void **)&lpVPAmp);
	if(FAILED(hr))
		return UCLIB_ERR;

	lpVPAmp->Set(UCLIB_CONTROL_CODE_ADR, Adrs, NULL);
	lpVPAmp->Get(UCLIB_CONTROL_CODE_DAT, &Val, &Flags);
	lpVPAmp->Release();

	return (int)Val;
}


//カメラ特殊設定
int UsbCamLib::SpiCtl(int CameraNo, int Data)
{
	IAMVideoProcAmp *lpVPAmp;

	HRESULT hr = mUsbCam.pCap[CameraNo]->QueryInterface(IID_IAMVideoProcAmp,(void **)&lpVPAmp);
	if(FAILED(hr))
		return UCLIB_ERR;

	lpVPAmp->Set(UCLIB_CONTROL_CODE_EXT, Data, NULL);
	lpVPAmp->Release();

	return UCLIB_OK;
}


//フィルタのピンを取得する
IPin *UsbCamLib::_GetPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir)
{
	IEnumPins *pEnum;
	IPin *pPin = NULL;

	if(pFilter->EnumPins(&pEnum) != S_OK)
		return NULL;

	while(pEnum->Next(1, &pPin, 0) == S_OK){
		PIN_DIRECTION PinDirThis;
		pPin->QueryDirection(&PinDirThis);

		if(PinDir == PinDirThis)
			break;
	}

	return pPin;
}




/*****************************/
/*** 以下、Ver1.02拡張関数 ***/
/*****************************/



int UsbCamLib::InitLibEx(void)
{
//バッファクリア
	ZeroMemory(&mUsbCam,  sizeof(UCF_TAG));
	ZeroMemory(&mCamInfo, sizeof(UCI_TAG));

	pSampleGrabberCB = NULL;
	mConnectCamNo = -1;
	mMiconVer = -1;

//COMライブラリの初期化
	HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if(FAILED(hr))
		return UCLIB_ERR;

	return UCLIB_OK;
}


int UsbCamLib::UninitLibEx(void)
{


//COMライブラリを閉じる
	CoUninitialize();

	return UCLIB_OK;
}


int UsbCamLib::SetParamEx(PVOID pBuf1, PVOID pBuf2, void *hFuncAddress, void *hProcAddress, int SizeX, int SizeY, int OffsetX, int OffsetY)
{
	mpBuf  = pBuf1;		//ビデオキャプチャ用設定
	mpCBuf = pBuf2;		//補正データ保存用設定

	mhFuncAdrs = hFuncAddress;	//コールバック関数アドレス
	mhProcAdrs = hProcAddress;	//コールバック関数パラメータ


	//設定値は1以上が有効
	mSizeX = SizeX;
	if(mSizeX > UCLIB_SIZE_X_MAX)
		mSizeX = UCLIB_SIZE_X_MAX;
	if(mSizeX < UCLIB_SIZE_X_MIN)
		mSizeX = UCLIB_SIZE_X_MIN;

	mSizeY = SizeY;
	if(mSizeY > UCLIB_SIZE_Y_MAX)
		mSizeY = UCLIB_SIZE_Y_MAX;
	if(mSizeY < UCLIB_SIZE_Y_MIN)
		mSizeY = UCLIB_SIZE_Y_MIN;

	//設定値に不整合があった場合、オフセット優先
	mOffsetX = OffsetX;
	if(mOffsetX < UCLIB_OFFSET_X_MIN)
		mOffsetX = UCLIB_OFFSET_X_MIN;
	if((mSizeX + mOffsetX) > UCLIB_SIZE_X_MAX)
		mSizeX = UCLIB_SIZE_X_MAX - mOffsetX;

	mOffsetY = OffsetY;
	if(mOffsetY < UCLIB_OFFSET_Y_MIN)
		mOffsetY = UCLIB_OFFSET_Y_MIN;
	if((mSizeY + mOffsetY) > UCLIB_SIZE_Y_MAX)
		mSizeY = UCLIB_SIZE_Y_MAX - mOffsetY;


	//補正データ用バッファをクリアしておく
	int xy = mSizeX * mSizeY;
	ZeroMemory(mpCBuf, xy);


	return UCLIB_OK;
}


int UsbCamLib::DetectCamEx()
{
	HRESULT hr;
	ICreateDevEnum	*lpDevEnum;
	IEnumMoniker	*lpEnum;
	IMoniker		*lpMoniker;

//Grab中なら、ERR応答(UCLIB_ERR_LIB_ISBUSY)

	CloseCamEx();

//フィルタグラフマネージャ作成 pGraph
	hr = CoCreateInstance(CLSID_FilterGraph, 0 ,CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void **)&mUsbCam.pGraph);
	if(FAILED(hr))
		return UCLIB_ERR;

//システムデバイス列挙子の作成
	hr = CoCreateInstance(CLSID_SystemDeviceEnum, 0 ,CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, (void **)&lpDevEnum);
	if(FAILED(hr))
		return UCLIB_ERR;

//列挙子の取得
	hr = lpDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &lpEnum, 0);
	if(FAILED(hr))
		return UCLIB_ERR;

//モニカの取得
	IAMVideoProcAmp *lpVPAmp;
	unsigned long cFetched;
	long Val, Flags;
	int i, j, retry;

	mCamNum = 0;
	for(i = 0; i < UCLIB_CAMERA_MAX; i++){
		if(lpEnum->Next(1, &lpMoniker, &cFetched) == S_OK){
			
			//DisplayNameの取得
			LPOLESTR strMonikerName = 0;
			hr = lpMoniker->GetDisplayName(NULL, NULL, &strMonikerName);
			if(FAILED(hr))
				return UCLIB_ERR;

			//DisplayNameにiPokrを見つけて登録
			if(wcsstr(strMonikerName, L"@device:pnp") && wcsstr(strMonikerName, L"vid_04b4&pid_00f2")){

				//オブジェクト初期化 pCap
				lpMoniker->BindToObject(0, 0, IID_IBaseFilter, (void **)&mUsbCam.pCap[mCamNum]);
				mUsbCam.pCap[mCamNum]->SetSyncSource(NULL);
				mCamInfo[mCamNum].isExist = TRUE;

				//カメラ情報取得
				mUsbCam.pCap[mCamNum]->QueryInterface(IID_IAMVideoProcAmp,(void **)&lpVPAmp);
				for(j = 0; j < 16; j ++){
					//最初のpVPAmp制御が成功するまでリトライする
					hr = lpVPAmp->Set(UCLIB_CONTROL_CODE_ADR, 816 + j, NULL);
					if(hr != NO_ERROR){
						retry = 50;
						while(retry){
							if(lpVPAmp->Set(UCLIB_CONTROL_CODE_ADR, 816 + j, NULL) == NO_ERROR)
								break;
							Sleep(20);
							retry --;
						}
					}
					lpVPAmp->Get(UCLIB_CONTROL_CODE_DAT, &Val, &Flags);
					mCamInfo[mCamNum].id[j] = (char)(Val & 0xff);
				}

				lpVPAmp->Set(UCLIB_CONTROL_CODE_ADR, 190, NULL);
				lpVPAmp->Get(UCLIB_CONTROL_CODE_DAT, &Val, &Flags);
				mCamInfo[mCamNum].isColorModel = (BOOL)(Val & 0x1);

				lpVPAmp->Get(UCLIB_CONTROL_CODE_EXT, &Val, &Flags);
				mCamInfo[mCamNum].isBusy = (BOOL)(Val >> 8) & 0x1;

				lpVPAmp->Release();

				mCamNum ++;
			}
			lpMoniker->Release();

		}
	}
	lpEnum->Release();
	lpDevEnum->Release();

	return mCamNum;
}


int UsbCamLib::OpenCamEx(int CameraNo, int flg)
{
//CameraNoチェック
	//-1なら一番最初のアイドルカメラに接続する
	if(CameraNo == -1){
		int i;
		for(i = 0; i < UCLIB_CAMERA_MAX; i++){
			if(mCamInfo[i].isExist != TRUE)
				continue;
			if(GetInfoIsBusy(i) == FALSE)
				break;
		}
		if(i == UCLIB_CAMERA_MAX)
			return UCLIB_ERR_CAM_NOT_EXIST;
		CameraNo = i;
	}

//Detect済みかチェック
	if(!mCamInfo[CameraNo].isExist)
		return UCLIB_ERR_CAM_NOT_EXIST;

//グラフにフィルタを追加
	HRESULT hr;
	wchar_t SrcName[32];
	swprintf_s(SrcName,32,L"Video Capture %d", CameraNo);
	hr = mUsbCam.pGraph->AddFilter(mUsbCam.pCap[CameraNo], SrcName);
	if(FAILED(hr))
		return UCLIB_ERR;

//サンプルグラバの生成 pF,pGrab
	hr = CoCreateInstance(CLSID_SampleGrabber, 0, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (LPVOID *)&mUsbCam.pF);
	if(FAILED(hr))
		return UCLIB_ERR;
	hr = mUsbCam.pF->QueryInterface(IID_ISampleGrabber, (void **)&mUsbCam.pGrab);
	if(FAILED(hr))
		return UCLIB_ERR;

//フィルタグラフへの追加
	wchar_t GrabName[32];
	swprintf_s(GrabName,32, L"Grabber");
	hr = mUsbCam.pGraph->AddFilter(mUsbCam.pF, GrabName);
	if(FAILED(hr))
		return UCLIB_ERR;

//サンプルグラバの接続
	IPin *lpSrc;
	IPin *lpDst;

	// ピンの取得
	lpSrc = _GetPin(mUsbCam.pCap[CameraNo], PINDIR_OUTPUT);
	lpDst = _GetPin(mUsbCam.pF, PINDIR_INPUT);

// ピンの接続
	hr = mUsbCam.pGraph->Connect(lpSrc, lpDst);
	if(FAILED(hr))
		return UCLIB_ERR;
	lpSrc->Release();
	lpDst->Release();

//グラバのモード設定
	hr = mUsbCam.pGrab->SetBufferSamples(FALSE);
	if(FAILED(hr))
		return UCLIB_ERR;
	hr = mUsbCam.pGrab->SetOneShot(FALSE);
	if(FAILED(hr))
		return UCLIB_ERR;
//コールバック関数の登録
	pSampleGrabberCB = new SampleGrabberCB();
	hr = mUsbCam.pGrab->SetCallback(pSampleGrabberCB, 1);
	if(FAILED(hr))
		return UCLIB_ERR;


//画像取り込みバッファ設定
	char state = 1;
	int count = 1500;
	pSampleGrabberCB->SetPtr(mpBuf, mpCBuf, &state, mhFuncAdrs, mhProcAdrs);
	pSampleGrabberCB->SetROI(mSizeX, mSizeY, mOffsetX, mOffsetY);


/** Ver1.03b **/
	//ファームウェアバーションチェック
	GetMiconVersion(CameraNo);


//補正データをロードしない
	if(flg == UCLIB_CAPTUREMODE_THROUGH){
		mConnectCamNo = CameraNo;
		return UCLIB_OK;
	}

//トリガーモード確認
	int trg = GetVal(CameraNo, 4);
	if(trg != 0)
		SetVal(CameraNo, 4, 0);

//補正データ取り込み処理
	//転送スタート
	Run(CameraNo);

	//モード変更(補正データ送出)
	SpiCtl(CameraNo, 88);

	//補正データ取り込み待ち
	while(state){
		Sleep(5);
		count --;
		if(!count)
			return UCLIB_ERR_CORRECT_DATA;
	}

	//転送ストップ
	Stop(CameraNo);

	//トリガーモードを復元
	if(trg != 0)
		SetVal(CameraNo, 4, trg);

	mConnectCamNo = CameraNo;
	return UCLIB_OK;
}


int UsbCamLib::CloseCamEx()
{
	mConnectCamNo = -1;


//メモリ解放
	if(mUsbCam.pGraph){
		mUsbCam.pGraph->Release();
		mUsbCam.pGraph = NULL;
	}

	if(mUsbCam.pF){
		mUsbCam.pF->Release();
		mUsbCam.pF = NULL;
	}

	for(int i = 0; i < UCLIB_CAMERA_MAX; i ++){
		if(mUsbCam.pCap[i]){
			mUsbCam.pCap[i]->Release();
			mUsbCam.pCap[i] = NULL;
		}
	}

	if(mUsbCam.pGrab){
		mUsbCam.pGrab->Release();
		mUsbCam.pGrab = NULL;
	}


	if(pSampleGrabberCB)
		delete pSampleGrabberCB;

	return UCLIB_OK;
}


/* カメラ切り替え */
int UsbCamLib::SwitchCamEx(int CameraNo, int flg)
{
	if((mConnectCamNo < 0) || (mConnectCamNo == CameraNo))
		return UCLIB_ERR;

	if(!mCamInfo[CameraNo].isExist)
		return UCLIB_ERR_CAM_NOT_EXIST;

	IPin *lpSrc;
	IPin *lpDst;

	lpSrc = _GetPin(mUsbCam.pCap[mConnectCamNo], PINDIR_OUTPUT);
	lpDst = _GetPin(mUsbCam.pF,      PINDIR_INPUT);
	if(mUsbCam.pGraph->Disconnect(lpSrc) != S_OK)
		return UCLIB_ERR;
	if(mUsbCam.pGraph->Disconnect(lpDst) != S_OK)
		return UCLIB_ERR;
	lpSrc->Release();

	mUsbCam.pGraph->RemoveFilter(mUsbCam.pCap[mConnectCamNo]);
	mConnectCamNo = CameraNo;

	HRESULT hr;
	wchar_t SrcName[32];
	swprintf_s(SrcName, 32, L"Video Capture %d", mConnectCamNo);
	hr = mUsbCam.pGraph->AddFilter(mUsbCam.pCap[mConnectCamNo], SrcName);
	if(FAILED(hr))
		return UCLIB_ERR;

	lpSrc = _GetPin(mUsbCam.pCap[mConnectCamNo], PINDIR_OUTPUT);
	hr = mUsbCam.pGraph->Connect(lpSrc, lpDst);
	if(FAILED(hr))
		return UCLIB_ERR;
	lpSrc->Release();
	lpDst->Release();


//画像取り込みバッファ設定
	char state = 1;
	int count = 1500;
	pSampleGrabberCB->SetPtr(mpBuf, mpCBuf, &state, mhFuncAdrs, mhProcAdrs);


/** Ver1.03b **/
	//ファームウェアバーションチェック
	GetMiconVersion(CameraNo);


//補正データをロードしない
	if(flg == UCLIB_CAPTUREMODE_THROUGH){
		//補正データ用バッファをクリアしておく
		int xy = mSizeX * mSizeY;
		ZeroMemory(mpCBuf, xy);

		return UCLIB_OK;
	}

//トリガーモード確認
	int trg = GetVal(CameraNo, 4);
	if(trg != 0)
		SetVal(CameraNo, 4, 0);

//補正データ取り込み処理
	//転送スタート
	Run(CameraNo);

	//モード変更(補正データ送出)
	SpiCtl(CameraNo, 88);

	//補正データ取り込み待ち
	while(state){
		Sleep(5);
		count --;
		if(!count)
			return UCLIB_ERR_CORRECT_DATA;
	}

	//転送ストップ
	Stop(CameraNo);

//トリガーモードを復元
	if(trg != 0)
		SetVal(CameraNo, 4, trg);


	return UCLIB_OK;
}



/* スナップ取り込み */
/* Ver1.04で引数修正、機能実装 */
/* 画像取り込み完了の検出と待ち時間は簡易的なもの*/
/* リアルタイム性が必要な場合は、ハードトリガーとコールバック関数を使用した方がよい */
int UsbCamLib::Snap(int CameraNo, int timeout)
{
	/* timeoutの値が1以上の時、関数内でキャプチャ完了まで待機(単位はおよそms) */
	/* 0以下の場合、キャプチャ完了を待たずに抜ける */

	HRESULT hr;
	IMediaControl *lpMediaControl;

	hr = mUsbCam.pGraph->QueryInterface(IID_IMediaControl, (void **)&lpMediaControl);
	if(FAILED(hr))
		return UCLIB_ERR;

	//StopWhenReady
	lpMediaControl->StopWhenReady();

	//Set SnapMode
	char state = 1;
	int count = timeout;
	pSampleGrabberCB->SetSnap(&state);

	//run
	lpMediaControl->Run();

	//取り込み待ち
	if(timeout > 0){
		while(state){
			Sleep(1);
			count --;
			if(!count){
				lpMediaControl->StopWhenReady();
				lpMediaControl->Release();
				return UCLIB_ERR_TIMEOUT;
			}
		}
		//Stop
		lpMediaControl->Stop();
	}

	lpMediaControl->Release();
	return UCLIB_OK;

}



/* カメラNo.取得 */
int UsbCamLib::GetInfoCamNo(char *CameraID)
{
	int i, no;

	no = -1;
	for(i = 0; i < UCLIB_CAMERA_MAX; i ++){

		if(memcmp(mCamInfo[i].id, CameraID, 16) == 0){
			no = i;
			break;
		}
	}

	return no;
}


/* 接続中のカメラNo.取得 */
int UsbCamLib::GetInfoConnectCamNo()
{
	return mConnectCamNo;
}


//Busy状態確認
BOOL UsbCamLib::GetInfoIsBusy(int CameraNo)
{
	IAMVideoProcAmp *lpVPAmp;
	long Val, Flags;

	HRESULT hr = mUsbCam.pCap[CameraNo]->QueryInterface(IID_IAMVideoProcAmp,(void **)&lpVPAmp);
	if(FAILED(hr))
		return TRUE;

	lpVPAmp->Get(UCLIB_CONTROL_CODE_EXT, &Val, &Flags);
	lpVPAmp->Release();


	mCamInfo[CameraNo].isBusy = (BOOL)(Val >> 8) & 0x1;

	return mCamInfo[CameraNo].isBusy;
}


//カメラ検出確認
BOOL UsbCamLib::GetInfoIsExist(int CameraNo)
{
	return mCamInfo[CameraNo].isExist;
}


//カラーモデル確認
BOOL UsbCamLib::GetInfoIsColorModel(int CameraNo)
{
	return mCamInfo[CameraNo].isColorModel;
}


//USBバージョン確認
//正常な値を得るには、USBコントローラのファームウェアが
//Ver.004以上である必要があります。
int UsbCamLib::GetInfoUSBVersion(int CameraNo)
{
	IAMVideoProcAmp *lpVPAmp;
	long Val, Flags;

	HRESULT hr = mUsbCam.pCap[CameraNo]->QueryInterface(IID_IAMVideoProcAmp,(void **)&lpVPAmp);
	if(FAILED(hr))
		return TRUE;

	lpVPAmp->Get(UCLIB_CONTROL_CODE_EXT, &Val, &Flags);
	lpVPAmp->Release();


	mCamInfo[CameraNo].USBVer = (Val >> 12) & 0xf;

	return mCamInfo[CameraNo].USBVer;
}



/******************************/
/*** 以下、Ver1.03b追加関数 ***/
/******************************/



void UsbCamLib::GetMiconVersion(int CameraNo)
{
	mMiconVer = GetVal(CameraNo, UCLIB_ADRS_MICON_VER);
	if(mMiconVer == 2){
		SetVal(CameraNo, 259, 224);
		SetVal(CameraNo, 260, 208);
		SetVal(CameraNo, 261,   8);
	}
}

void UsbCamLib::Patch000(IAMVideoProcAmp *lpVPAmp, int Adrs)
{
	long Flags;
	long sht, trg, pre, itg;
	long time;


	if(mMiconVer != 2)
		return;

	/* ShutterSpeedに関わる設定変更あり */
	if((Adrs == UCLIB_ADRS_SHT) || (Adrs == UCLIB_ADRS_TRG) || (Adrs == UCLIB_ADRS_PRE) || (Adrs == UCLIB_ADRS_ITG)){

		lpVPAmp->Set(UCLIB_CONTROL_CODE_ADR, UCLIB_ADRS_SHT, NULL);
		lpVPAmp->Get(UCLIB_CONTROL_CODE_DAT, &sht, &Flags);
		lpVPAmp->Set(UCLIB_CONTROL_CODE_ADR, UCLIB_ADRS_TRG, NULL);
		lpVPAmp->Get(UCLIB_CONTROL_CODE_DAT, &trg, &Flags);
		lpVPAmp->Set(UCLIB_CONTROL_CODE_ADR, UCLIB_ADRS_PRE, NULL);
		lpVPAmp->Get(UCLIB_CONTROL_CODE_DAT, &pre, &Flags);
		lpVPAmp->Set(UCLIB_CONTROL_CODE_ADR, UCLIB_ADRS_ITG, NULL);
		lpVPAmp->Get(UCLIB_CONTROL_CODE_DAT, &itg, &Flags);

		/* 且つ、高速シャッターでフリーラン状態 */
		if(trg == 0){
			time = (pre * 2 + 2) * (itg * 2 + 1);
			if(((4 < sht) && (sht < 15)) || ((sht == 15) && (time < 2200))){
				/* FPS制限発動 */
				lpVPAmp->Set(UCLIB_CONTROL_CODE_ADR, UCLIB_ADRS_TRG_FPGA, NULL);
				lpVPAmp->Set(UCLIB_CONTROL_CODE_DAT, 3, NULL);
			} else {
				lpVPAmp->Set(UCLIB_CONTROL_CODE_ADR, UCLIB_ADRS_TRG_FPGA, NULL);
				lpVPAmp->Set(UCLIB_CONTROL_CODE_DAT, 0, NULL);
			}
		}
	}

}


