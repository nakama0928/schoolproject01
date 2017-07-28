/* UsbCamLib.cpp Ver1.11 */
/* ���@iPokr����pC++�N���X�̃\�[�X�t�@�C���ł� */

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
//�o�b�t�@�N���A
	ZeroMemory(&mUsbCam,  sizeof(UCF_TAG));
	ZeroMemory(&mCamInfo, sizeof(UCI_TAG));

	pSampleGrabberCB = NULL;
	mConnectCamNo = -1;
	mMiconVer = -1;

//COM���C�u�����̏�����
	HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if(FAILED(hr))
		return UCLIB_ERR;

	return UCLIB_OK;
}


int UsbCamLib::SetParam(PVOID pBuf1, PVOID pBuf2, void *hFuncAddress, void *hProcAddress)
{
	mpBuf  = pBuf1;		//�r�f�I�L���v�`���p�ݒ�
	mpCBuf = pBuf2;		//�␳�f�[�^�ۑ��p�ݒ�

	mhFuncAdrs = hFuncAddress;	//�R�[���o�b�N�֐��A�h���X
	mhProcAdrs = hProcAddress;	//�R�[���o�b�N�֐��p�����[�^

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

//�͈͂̃`�F�b�N
	if((CameraNo < 0) || (CameraNo >= UCLIB_CAMERA_MAX))
		return UCLIB_ERR;

//�t�B���^�O���t�}�l�[�W���쐬 pGraph

	hr = CoCreateInstance(CLSID_FilterGraph, 0 ,CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void **)&mUsbCam.pGraph);
	if(FAILED(hr))
		return UCLIB_ERR;

//�V�X�e���f�o�C�X�񋓎q�̍쐬
	hr = CoCreateInstance(CLSID_SystemDeviceEnum, 0 ,CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, (void **)&lpDevEnum);
	if(FAILED(hr))
		return UCLIB_ERR;

//�񋓎q�̎擾
	hr = lpDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &lpEnum, 0);
	if(FAILED(hr))
		return UCLIB_ERR;

//���j�J�̎擾
	unsigned long cFetched;
	wchar_t SrcName[32];
	int i, lDetectFlg;

	lDetectFlg = 0;
	mCamNum = 0;
	for(i = 0; i < UCLIB_CAMERA_MAX; i++){
		if(lpEnum->Next(1, &lpMoniker, &cFetched) == S_OK){
			
			//DisplayName�̎擾
			LPOLESTR strMonikerName = 0;
			hr = lpMoniker->GetDisplayName(NULL, NULL, &strMonikerName);
			if(FAILED(hr))
				return UCLIB_ERR;

			//DisplayName��iPokr�������ēo�^
			if(wcsstr(strMonikerName, L"@device:pnp") && wcsstr(strMonikerName, L"vid_04b4&pid_00f2")){

				//�I�u�W�F�N�g������ pCap
				lpMoniker->BindToObject(0, 0, IID_IBaseFilter, (void **)&mUsbCam.pCap[mCamNum]);
				mUsbCam.pCap[mCamNum]->SetSyncSource(NULL);
				mCamInfo[mCamNum].isExist = TRUE;

				if(mCamNum == CameraNo){
					//�O���t�Ƀt�B���^��ǉ�
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

//�^�[�Q�b�g�̃J������������Ȃ�����
	if(!lDetectFlg)
		return UCLIB_ERR;

//�T���v���O���o�̐��� pF,pGrab
	hr = CoCreateInstance(CLSID_SampleGrabber, 0, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (LPVOID *)&mUsbCam.pF);
	if(FAILED(hr))
		return UCLIB_ERR;
	hr = mUsbCam.pF->QueryInterface(IID_ISampleGrabber, (void **)&mUsbCam.pGrab);
	if(FAILED(hr))
		return UCLIB_ERR;

//�t�B���^�O���t�ւ̒ǉ�
	wchar_t GrabName[32];
	swprintf_s(GrabName,32, L"Grabber");
	hr = mUsbCam.pGraph->AddFilter(mUsbCam.pF, GrabName);
	if(FAILED(hr))
		return UCLIB_ERR;

//�T���v���O���o�̐ڑ�
	IPin *lpSrc;
	IPin *lpDst;

	// �s���̎擾
	lpSrc = _GetPin(mUsbCam.pCap[CameraNo], PINDIR_OUTPUT);
	lpDst = _GetPin(mUsbCam.pF, PINDIR_INPUT);

// �s���̐ڑ�
	hr = mUsbCam.pGraph->Connect(lpSrc, lpDst);
	if(FAILED(hr))
		return UCLIB_ERR;
	lpSrc->Release();
	lpDst->Release();

//�O���o�̃��[�h�ݒ�
	hr = mUsbCam.pGrab->SetBufferSamples(FALSE);
	if(FAILED(hr))
		return UCLIB_ERR;
	hr = mUsbCam.pGrab->SetOneShot(FALSE);
	if(FAILED(hr))
		return UCLIB_ERR;
//�R�[���o�b�N�֐��̓o�^
	pSampleGrabberCB = new SampleGrabberCB();
	hr = mUsbCam.pGrab->SetCallback(pSampleGrabberCB, 1);
	if(FAILED(hr))
		return UCLIB_ERR;


//�摜��荞�݃o�b�t�@�ݒ�
	char state = 1;
	int count = 1500;
	pSampleGrabberCB->SetPtr(mpBuf, mpCBuf, &state, mhFuncAdrs, mhProcAdrs);
	pSampleGrabberCB->SetROI(mSizeX, mSizeY, mOffsetX, mOffsetY);


/** Ver1.03b **/
	//�t�@�[���E�F�A�o�[�V�����`�F�b�N
	GetMiconVersion(CameraNo);


//�␳�f�[�^�����[�h���Ȃ�
	if(flg == UCLIB_CAPTUREMODE_THROUGH)
		return UCLIB_OK;

//�g���K�[���[�h�m�F
	int trg = GetVal(CameraNo, 4);
	if(trg != 0)
		SetVal(CameraNo, 4, 0);


//�␳�f�[�^��荞�ݏ���
	//�]���X�^�[�g
	Run(CameraNo);

	//���[�h�ύX(�␳�f�[�^���o)
	SpiCtl(CameraNo, 88);

	//�␳�f�[�^��荞�ݑ҂�
	while(state){
		Sleep(5);
		count --;
		if(!count)
			return UCLIB_ERR_CORRECT_DATA;
	}

	//�]���X�g�b�v
	Stop(CameraNo);

//�g���K�[���[�h�𕜌�
	if(trg != 0)
		SetVal(CameraNo, 4, trg);

	return UCLIB_OK;

}


int UsbCamLib::Close(int CameraNo)
{
	mConnectCamNo = -1;

//���������
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

//COM���C�u���������
	CoUninitialize();

	return UCLIB_OK;
}


//�L���v�`���J�n
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


//�L���v�`����~
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

//�L���v�`���ꎞ��~
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


//�J�������W�X�^�Z�b�g
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


//�J�������W�X�^�Q�b�g
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


//�J��������ݒ�
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


//�t�B���^�̃s�����擾����
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
/*** �ȉ��AVer1.02�g���֐� ***/
/*****************************/



int UsbCamLib::InitLibEx(void)
{
//�o�b�t�@�N���A
	ZeroMemory(&mUsbCam,  sizeof(UCF_TAG));
	ZeroMemory(&mCamInfo, sizeof(UCI_TAG));

	pSampleGrabberCB = NULL;
	mConnectCamNo = -1;
	mMiconVer = -1;

//COM���C�u�����̏�����
	HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if(FAILED(hr))
		return UCLIB_ERR;

	return UCLIB_OK;
}


int UsbCamLib::UninitLibEx(void)
{


//COM���C�u���������
	CoUninitialize();

	return UCLIB_OK;
}


int UsbCamLib::SetParamEx(PVOID pBuf1, PVOID pBuf2, void *hFuncAddress, void *hProcAddress, int SizeX, int SizeY, int OffsetX, int OffsetY)
{
	mpBuf  = pBuf1;		//�r�f�I�L���v�`���p�ݒ�
	mpCBuf = pBuf2;		//�␳�f�[�^�ۑ��p�ݒ�

	mhFuncAdrs = hFuncAddress;	//�R�[���o�b�N�֐��A�h���X
	mhProcAdrs = hProcAddress;	//�R�[���o�b�N�֐��p�����[�^


	//�ݒ�l��1�ȏオ�L��
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

	//�ݒ�l�ɕs�������������ꍇ�A�I�t�Z�b�g�D��
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


	//�␳�f�[�^�p�o�b�t�@���N���A���Ă���
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

//Grab���Ȃ�AERR����(UCLIB_ERR_LIB_ISBUSY)

	CloseCamEx();

//�t�B���^�O���t�}�l�[�W���쐬 pGraph
	hr = CoCreateInstance(CLSID_FilterGraph, 0 ,CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void **)&mUsbCam.pGraph);
	if(FAILED(hr))
		return UCLIB_ERR;

//�V�X�e���f�o�C�X�񋓎q�̍쐬
	hr = CoCreateInstance(CLSID_SystemDeviceEnum, 0 ,CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, (void **)&lpDevEnum);
	if(FAILED(hr))
		return UCLIB_ERR;

//�񋓎q�̎擾
	hr = lpDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &lpEnum, 0);
	if(FAILED(hr))
		return UCLIB_ERR;

//���j�J�̎擾
	IAMVideoProcAmp *lpVPAmp;
	unsigned long cFetched;
	long Val, Flags;
	int i, j, retry;

	mCamNum = 0;
	for(i = 0; i < UCLIB_CAMERA_MAX; i++){
		if(lpEnum->Next(1, &lpMoniker, &cFetched) == S_OK){
			
			//DisplayName�̎擾
			LPOLESTR strMonikerName = 0;
			hr = lpMoniker->GetDisplayName(NULL, NULL, &strMonikerName);
			if(FAILED(hr))
				return UCLIB_ERR;

			//DisplayName��iPokr�������ēo�^
			if(wcsstr(strMonikerName, L"@device:pnp") && wcsstr(strMonikerName, L"vid_04b4&pid_00f2")){

				//�I�u�W�F�N�g������ pCap
				lpMoniker->BindToObject(0, 0, IID_IBaseFilter, (void **)&mUsbCam.pCap[mCamNum]);
				mUsbCam.pCap[mCamNum]->SetSyncSource(NULL);
				mCamInfo[mCamNum].isExist = TRUE;

				//�J�������擾
				mUsbCam.pCap[mCamNum]->QueryInterface(IID_IAMVideoProcAmp,(void **)&lpVPAmp);
				for(j = 0; j < 16; j ++){
					//�ŏ���pVPAmp���䂪��������܂Ń��g���C����
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
//CameraNo�`�F�b�N
	//-1�Ȃ��ԍŏ��̃A�C�h���J�����ɐڑ�����
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

//Detect�ς݂��`�F�b�N
	if(!mCamInfo[CameraNo].isExist)
		return UCLIB_ERR_CAM_NOT_EXIST;

//�O���t�Ƀt�B���^��ǉ�
	HRESULT hr;
	wchar_t SrcName[32];
	swprintf_s(SrcName,32,L"Video Capture %d", CameraNo);
	hr = mUsbCam.pGraph->AddFilter(mUsbCam.pCap[CameraNo], SrcName);
	if(FAILED(hr))
		return UCLIB_ERR;

//�T���v���O���o�̐��� pF,pGrab
	hr = CoCreateInstance(CLSID_SampleGrabber, 0, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (LPVOID *)&mUsbCam.pF);
	if(FAILED(hr))
		return UCLIB_ERR;
	hr = mUsbCam.pF->QueryInterface(IID_ISampleGrabber, (void **)&mUsbCam.pGrab);
	if(FAILED(hr))
		return UCLIB_ERR;

//�t�B���^�O���t�ւ̒ǉ�
	wchar_t GrabName[32];
	swprintf_s(GrabName,32, L"Grabber");
	hr = mUsbCam.pGraph->AddFilter(mUsbCam.pF, GrabName);
	if(FAILED(hr))
		return UCLIB_ERR;

//�T���v���O���o�̐ڑ�
	IPin *lpSrc;
	IPin *lpDst;

	// �s���̎擾
	lpSrc = _GetPin(mUsbCam.pCap[CameraNo], PINDIR_OUTPUT);
	lpDst = _GetPin(mUsbCam.pF, PINDIR_INPUT);

// �s���̐ڑ�
	hr = mUsbCam.pGraph->Connect(lpSrc, lpDst);
	if(FAILED(hr))
		return UCLIB_ERR;
	lpSrc->Release();
	lpDst->Release();

//�O���o�̃��[�h�ݒ�
	hr = mUsbCam.pGrab->SetBufferSamples(FALSE);
	if(FAILED(hr))
		return UCLIB_ERR;
	hr = mUsbCam.pGrab->SetOneShot(FALSE);
	if(FAILED(hr))
		return UCLIB_ERR;
//�R�[���o�b�N�֐��̓o�^
	pSampleGrabberCB = new SampleGrabberCB();
	hr = mUsbCam.pGrab->SetCallback(pSampleGrabberCB, 1);
	if(FAILED(hr))
		return UCLIB_ERR;


//�摜��荞�݃o�b�t�@�ݒ�
	char state = 1;
	int count = 1500;
	pSampleGrabberCB->SetPtr(mpBuf, mpCBuf, &state, mhFuncAdrs, mhProcAdrs);
	pSampleGrabberCB->SetROI(mSizeX, mSizeY, mOffsetX, mOffsetY);


/** Ver1.03b **/
	//�t�@�[���E�F�A�o�[�V�����`�F�b�N
	GetMiconVersion(CameraNo);


//�␳�f�[�^�����[�h���Ȃ�
	if(flg == UCLIB_CAPTUREMODE_THROUGH){
		mConnectCamNo = CameraNo;
		return UCLIB_OK;
	}

//�g���K�[���[�h�m�F
	int trg = GetVal(CameraNo, 4);
	if(trg != 0)
		SetVal(CameraNo, 4, 0);

//�␳�f�[�^��荞�ݏ���
	//�]���X�^�[�g
	Run(CameraNo);

	//���[�h�ύX(�␳�f�[�^���o)
	SpiCtl(CameraNo, 88);

	//�␳�f�[�^��荞�ݑ҂�
	while(state){
		Sleep(5);
		count --;
		if(!count)
			return UCLIB_ERR_CORRECT_DATA;
	}

	//�]���X�g�b�v
	Stop(CameraNo);

	//�g���K�[���[�h�𕜌�
	if(trg != 0)
		SetVal(CameraNo, 4, trg);

	mConnectCamNo = CameraNo;
	return UCLIB_OK;
}


int UsbCamLib::CloseCamEx()
{
	mConnectCamNo = -1;


//���������
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


/* �J�����؂�ւ� */
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


//�摜��荞�݃o�b�t�@�ݒ�
	char state = 1;
	int count = 1500;
	pSampleGrabberCB->SetPtr(mpBuf, mpCBuf, &state, mhFuncAdrs, mhProcAdrs);


/** Ver1.03b **/
	//�t�@�[���E�F�A�o�[�V�����`�F�b�N
	GetMiconVersion(CameraNo);


//�␳�f�[�^�����[�h���Ȃ�
	if(flg == UCLIB_CAPTUREMODE_THROUGH){
		//�␳�f�[�^�p�o�b�t�@���N���A���Ă���
		int xy = mSizeX * mSizeY;
		ZeroMemory(mpCBuf, xy);

		return UCLIB_OK;
	}

//�g���K�[���[�h�m�F
	int trg = GetVal(CameraNo, 4);
	if(trg != 0)
		SetVal(CameraNo, 4, 0);

//�␳�f�[�^��荞�ݏ���
	//�]���X�^�[�g
	Run(CameraNo);

	//���[�h�ύX(�␳�f�[�^���o)
	SpiCtl(CameraNo, 88);

	//�␳�f�[�^��荞�ݑ҂�
	while(state){
		Sleep(5);
		count --;
		if(!count)
			return UCLIB_ERR_CORRECT_DATA;
	}

	//�]���X�g�b�v
	Stop(CameraNo);

//�g���K�[���[�h�𕜌�
	if(trg != 0)
		SetVal(CameraNo, 4, trg);


	return UCLIB_OK;
}



/* �X�i�b�v��荞�� */
/* Ver1.04�ň����C���A�@�\���� */
/* �摜��荞�݊����̌��o�Ƒ҂����Ԃ͊ȈՓI�Ȃ���*/
/* ���A���^�C�������K�v�ȏꍇ�́A�n�[�h�g���K�[�ƃR�[���o�b�N�֐����g�p���������悢 */
int UsbCamLib::Snap(int CameraNo, int timeout)
{
	/* timeout�̒l��1�ȏ�̎��A�֐����ŃL���v�`�������܂őҋ@(�P�ʂ͂��悻ms) */
	/* 0�ȉ��̏ꍇ�A�L���v�`��������҂����ɔ����� */

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

	//��荞�ݑ҂�
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



/* �J����No.�擾 */
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


/* �ڑ����̃J����No.�擾 */
int UsbCamLib::GetInfoConnectCamNo()
{
	return mConnectCamNo;
}


//Busy��Ԋm�F
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


//�J�������o�m�F
BOOL UsbCamLib::GetInfoIsExist(int CameraNo)
{
	return mCamInfo[CameraNo].isExist;
}


//�J���[���f���m�F
BOOL UsbCamLib::GetInfoIsColorModel(int CameraNo)
{
	return mCamInfo[CameraNo].isColorModel;
}


//USB�o�[�W�����m�F
//����Ȓl�𓾂�ɂ́AUSB�R���g���[���̃t�@�[���E�F�A��
//Ver.004�ȏ�ł���K�v������܂��B
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
/*** �ȉ��AVer1.03b�ǉ��֐� ***/
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

	/* ShutterSpeed�Ɋւ��ݒ�ύX���� */
	if((Adrs == UCLIB_ADRS_SHT) || (Adrs == UCLIB_ADRS_TRG) || (Adrs == UCLIB_ADRS_PRE) || (Adrs == UCLIB_ADRS_ITG)){

		lpVPAmp->Set(UCLIB_CONTROL_CODE_ADR, UCLIB_ADRS_SHT, NULL);
		lpVPAmp->Get(UCLIB_CONTROL_CODE_DAT, &sht, &Flags);
		lpVPAmp->Set(UCLIB_CONTROL_CODE_ADR, UCLIB_ADRS_TRG, NULL);
		lpVPAmp->Get(UCLIB_CONTROL_CODE_DAT, &trg, &Flags);
		lpVPAmp->Set(UCLIB_CONTROL_CODE_ADR, UCLIB_ADRS_PRE, NULL);
		lpVPAmp->Get(UCLIB_CONTROL_CODE_DAT, &pre, &Flags);
		lpVPAmp->Set(UCLIB_CONTROL_CODE_ADR, UCLIB_ADRS_ITG, NULL);
		lpVPAmp->Get(UCLIB_CONTROL_CODE_DAT, &itg, &Flags);

		/* ���A�����V���b�^�[�Ńt���[������� */
		if(trg == 0){
			time = (pre * 2 + 2) * (itg * 2 + 1);
			if(((4 < sht) && (sht < 15)) || ((sht == 15) && (time < 2200))){
				/* FPS�������� */
				lpVPAmp->Set(UCLIB_CONTROL_CODE_ADR, UCLIB_ADRS_TRG_FPGA, NULL);
				lpVPAmp->Set(UCLIB_CONTROL_CODE_DAT, 3, NULL);
			} else {
				lpVPAmp->Set(UCLIB_CONTROL_CODE_ADR, UCLIB_ADRS_TRG_FPGA, NULL);
				lpVPAmp->Set(UCLIB_CONTROL_CODE_DAT, 0, NULL);
			}
		}
	}

}


