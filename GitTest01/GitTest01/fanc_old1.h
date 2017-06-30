#define DIR 8												//������
#define TIMES 40											//�h���񎦉�
#define DISP_TIME 1250										//��ʂɎh����񎦂��鎞��			�Q�l�E�u�ڂɂ����鎞�ԁF100�`150ms�Ƃ���
//#define EXP_TIME (300000 - (DISP_TIME * TIMES))				//�������ԁF300s�i300000ms�j
#define EXP_TIME (120000 - (DISP_TIME * TIMES))									//�������ԁF120s�i120000ms�j		�����p


//void test000(void);
void setArrayEquality(int preSti[TIMES]);		//�h�����ϓ��Ɋ���U��֐��C�@�߂�l�F�h��������U�����z��
void stiTiming(int sti[TIMES], int timing[TIMES - 1]);					//�h���\���֐�
void disp(int sti[TIMES], int timing[TIMES - 1], int Count);							//��ʕ\���p�֐��C�@�����F�\��������

/*void test000(void) {
	cout << "test1" << endl;
}*/

void setArrayEquality(int preSti[TIMES])
{
	//int preSti[TIMES];				//�h���񎦕����i�[�p�z��
	bool sti[DIR] = { false };		//�ϓ��Ɋ���U�邽�߂̔z��
	int i;
	int rand;						//�����i�[�ϐ�
	int c = DIR;					//sti�����ׂ�true�ɂȂ��Ă��Ȃ����`�F�b�N����ϐ�

	cv::RNG rng(GetTickCount());

	//cout << "�����擾�J�n" << endl;		//debug

	for (i = 0; i < TIMES; i++)		//�����i�[�z��ɒl������
	{
		rand = rng.uniform( 0, 8);
		rand %= DIR;				//0�`DIR-1�̒l������

		if (c <= 0)						//sti�z�񂪂��ׂ�true�̂Ƃ�
		{
			for (c; c < DIR; c++)
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
				//rand = (rand + DIR + 1) % DIR;
				rand = rng.uniform(0, 8);
				rand %= DIR;				//0�`DIR-1�̒l������
			}
		}
		preSti[i] = rand;			//�z��ɒl������
		
		//cout << rand << endl;		//debug
		//cout << preSti[i] << endl;	//debug
	}

	//cout << "�����擾����" << endl;		//debug
}

void stiTiming(int sti[TIMES], int timing[TIMES - 1])				//���Ԃ�ms�P�ʂōl����
{
	int i = 0, j;
	//int timing[TIMES - 1];					//�h���\���^�C�~���O�i�[�z��
	int tim = EXP_TIME / (TIMES - 1);		//�ϓ��ɂ��ꂽ�^�C�~���O
	int pair1, pair2;						//�^�C�~���O�����Ɏg���A���Ԃ𒲐�����y�A���w�肷�邽�߂̕ϐ�
	int repl_time;							//�y�A�ő�����������鎞��
	cv::RNG rng(GetTickCount());			//���������p
	int max_repl = tim + (tim / 2);			//�ω������鎞�Ԃ̍ő啝
	LARGE_INTEGER nFreq, nBefore, nAfter;
	int elap;

	int sum = 0;							//debug

	setArrayEquality(sti);

	memset(&nFreq, 0x00, sizeof nFreq);
	memset(&nBefore, 0x00, sizeof nBefore);
	memset(&nAfter, 0x00, sizeof nAfter);

	for (j = 0; j < TIMES - 1; j++)
	{
		timing[j] = tim;					//�^�C�~���O�z��ɋϓ��Ԋu�̃^�C�~���O��������
	}
	for (j = 0; j < 100; j++)
	{
		repl_time = rng.uniform(1, tim / 2);
		do
		{
			pair1 = rng.uniform(0, TIMES - 1);
		} while (timing[pair1] + repl_time > max_repl);						//�ω��ʂ��v�Z���A�ω������鎞�Ԃ̍ő啝���z���Ȃ��悤�ȓY�����ɂ���

		do
		{
			pair2 = rng.uniform(0, TIMES - 1);
		} while (timing[pair2] - repl_time < 0 || pair1 == pair2);			//����A�����Y�����ɂȂ�Ȃ��悤�ɂ���

		timing[pair1] += repl_time;
		timing[pair2] -= repl_time;
	}

	/*
	while (i < TIMES-1)					//debug
	{
		cout << timing[i] << endl;
		sum += timing[i] + DISP_TIME;
		i++;
	}
	cout << "sum=" << sum << endl;
	*/
}
	
void disp(int sti[TIMES], int timing[TIMES - 1], int Count)				//��ʕ\��
{
	LARGE_INTEGER nFreq, nBefore, nAfter;
	int elap;

	system("cls");														//��ʂ��N���A����
	cout << sti[Count] << endl;												//��ʕ\��
	//Sleep(DISP_TIME);
	QueryPerformanceFrequency(&nFreq);
	QueryPerformanceCounter(&nBefore);
	QueryPerformanceCounter(&nAfter);
	elap = (int)((nAfter.QuadPart - nBefore.QuadPart) * 1000 / nFreq.QuadPart);
	while (elap < DISP_TIME)			//�������~�߂��Ɏ��Ԃ��v��
	{
		QueryPerformanceCounter(&nAfter);
		elap = (int)((nAfter.QuadPart - nBefore.QuadPart) * 1000 / nFreq.QuadPart);
	}
	system("cls");
	//Sleep(timing[i]);													//�Ԋu��������~����
	QueryPerformanceFrequency(&nFreq);
	QueryPerformanceCounter(&nBefore);
	QueryPerformanceCounter(&nAfter);
	elap = (int)((nAfter.QuadPart - nBefore.QuadPart) * 1000 / nFreq.QuadPart);
	while (elap < timing[Count])			//�������~�߂��Ɏ��Ԃ��v��
	{
		QueryPerformanceCounter(&nAfter);
		elap = (int)((nAfter.QuadPart - nBefore.QuadPart) * 1000 / nFreq.QuadPart);
	}


	/*while (1)								//�X�y�[�X�L�[�������ƕ\������郋�[�v
	{
		if (GetAsyncKeyState(VK_SPACE) != 0)
		{
			if (i == TIMES)
			{
				break;
			}

			cout << sti[Count] << endl;
			while (GetAsyncKeyState(VK_SPACE) & 0x8000);
			i++;
		}
	}*/
}