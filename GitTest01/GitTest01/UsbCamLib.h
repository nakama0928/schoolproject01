/* UsbCamLib.h Ver1.11 */
/* ※　iPokr制御用C++クラスのヘッダファイルです */

#pragma once

#include <dshow.h>
#pragma include_alias( "dxtrans.h", "qedit.h" )
#define __IDxtCompositor_INTERFACE_DEFINED__
#define __IDxtAlphaSetter_INTERFACE_DEFINED__
#define __IDxtJpeg_INTERFACE_DEFINED__
#define __IDxtKey_INTERFACE_DEFINED__
#include "qedit.h"
#pragma comment(lib,"strmiids.lib")



/* 定数定義 */
#define UCLIB_CAMERA_MAX				16
#define UCLIB_ID_LEN					16

#define UCLIB_SIZE_X_DEFAULT			640
#define UCLIB_SIZE_Y_DEFAULT			640
#define UCLIB_SIZE_XY_DEFAULT			409600
#define UCLIB_OFFSET_X_DEFAULT			0
#define UCLIB_OFFSET_Y_DEFAULT			0

#define UCLIB_SIZE_X_MAX				640
#define UCLIB_SIZE_Y_MAX				640
#define UCLIB_SIZE_X_MIN				1
#define UCLIB_SIZE_Y_MIN				1

#define UCLIB_OFFSET_X_MAX				639
#define UCLIB_OFFSET_Y_MAX				639
#define UCLIB_OFFSET_X_MIN				0
#define UCLIB_OFFSET_Y_MIN				0

#define UCLIB_DATA_MIN					0
#define UCLIB_DATA_MAX					255

#define UCLIB_TRAINING_CODE1			0
#define UCLIB_TRAINING_CODE2			255

#define UCLIB_CAPTUREMODE_THROUGH		0
#define UCLIB_CAPTUREMODE_NORMAL		1

#define UCLIB_CONTROL_CODE_ADR			0
#define UCLIB_CONTROL_CODE_DAT			1
#define UCLIB_CONTROL_CODE_EXT			2


#define UCLIB_OK						0
#define UCLIB_ERR						-1
#define UCLIB_ERR_CORRECT_DATA			-2
#define UCLIB_ERR_TIMEOUT				-3

#define UCLIB_ERR_CAM_ISBUSY			-5
#define UCLIB_ERR_CAM_NOT_EXIST			-6

#define UCLIB_ERR_LIB_ISBUSY			-10

#define UCLIB_ERR_FUNC_NOT_AVAILABLE	-20

/* Ver1.03b */
#define UCLIB_ADRS_SHT					1
#define UCLIB_ADRS_TRG					4
#define UCLIB_ADRS_PRE					9
#define UCLIB_ADRS_ITG					10
#define UCLIB_ADRS_MICON_VER			191
#define UCLIB_ADRS_TRG_FPGA				285


//GrabberCallback関数の定義
class SampleGrabberCB :public ISampleGrabberCB
{
typedef void (CALLBACK *FRAMEEND_EVENT)(void *);

private:
	PVOID pUBuf;	
	PVOID pCBuf;
	char *pCorrectFlg;
	char CorrectCnt;
	char *pSnapFlg;
	char SnapCnt;

	int size_x;
	int size_y;
	int offset_x;
	int offset_y;

	FRAMEEND_EVENT FrameEndEvent;
	void *MasterProc;


public:
	//コンストラクタ	
	SampleGrabberCB()
	{
	}

	//デストラクタ
	~SampleGrabberCB()
	{
	}

	STDMETHODIMP_(ULONG) AddRef()
	{
		return 2;
	}

	STDMETHODIMP_(ULONG) Release()
	{
		return 1;
	}

	STDMETHODIMP QueryInterface(REFIID riid, void ** ppv)
	{
		if((riid == IID_ISampleGrabberCB) || (riid == IID_IUnknown)){
			*ppv = (void *)static_cast<ISampleGrabberCB*>(this);
			return NOERROR;
		}
		return E_NOINTERFACE;
	}

	STDMETHODIMP SampleCB(double SampleTime, IMediaSample *pSample)
	{
		return S_OK;
	}

	//フレーム毎に呼ばれる関数
	STDMETHODIMP BufferCB(double dblSampleTime, BYTE *pBuffer, long lBufferSize)
	{
		//補正データ識別
		if((*pBuffer == UCLIB_TRAINING_CODE1) && (*(pBuffer+1) == UCLIB_TRAINING_CODE2)){	//補正データの時
			//リクエストがあった場合のみ、補正データバッファを更新する
			if(CorrectCnt){
				//pCBufへコピーする
				memcpy((unsigned char *)pCBuf, (unsigned char *)pBuffer, lBufferSize);

				//補正データを取り込んだことを上位に伝える。
				CorrectCnt --;
				*pCorrectFlg = CorrectCnt;
			}
		} else {										//Videoデータの時
			char *src, *dst, *cal;
			int i, z;

			cal = (char *)pCBuf;		//補正データ
			src = (char *)pBuffer;		//キャプチャデータ(コピー元)
			dst = (char *)pUBuf;		//ユーザーバッファ(コピー先)

			//ユーザーバッファへコピー
			for(i = 0; i < lBufferSize; i++){

				//垂直方向ROIチェック
				if((i / UCLIB_SIZE_Y_DEFAULT) < offset_y)
					continue;
				if((i / UCLIB_SIZE_Y_DEFAULT) >= (size_y + offset_y))
					continue;
				//水平方向ROIチェック
				if((i % UCLIB_SIZE_X_DEFAULT) < offset_x)
					continue;
				if((i % UCLIB_SIZE_X_DEFAULT) >= (size_x + offset_x))
					continue;

				//補正データを反映
				z = (src[i] & 0xff);
				if(z != UCLIB_DATA_MAX){
					z += cal[i];
					if(z > UCLIB_DATA_MAX)
						z = UCLIB_DATA_MAX;
					if(z < UCLIB_DATA_MIN)
						z = UCLIB_DATA_MIN;
				}
				*dst++ = (char)z & 0xff;

			}
			//スナップ
			if(SnapCnt){
				SnapCnt = 0;
				*pSnapFlg = SnapCnt;
			}

			//フレームエンドコールバック
			if(FrameEndEvent && lBufferSize == UCLIB_SIZE_XY_DEFAULT)
				FrameEndEvent(MasterProc);

		}

		return S_OK;
	}

	//コールバック関数、バッファアドレスセット
	void SetPtr(PVOID ptr1, PVOID ptr2, char *flg, void *func, void *proc)
	{
		pUBuf = ptr1;
		pCBuf = ptr2;

		pCorrectFlg = flg;
		CorrectCnt = *flg;
		pSnapFlg = NULL;
		SnapCnt = 0;

		FrameEndEvent = (FRAMEEND_EVENT)func;
		MasterProc = proc;

		size_x = UCLIB_SIZE_X_DEFAULT;
		size_y = UCLIB_SIZE_Y_DEFAULT;
		offset_x = UCLIB_OFFSET_X_DEFAULT;
		offset_y = UCLIB_OFFSET_Y_DEFAULT;

	}

	//切り出し設定
	void SetROI(int siz_x, int siz_y, int off_x, int off_y)
	{
		size_x = siz_x;
		size_y = siz_y;
		offset_x = off_x;
		offset_y = off_y;

	}

	//スナップモード
	void SetSnap(char *flg)
	{
		pSnapFlg = flg;
		SnapCnt = *flg;

	}

};



class UsbCamLib
{

//カメラ情報構造体の定義
typedef struct UsbCameraInformation{
		BOOL isExist;
		BOOL isBusy;
		BOOL isColorModel;
		char id[UCLIB_ID_LEN];

		unsigned short USBVer;	/* Ver1.04拡張 */

	} UCI_TAG;
	
//カメラ制御構造体の定義
typedef struct UsbControlFunc{

		IGraphBuilder			*pGraph;
		ISampleGrabber			*pGrab;
		IBaseFilter				*pF;
		IBaseFilter				*pCap[UCLIB_CAMERA_MAX];

	} UCF_TAG;


private:
//ローカル関数プロトタイプ宣言(旧バージョン互換)
	IPin *_GetPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir);

//ローカル関数プロトタイプ宣言(Ver1.02拡張)
	/* 無し */

//ローカル関数プロトタイプ宣言(Ver1.03b追加)
	void GetMiconVersion(int CameraNo);
	void Patch000(IAMVideoProcAmp *lpVPAmp, int Adrs);

public:
	UsbCamLib(void);
	~UsbCamLib(void);

//関数プロトタイプ宣言(旧バージョン互換)
	int Init(void);
	int SetParam(PVOID pBuf1, PVOID pBuf2, void *hFuncAddress, void *hProcAddress);
	int Open(int CameraNo, int flg);
	int Close(int CameraNo);
	int Run(int CameraNo);
	int Stop(int CameraNo);
	int Pause(int CameraNo);
	int SetVal(int CameraNo, int Adrs, int Data);
	int GetVal(int CameraNo, int Adrs);
	int SpiCtl(int CameraNo, int Data);


//関数プロトタイプ宣言(Ver1.02拡張)
	int InitLibEx(void);
	int UninitLibEx(void);
	int SetParamEx(PVOID pBuf1, PVOID pBuf2, void *hFuncAddress, void *hProcAddress, int SizeX, int SizeY, int OffsetX, int OffsetY);

	int DetectCamEx(void);
	int OpenCamEx(int CameraNo, int flg);
	int CloseCamEx(void);
	int SwitchCamEx(int CameraNo, int flg);
	int Snap(int CameraNo, int timeout);		/* Ver1.04で引数修正、機能実装 */

	int	GetInfoCamNo(char *CameraID);
	int GetInfoConnectCamNo();
	BOOL GetInfoIsBusy(int CameraNo);
	BOOL GetInfoIsExist(int CameraNo);
	BOOL GetInfoIsColorModel(int CameraNo);

	int	GetInfoUSBVersion(int CameraNo);			/* Ver1.04拡張 */

	SampleGrabberCB *pSampleGrabberCB;


//変数宣言
	PVOID mpBuf;
	PVOID mpCBuf;
	void* mhFuncAdrs;
	void* mhProcAdrs;

	int mSizeX;
	int mSizeY;
	int mOffsetX;
	int mOffsetY;

	int mCamNum;
	int mConnectCamNo;

	int mMiconVer;		/* Ver1.03b */

	UCF_TAG mUsbCam;
	UCI_TAG mCamInfo[UCLIB_CAMERA_MAX];

};

