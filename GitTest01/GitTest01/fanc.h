//define
//ウィンドウサイズ
#define window_x 1920
#define window_y 1080
//0.294mm/画素(RDT196M)
#define PITCH 0.294
//刺激位置 5deg
#define INTDEG 15
//視距離
#define OBDIST 500.0
#define DIR 8												//方向数
#define DISP_TIME 1000										//画面に刺激を提示する時間			参考・瞬目にかかる時間：100～150msとする
#define TOTAL_EXP_TIME (5 * 60000)							//実験時間：5m（300000ms）
#define TIMES 40											//刺激提示回数
#define EXP_TIME (TOTAL_EXP_TIME - (DISP_TIME * TIMES))		//動作を考慮した実験時間
#define BLANK_TIME 1500										//画面に何も表示しない時間


//構造体
typedef struct {
	int x; //刺激のx座標
	int y; //刺激のy座標
}Mark;


using namespace std;
Mark c[8];


void setArrayEquality(int preSti[TIMES]);				//刺激を均等に割り振る関数，　戻り値：刺激を割り振った配列
void stiTiming(int timing[TIMES - 1]);					//刺激表示関数
int disp(int sti[TIMES], int timing[TIMES - 1], int Count,cv::Mat img1,cv::Mat img2, cv::Mat img3, std::string);	//画面表示用関数（1ループ）
void show(int preSti[TIMES], int timing[TIMES - 1], int Count);		//画面表示用関数（全体）
cv::Mat make_window();									//ウィンドウ作成関数
cv::Mat make_cross(cv::Mat);							//十字作成関数
cv::Mat make_circle(cv::Mat img, int Count, int preSti[TIMES]);		//刺激点作成関数
void file_write(DWORD, std::string, int, int, int);				//ファイル書き込み関数
void mark();											//刺激点位置決定関数



void setArrayEquality(int preSti[TIMES])
{
	//int preSti[TIMES];			//刺激提示方向格納用配列
	bool sti[DIR] = { false };		//均等に割り振るための配列
	int i;
	int rand;						//乱数格納変数
	int c = DIR;					//stiがすべてtrueになっていないかチェックする変数

	cv::RNG rng(GetTickCount());


	for (i = 0; i < TIMES; i++)		//方向格納配列に値を入れる
	{
		rand = rng.uniform( 0, 8);
		rand %= DIR;				//0～DIR-1の値が入る

		if (c <= 0)					//sti配列がすべてtrueのとき
		{
			for (c; c < DIR; c++)	//配列をすべてfalseに戻す
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
				rand = rng.uniform(0, 8);
				rand %= DIR;				//0～DIR-1の値が入る
			}
		}
		preSti[i] = rand;			//配列に値を入れる
	}
}



void stiTiming(int timing[TIMES - 1])				//時間はms単位で考える
{
	//int timing[TIMES - 1];					//刺激表示タイミング格納配列
	int i = 0, j;
	int tim = EXP_TIME / (TIMES - 1);		//均等にされたタイミング
	int pair1, pair2;						//タイミング調整に使う、時間を調整するペアを指定するための変数
	int repl_time;							//ペアで足し引きされる時間
	cv::RNG rng(GetTickCount());			//乱数生成用
	int max_repl = tim + (tim / 2);			//変化させる時間の最大幅
	

	for (j = 0; j < TIMES - 1; j++)
	{
		timing[j] = tim;					//タイミング配列に均等間隔のタイミングを代入する
	}
	for (j = 0; j < 50; j++)
	{
		repl_time = rng.uniform(1, tim / 2);
		do
		{
			pair1 = rng.uniform(0, TIMES - 1);
		} while (timing[pair1] + repl_time > max_repl);						//変化量を計算し、変化させる時間の最大幅を越えないような添え字にする

		do
		{
			pair2 = rng.uniform(0, TIMES - 1);
		} while (timing[pair2] - repl_time < BLANK_TIME || pair1 == pair2);			//同上 + 同じ添え字にならないようにする

		timing[pair1] += repl_time;			//実際に代入する
		timing[pair2] -= repl_time;
	}
}



int disp(int sti[TIMES], int timing[TIMES - 1],volatile int Count, cv::Mat img1, cv::Mat img2, cv::Mat img3, std::string fname)
{
	LARGE_INTEGER nFreq, nBefore, nAfter;	//キー入力用変数
	DWORD dwTime = 0;
	int x;									//キー取得変数
	int elap;

	memset(&nFreq, 0x00, sizeof nFreq);		//変数初期化
	memset(&nBefore, 0x00, sizeof nBefore);
	memset(&nAfter, 0x00, sizeof nAfter);


	QueryPerformanceFrequency(&nFreq);

	//system("cls");							//画面をクリアする
	//cout << sti[Count] << endl;				//配列出力
	cv::imshow("image", img2);				//画面表示

	QueryPerformanceCounter(&nBefore);
	x = cv::waitKey(DISP_TIME);				//刺激提示時間分キー入力を待つ
	//x = cv::waitKey(0);		//debug
	elap = (int)((nAfter.QuadPart - nBefore.QuadPart) * 1000 / nFreq.QuadPart);
	if (x == 32)		//27　=　エスケープキー　，32　=　スペースキー　，-1　=　エラー
	{
		QueryPerformanceCounter(&nAfter);	//nAfterにキーを押された時のパフォーマンスカウンタの値を取得
		dwTime = (DWORD)((nAfter.QuadPart - nBefore.QuadPart) * 1000 / nFreq.QuadPart);			//dwTimeにnAfter-nBeforeの値を入れる(ms)
		file_write(dwTime, fname, sti[Count], timing[Count], Count);			//ファイルに書き込む
	}
	else if (x == 27)		//ループを抜ける
	{
		return -1;
	}

	while (elap < DISP_TIME - dwTime)		//処理を止めずに時間を計る
	{
		QueryPerformanceCounter(&nAfter);
		elap = (int)((nAfter.QuadPart - nBefore.QuadPart) * 1000 / nFreq.QuadPart);

		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)			//ループを抜ける
		{
			return -1;
		}
	}

	//画面をクリアする
	cv::imshow("image", img3);				//画面表示
	cv::waitKey(10);
	if (x == 27)			//ループを抜ける
	{
		return -1;
	}
	//次までの表示タイミング分キー入力を待つ
	QueryPerformanceFrequency(&nFreq);
	QueryPerformanceCounter(&nBefore);
	QueryPerformanceCounter(&nAfter);
	elap = (int)((nAfter.QuadPart - nBefore.QuadPart) * 1000 / nFreq.QuadPart);
	while (elap < BLANK_TIME - 10)		//処理を止めずに時間を計る
	{
		QueryPerformanceCounter(&nAfter);
		elap = (int)((nAfter.QuadPart - nBefore.QuadPart) * 1000 / nFreq.QuadPart);

		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)			//ループを抜ける
		{
			return -1;
		}
	}
	//system("cls");							//画面をクリアする
	cv::imshow("image", img1);				//画面表示
	cv::waitKey(10);
	if (x == 27)			//ループを抜ける
	{
		return -1;
	}
	//次までの表示タイミング分キー入力を待つ
	QueryPerformanceFrequency(&nFreq);
	QueryPerformanceCounter(&nBefore);
	QueryPerformanceCounter(&nAfter);
	elap = (int)((nAfter.QuadPart - nBefore.QuadPart) * 1000 / nFreq.QuadPart);
	while (elap < timing[Count] - 10)		//処理を止めずに時間を計る
	{
		QueryPerformanceCounter(&nAfter);
		elap = (int)((nAfter.QuadPart - nBefore.QuadPart) * 1000 / nFreq.QuadPart);

		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)			//ループを抜ける
		{
			return -1;
		}
	}
	return 0;
}



void file_write(DWORD dwTime, std::string fname, int dir, int timing, int Count) {
	
	std::ofstream fs(fname, std::ios::app);		//ファイルを開く

	fs << Count << "," << dwTime << "," << dir << "," << timing << endl;			//ファイルにms単位で書き込む

	fs.close();									// ファイルを閉じる
}



//画面表示
void show(int preSti[TIMES], int timing[TIMES - 1], int Count) {

	static int fin = 0;			//終了フラグ
	std::string subnm;			// 実験参加者名

	cout << "---刺激変化検出実験3---" << endl;
	cout << "実験参加者=";
	cin >> subnm;
	cout << "画面中央に十字が表示されるのでそこを注視してください。" << endl << endl <<"注視点(十)が表示された数秒後に刺激が表示されます。" << endl;
	cout << "約" << TOTAL_EXP_TIME / 60000 << "分間の実験です。" << endl;
	cout << "刺激は" << TIMES << "回提示されます。" << endl;
	getchar();
	getchar();

	// 回答情報記録準備
	std::string fname;
	//ファイル名の生成
	fname = "実験3_" + subnm + ".csv";

	//ウィンドウ作成
	//注視点(十字)表示ウィンドウ
	cv::Mat img1 = make_window();
	//注視点(十字)、刺激点(円)表示ウィンドウ
	cv::Mat img2 = make_window();
	//まっさらな画像表示ウィンドウ
	cv::Mat img3 = make_window();

	setArrayEquality(preSti);		//位置指定配列作成
	stiTiming(timing);				//時間指定配列作成

	//注視点作成
	img1 = make_cross(img1);
	img2 = make_cross(img2);

	//刺激点位置作成
	mark();

	//カーソルを消す
	ShowCursor(false);

	cv::imshow("image", img1);				//画面表示
	cv::waitKey(50);
	Sleep(4000);
	//画面表示用ループ
	while (1) {

		//刺激切り替え
		img2 = make_circle(img2, Count, preSti);

		//画面表示
		fin = disp(preSti, timing, Count, img1, img2, img3, fname);

		Count++;

		//実験回数が規定回数を超えたらループを抜ける
		if (Count >= TIMES)
		{
			break;
		}
		if (fin == -1)
		{
			break;
		}
	}


	//終了処理
	// すべてのwindowを破棄
	cv::destroyAllWindows();
}



//ウィンドウ作成
cv::Mat make_window() {
	HWND handle;		//ウィンドウハンドル用変数
	//imgの初期化
	cv::Mat img(cv::Size(window_x, window_y), CV_8UC3, cv::Scalar(255, 255, 255));
	//ウィンドウへの名前付け
	cv::namedWindow("image", CV_WINDOW_AUTOSIZE);
	// ウィンドウを中央に移動
	cv::moveWindow("image", 0, 0);
	handle = GetActiveWindow();			//ウィンドウハンドル取得
	SetForegroundWindow(handle);		//アクティブウィンドウに変更
	return img;
}



//十字を作成する関数
cv::Mat make_cross(cv::Mat img) {
	//十字(注視点作成)
	cv::putText(img, "+", cv::Point(window_x / 2, window_y / 2), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0, 0, 0), 3, CV_AA);
	return img;
}



//刺激点作成(int配列)
cv::Mat make_circle(cv::Mat img, int Count, int preSti[TIMES]) {

	int j, k;
	//刺激表示位置取り出し
	k = preSti[Count];

	//前回の刺激を削除
	for (j = 0; j < 8; j++) {
		cv::circle(img, cv::Point(c[j].x, c[j].y), 20, cv::Scalar(255, 255, 255), -1, CV_AA);
	}
	//新たな刺激作成
	cv::circle(img, cv::Point(c[k].x, c[k].y), 10, cv::Scalar(200, 0, 0), -1, CV_AA);

	return img;
}



//刺激点位置決定
void mark() {
	double dtmp;
	int dpix;
	int i, j, k = 0;

	// 刺激配置の算出
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