#define DIR 8												//方向数
#define TIMES 40											//刺激提示回数
#define DISP_TIME 1250										//画面に刺激を提示する時間			参考・瞬目にかかる時間：100～150msとする
//#define EXP_TIME (300000 - (DISP_TIME * TIMES))				//実験時間：300s（300000ms）
#define EXP_TIME (120000 - (DISP_TIME * TIMES))									//実験時間：120s（120000ms）		試験用


//void test000(void);
void setArrayEquality(int preSti[TIMES]);		//刺激を均等に割り振る関数，　戻り値：刺激を割り振った配列
void stiTiming(int sti[TIMES], int timing[TIMES - 1]);					//刺激表示関数
void disp(int sti[TIMES], int timing[TIMES - 1], int Count);							//画面表示用関数，　引数：表示した回数

/*void test000(void) {
	cout << "test1" << endl;
}*/

void setArrayEquality(int preSti[TIMES])
{
	//int preSti[TIMES];				//刺激提示方向格納用配列
	bool sti[DIR] = { false };		//均等に割り振るための配列
	int i;
	int rand;						//乱数格納変数
	int c = DIR;					//stiがすべてtrueになっていないかチェックする変数

	cv::RNG rng(GetTickCount());

	//cout << "乱数取得開始" << endl;		//debug

	for (i = 0; i < TIMES; i++)		//方向格納配列に値を入れる
	{
		rand = rng.uniform( 0, 8);
		rand %= DIR;				//0～DIR-1の値が入る

		if (c <= 0)						//sti配列がすべてtrueのとき
		{
			for (c; c < DIR; c++)
			{
				sti[c] = false;
			}
		}

		while (1)
		{
			if (sti[rand] == false)			//sti[rand]がfalseならtrueに変えてループを出る
			{
				sti[rand] = true;
				c--;
				break;
			}
			else							// randを再取得してまたループする
			{
				//rand = (rand + DIR + 1) % DIR;
				rand = rng.uniform(0, 8);
				rand %= DIR;				//0～DIR-1の値が入る
			}
		}
		preSti[i] = rand;			//配列に値を入れる
		
		//cout << rand << endl;		//debug
		//cout << preSti[i] << endl;	//debug
	}

	//cout << "乱数取得完了" << endl;		//debug
}

void stiTiming(int sti[TIMES], int timing[TIMES - 1])				//時間はms単位で考える
{
	int i = 0, j;
	//int timing[TIMES - 1];					//刺激表示タイミング格納配列
	int tim = EXP_TIME / (TIMES - 1);		//均等にされたタイミング
	int pair1, pair2;						//タイミング調整に使う、時間を調整するペアを指定するための変数
	int repl_time;							//ペアで足し引きされる時間
	cv::RNG rng(GetTickCount());			//乱数生成用
	int max_repl = tim + (tim / 2);			//変化させる時間の最大幅
	LARGE_INTEGER nFreq, nBefore, nAfter;
	int elap;

	int sum = 0;							//debug

	setArrayEquality(sti);

	memset(&nFreq, 0x00, sizeof nFreq);
	memset(&nBefore, 0x00, sizeof nBefore);
	memset(&nAfter, 0x00, sizeof nAfter);

	for (j = 0; j < TIMES - 1; j++)
	{
		timing[j] = tim;					//タイミング配列に均等間隔のタイミングを代入する
	}
	for (j = 0; j < 100; j++)
	{
		repl_time = rng.uniform(1, tim / 2);
		do
		{
			pair1 = rng.uniform(0, TIMES - 1);
		} while (timing[pair1] + repl_time > max_repl);						//変化量を計算し、変化させる時間の最大幅を越えないような添え字にする

		do
		{
			pair2 = rng.uniform(0, TIMES - 1);
		} while (timing[pair2] - repl_time < 0 || pair1 == pair2);			//同上、同じ添え字にならないようにする

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
	
void disp(int sti[TIMES], int timing[TIMES - 1], int Count)				//画面表示
{
	LARGE_INTEGER nFreq, nBefore, nAfter;
	int elap;

	system("cls");														//画面をクリアする
	cout << sti[Count] << endl;												//画面表示
	//Sleep(DISP_TIME);
	QueryPerformanceFrequency(&nFreq);
	QueryPerformanceCounter(&nBefore);
	QueryPerformanceCounter(&nAfter);
	elap = (int)((nAfter.QuadPart - nBefore.QuadPart) * 1000 / nFreq.QuadPart);
	while (elap < DISP_TIME)			//処理を止めずに時間を計る
	{
		QueryPerformanceCounter(&nAfter);
		elap = (int)((nAfter.QuadPart - nBefore.QuadPart) * 1000 / nFreq.QuadPart);
	}
	system("cls");
	//Sleep(timing[i]);													//間隔分動作を停止する
	QueryPerformanceFrequency(&nFreq);
	QueryPerformanceCounter(&nBefore);
	QueryPerformanceCounter(&nAfter);
	elap = (int)((nAfter.QuadPart - nBefore.QuadPart) * 1000 / nFreq.QuadPart);
	while (elap < timing[Count])			//処理を止めずに時間を計る
	{
		QueryPerformanceCounter(&nAfter);
		elap = (int)((nAfter.QuadPart - nBefore.QuadPart) * 1000 / nFreq.QuadPart);
	}


	/*while (1)								//スペースキーを押すと表示されるループ
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