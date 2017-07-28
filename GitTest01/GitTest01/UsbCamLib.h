/* UsbCamLib.h Ver1.11 */
/* ���@iPokr����pC++�N���X�̃w�b�_�t�@�C���ł� */

#pragma once

#include <dshow.h>
#pragma include_alias( "dxtrans.h", "qedit.h" )
#define __IDxtCompositor_INTERFACE_DEFINED__
#define __IDxtAlphaSetter_INTERFACE_DEFINED__
#define __IDxtJpeg_INTERFACE_DEFINED__
#define __IDxtKey_INTERFACE_DEFINED__
#include "qedit.h"
#pragma comment(lib,"strmiids.lib")



/* �萔��` */
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


//GrabberCallback�֐��̒�`
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
	//�R���X�g���N�^	
	SampleGrabberCB()
	{
	}

	//�f�X�g���N�^
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

	//�t���[�����ɌĂ΂��֐�
	STDMETHODIMP BufferCB(double dblSampleTime, BYTE *pBuffer, long lBufferSize)
	{
		//�␳�f�[�^����
		if((*pBuffer == UCLIB_TRAINING_CODE1) && (*(pBuffer+1) == UCLIB_TRAINING_CODE2)){	//�␳�f�[�^�̎�
			//���N�G�X�g���������ꍇ�̂݁A�␳�f�[�^�o�b�t�@���X�V����
			if(CorrectCnt){
				//pCBuf�փR�s�[����
				memcpy((unsigned char *)pCBuf, (unsigned char *)pBuffer, lBufferSize);

				//�␳�f�[�^����荞�񂾂��Ƃ���ʂɓ`����B
				CorrectCnt --;
				*pCorrectFlg = CorrectCnt;
			}
		} else {										//Video�f�[�^�̎�
			char *src, *dst, *cal;
			int i, z;

			cal = (char *)pCBuf;		//�␳�f�[�^
			src = (char *)pBuffer;		//�L���v�`���f�[�^(�R�s�[��)
			dst = (char *)pUBuf;		//���[�U�[�o�b�t�@(�R�s�[��)

			//���[�U�[�o�b�t�@�փR�s�[
			for(i = 0; i < lBufferSize; i++){

				//��������ROI�`�F�b�N
				if((i / UCLIB_SIZE_Y_DEFAULT) < offset_y)
					continue;
				if((i / UCLIB_SIZE_Y_DEFAULT) >= (size_y + offset_y))
					continue;
				//��������ROI�`�F�b�N
				if((i % UCLIB_SIZE_X_DEFAULT) < offset_x)
					continue;
				if((i % UCLIB_SIZE_X_DEFAULT) >= (size_x + offset_x))
					continue;

				//�␳�f�[�^�𔽉f
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
			//�X�i�b�v
			if(SnapCnt){
				SnapCnt = 0;
				*pSnapFlg = SnapCnt;
			}

			//�t���[���G���h�R�[���o�b�N
			if(FrameEndEvent && lBufferSize == UCLIB_SIZE_XY_DEFAULT)
				FrameEndEvent(MasterProc);

		}

		return S_OK;
	}

	//�R�[���o�b�N�֐��A�o�b�t�@�A�h���X�Z�b�g
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

	//�؂�o���ݒ�
	void SetROI(int siz_x, int siz_y, int off_x, int off_y)
	{
		size_x = siz_x;
		size_y = siz_y;
		offset_x = off_x;
		offset_y = off_y;

	}

	//�X�i�b�v���[�h
	void SetSnap(char *flg)
	{
		pSnapFlg = flg;
		SnapCnt = *flg;

	}

};



class UsbCamLib
{

//�J�������\���̂̒�`
typedef struct UsbCameraInformation{
		BOOL isExist;
		BOOL isBusy;
		BOOL isColorModel;
		char id[UCLIB_ID_LEN];

		unsigned short USBVer;	/* Ver1.04�g�� */

	} UCI_TAG;
	
//�J��������\���̂̒�`
typedef struct UsbControlFunc{

		IGraphBuilder			*pGraph;
		ISampleGrabber			*pGrab;
		IBaseFilter				*pF;
		IBaseFilter				*pCap[UCLIB_CAMERA_MAX];

	} UCF_TAG;


private:
//���[�J���֐��v���g�^�C�v�錾(���o�[�W�����݊�)
	IPin *_GetPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir);

//���[�J���֐��v���g�^�C�v�錾(Ver1.02�g��)
	/* ���� */

//���[�J���֐��v���g�^�C�v�錾(Ver1.03b�ǉ�)
	void GetMiconVersion(int CameraNo);
	void Patch000(IAMVideoProcAmp *lpVPAmp, int Adrs);

public:
	UsbCamLib(void);
	~UsbCamLib(void);

//�֐��v���g�^�C�v�錾(���o�[�W�����݊�)
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


//�֐��v���g�^�C�v�錾(Ver1.02�g��)
	int InitLibEx(void);
	int UninitLibEx(void);
	int SetParamEx(PVOID pBuf1, PVOID pBuf2, void *hFuncAddress, void *hProcAddress, int SizeX, int SizeY, int OffsetX, int OffsetY);

	int DetectCamEx(void);
	int OpenCamEx(int CameraNo, int flg);
	int CloseCamEx(void);
	int SwitchCamEx(int CameraNo, int flg);
	int Snap(int CameraNo, int timeout);		/* Ver1.04�ň����C���A�@�\���� */

	int	GetInfoCamNo(char *CameraID);
	int GetInfoConnectCamNo();
	BOOL GetInfoIsBusy(int CameraNo);
	BOOL GetInfoIsExist(int CameraNo);
	BOOL GetInfoIsColorModel(int CameraNo);

	int	GetInfoUSBVersion(int CameraNo);			/* Ver1.04�g�� */

	SampleGrabberCB *pSampleGrabberCB;


//�ϐ��錾
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

