//define
//�E�B���h�E�T�C�Y
#define window_x 1920
#define window_y 1080
//0.294mm/��f(RDT196M)
#define PITCH 0.294
//�h���ʒu 5deg
#define INTDEG 15
//������
#define OBDIST 500.0
#define DIR 8												//������
#define DISP_TIME 1000										//��ʂɎh����񎦂��鎞��			�Q�l�E�u�ڂɂ����鎞�ԁF100�`150ms�Ƃ���
#define TOTAL_EXP_TIME (5 * 60000)							//�������ԁF5m�i300000ms�j
#define TIMES 40											//�h���񎦉�
#define EXP_TIME (TOTAL_EXP_TIME - (DISP_TIME * TIMES))		//������l��������������
#define BLANK_TIME 1500										//��ʂɉ����\�����Ȃ�����


//�\����
typedef struct {
	int x; //�h����x���W
	int y; //�h����y���W
}Mark;


using namespace std;
Mark c[8];


void setArrayEquality(int preSti[TIMES]);				//�h�����ϓ��Ɋ���U��֐��C�@�߂�l�F�h��������U�����z��
void stiTiming(int timing[TIMES - 1]);					//�h���\���֐�
int disp(int sti[TIMES], int timing[TIMES - 1], int Count,cv::Mat img1,cv::Mat img2, cv::Mat img3, std::string);	//��ʕ\���p�֐��i1���[�v�j
void show(int preSti[TIMES], int timing[TIMES - 1], int Count);		//��ʕ\���p�֐��i�S�́j
cv::Mat make_window();									//�E�B���h�E�쐬�֐�
cv::Mat make_cross(cv::Mat);							//�\���쐬�֐�
cv::Mat make_circle(cv::Mat img, int Count, int preSti[TIMES]);		//�h���_�쐬�֐�
void file_write(DWORD, std::string, int, int, int);				//�t�@�C���������݊֐�
void mark();											//�h���_�ʒu����֐�



void setArrayEquality(int preSti[TIMES])
{
	//int preSti[TIMES];			//�h���񎦕����i�[�p�z��
	bool sti[DIR] = { false };		//�ϓ��Ɋ���U�邽�߂̔z��
	int i;
	int rand;						//�����i�[�ϐ�
	int c = DIR;					//sti�����ׂ�true�ɂȂ��Ă��Ȃ����`�F�b�N����ϐ�

	cv::RNG rng(GetTickCount());


	for (i = 0; i < TIMES; i++)		//�����i�[�z��ɒl������
	{
		rand = rng.uniform( 0, 8);
		rand %= DIR;				//0�`DIR-1�̒l������

		if (c <= 0)					//sti�z�񂪂��ׂ�true�̂Ƃ�
		{
			for (c; c < DIR; c++)	//�z������ׂ�false�ɖ߂�
			{
				sti[c] = false;
			}
		}

		while (1)
		{
			if (sti[rand] == false)			//sti[rand]��false�Ȃ�true�ɕς��ă��[�v���o��
			{
				sti[rand] = true;
				c--;
				break;
			}
			else							// rand���Ď擾���Ă܂����[�v����
			{
				rand = rng.uniform(0, 8);
				rand %= DIR;				//0�`DIR-1�̒l������
			}
		}
		preSti[i] = rand;			//�z��ɒl������
	}
}



void stiTiming(int timing[TIMES - 1])				//���Ԃ�ms�P�ʂōl����
{
	//int timing[TIMES - 1];					//�h���\���^�C�~���O�i�[�z��
	int i = 0, j;
	int tim = EXP_TIME / (TIMES - 1);		//�ϓ��ɂ��ꂽ�^�C�~���O
	int pair1, pair2;						//�^�C�~���O�����Ɏg���A���Ԃ𒲐�����y�A���w�肷�邽�߂̕ϐ�
	int repl_time;							//�y�A�ő�����������鎞��
	cv::RNG rng(GetTickCount());			//���������p
	int max_repl = tim + (tim / 2);			//�ω������鎞�Ԃ̍ő啝
	

	for (j = 0; j < TIMES - 1; j++)
	{
		timing[j] = tim;					//�^�C�~���O�z��ɋϓ��Ԋu�̃^�C�~���O��������
	}
	for (j = 0; j < 50; j++)
	{
		repl_time = rng.uniform(1, tim / 2);
		do
		{
			pair1 = rng.uniform(0, TIMES - 1);
		} while (timing[pair1] + repl_time > max_repl);						//�ω��ʂ��v�Z���A�ω������鎞�Ԃ̍ő啝���z���Ȃ��悤�ȓY�����ɂ���

		do
		{
			pair2 = rng.uniform(0, TIMES - 1);
		} while (timing[pair2] - repl_time < BLANK_TIME || pair1 == pair2);			//���� + �����Y�����ɂȂ�Ȃ��悤�ɂ���

		timing[pair1] += repl_time;			//���ۂɑ������
		timing[pair2] -= repl_time;
	}
}



int disp(int sti[TIMES], int timing[TIMES - 1],volatile int Count, cv::Mat img1, cv::Mat img2, cv::Mat img3, std::string fname)
{
	LARGE_INTEGER nFreq, nBefore, nAfter;	//�L�[���͗p�ϐ�
	DWORD dwTime = 0;
	int x;									//�L�[�擾�ϐ�
	int elap;

	memset(&nFreq, 0x00, sizeof nFreq);		//�ϐ�������
	memset(&nBefore, 0x00, sizeof nBefore);
	memset(&nAfter, 0x00, sizeof nAfter);


	QueryPerformanceFrequency(&nFreq);

	//system("cls");							//��ʂ��N���A����
	//cout << sti[Count] << endl;				//�z��o��
	cv::imshow("image", img2);				//��ʕ\��

	QueryPerformanceCounter(&nBefore);
	x = cv::waitKey(DISP_TIME);				//�h���񎦎��ԕ��L�[���͂�҂�
	//x = cv::waitKey(0);		//debug
	elap = (int)((nAfter.QuadPart - nBefore.QuadPart) * 1000 / nFreq.QuadPart);
	if (x == 32)		//27�@=�@�G�X�P�[�v�L�[�@�C32�@=�@�X�y�[�X�L�[�@�C-1�@=�@�G���[
	{
		QueryPerformanceCounter(&nAfter);	//nAfter�ɃL�[�������ꂽ���̃p�t�H�[�}���X�J�E���^�̒l���擾
		dwTime = (DWORD)((nAfter.QuadPart - nBefore.QuadPart) * 1000 / nFreq.QuadPart);			//dwTime��nAfter-nBefore�̒l������(ms)
		file_write(dwTime, fname, sti[Count], timing[Count], Count);			//�t�@�C���ɏ�������
	}
	else if (x == 27)		//���[�v�𔲂���
	{
		return -1;
	}

	while (elap < DISP_TIME - dwTime)		//�������~�߂��Ɏ��Ԃ��v��
	{
		QueryPerformanceCounter(&nAfter);
		elap = (int)((nAfter.QuadPart - nBefore.QuadPart) * 1000 / nFreq.QuadPart);

		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)			//���[�v�𔲂���
		{
			return -1;
		}
	}

	//��ʂ��N���A����
	cv::imshow("image", img3);				//��ʕ\��
	cv::waitKey(10);
	if (x == 27)			//���[�v�𔲂���
	{
		return -1;
	}
	//���܂ł̕\���^�C�~���O���L�[���͂�҂�
	QueryPerformanceFrequency(&nFreq);
	QueryPerformanceCounter(&nBefore);
	QueryPerformanceCounter(&nAfter);
	elap = (int)((nAfter.QuadPart - nBefore.QuadPart) * 1000 / nFreq.QuadPart);
	while (elap < BLANK_TIME - 10)		//�������~�߂��Ɏ��Ԃ��v��
	{
		QueryPerformanceCounter(&nAfter);
		elap = (int)((nAfter.QuadPart - nBefore.QuadPart) * 1000 / nFreq.QuadPart);

		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)			//���[�v�𔲂���
		{
			return -1;
		}
	}
	//system("cls");							//��ʂ��N���A����
	cv::imshow("image", img1);				//��ʕ\��
	cv::waitKey(10);
	if (x == 27)			//���[�v�𔲂���
	{
		return -1;
	}
	//���܂ł̕\���^�C�~���O���L�[���͂�҂�
	QueryPerformanceFrequency(&nFreq);
	QueryPerformanceCounter(&nBefore);
	QueryPerformanceCounter(&nAfter);
	elap = (int)((nAfter.QuadPart - nBefore.QuadPart) * 1000 / nFreq.QuadPart);
	while (elap < timing[Count] - 10)		//�������~�߂��Ɏ��Ԃ��v��
	{
		QueryPerformanceCounter(&nAfter);
		elap = (int)((nAfter.QuadPart - nBefore.QuadPart) * 1000 / nFreq.QuadPart);

		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)			//���[�v�𔲂���
		{
			return -1;
		}
	}
	return 0;
}



void file_write(DWORD dwTime, std::string fname, int dir, int timing, int Count) {
	
	std::ofstream fs(fname, std::ios::app);		//�t�@�C�����J��

	fs << Count << "," << dwTime << "," << dir << "," << timing << endl;			//�t�@�C����ms�P�ʂŏ�������

	fs.close();									// �t�@�C�������
}



//��ʕ\��
void show(int preSti[TIMES], int timing[TIMES - 1], int Count) {

	static int fin = 0;			//�I���t���O
	std::string subnm;			// �����Q���Җ�

	cout << "---�h���ω����o����3---" << endl;
	cout << "�����Q����=";
	cin >> subnm;
	cout << "��ʒ����ɏ\�����\�������̂ł����𒍎����Ă��������B" << endl << endl <<"�����_(�\)���\�����ꂽ���b��Ɏh�����\������܂��B" << endl;
	cout << "��" << TOTAL_EXP_TIME / 60000 << "���Ԃ̎����ł��B" << endl;
	cout << "�h����" << TIMES << "��񎦂���܂��B" << endl;
	getchar();
	getchar();

	// �񓚏��L�^����
	std::string fname;
	//�t�@�C�����̐���
	fname = "����3_" + subnm + ".csv";

	//�E�B���h�E�쐬
	//�����_(�\��)�\���E�B���h�E
	cv::Mat img1 = make_window();
	//�����_(�\��)�A�h���_(�~)�\���E�B���h�E
	cv::Mat img2 = make_window();
	//�܂�����ȉ摜�\���E�B���h�E
	cv::Mat img3 = make_window();

	setArrayEquality(preSti);		//�ʒu�w��z��쐬
	stiTiming(timing);				//���Ԏw��z��쐬

	//�����_�쐬
	img1 = make_cross(img1);
	img2 = make_cross(img2);

	//�h���_�ʒu�쐬
	mark();

	//�J�[�\��������
	ShowCursor(false);

	cv::imshow("image", img1);				//��ʕ\��
	cv::waitKey(50);
	Sleep(4000);
	//��ʕ\���p���[�v
	while (1) {

		//�h���؂�ւ�
		img2 = make_circle(img2, Count, preSti);

		//��ʕ\��
		fin = disp(preSti, timing, Count, img1, img2, img3, fname);

		Count++;

		//�����񐔂��K��񐔂𒴂����烋�[�v�𔲂���
		if (Count >= TIMES)
		{
			break;
		}
		if (fin == -1)
		{
			break;
		}
	}


	//�I������
	// ���ׂĂ�window��j��
	cv::destroyAllWindows();
}



//�E�B���h�E�쐬
cv::Mat make_window() {
	HWND handle;		//�E�B���h�E�n���h���p�ϐ�
	//img�̏�����
	cv::Mat img(cv::Size(window_x, window_y), CV_8UC3, cv::Scalar(255, 255, 255));
	//�E�B���h�E�ւ̖��O�t��
	cv::namedWindow("image", CV_WINDOW_AUTOSIZE);
	// �E�B���h�E�𒆉��Ɉړ�
	cv::moveWindow("image", 0, 0);
	handle = GetActiveWindow();			//�E�B���h�E�n���h���擾
	SetForegroundWindow(handle);		//�A�N�e�B�u�E�B���h�E�ɕύX
	return img;
}



//�\�����쐬����֐�
cv::Mat make_cross(cv::Mat img) {
	//�\��(�����_�쐬)
	cv::putText(img, "+", cv::Point(window_x / 2, window_y / 2), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0, 0, 0), 3, CV_AA);
	return img;
}



//�h���_�쐬(int�z��)
cv::Mat make_circle(cv::Mat img, int Count, int preSti[TIMES]) {

	int j, k;
	//�h���\���ʒu���o��
	k = preSti[Count];

	//�O��̎h�����폜
	for (j = 0; j < 8; j++) {
		cv::circle(img, cv::Point(c[j].x, c[j].y), 20, cv::Scalar(255, 255, 255), -1, CV_AA);
	}
	//�V���Ȏh���쐬
	cv::circle(img, cv::Point(c[k].x, c[k].y), 10, cv::Scalar(200, 0, 0), -1, CV_AA);

	return img;
}



//�h���_�ʒu����
void mark() {
	double dtmp;
	int dpix;
	int i, j, k = 0;

	// �h���z�u�̎Z�o
	dtmp = OBDIST*tan(INTDEG*M_PI / 180.0);
	dtmp /= PITCH;
	dpix = (int)dtmp;

	for (j = -1; j <= 1; j++) {
		for (i = -1; i <= 1; i++) {
			if (i == 0 && j == 0) {
				continue;
			}
			c[k].x = window_x / 2 + i*dpix;
			c[k].y = window_y / 2 + j*dpix;
			k++;
		}
	}

}