#pragma once
#define PAI 3.14159265359

//BL検出用のパラメータ
#define FR_DUR	 5	  // 直近5フレーム間の平均値と比較する
#define CANNY_TH1 120 // cannyオペレータしきい値１
#define CANNY_TH2  60 // cannyオペレータしきい値２  

//Display諸元( RDT196LM ,三菱)
#define DH_SIZE 1280
#define DV_SIZE 1024


// VGA画面サイズ
#define H_SIZE 640
#define V_SIZE 480

//CB確認実験における刺激提示画面サイズ(使用せず）
//#define SW_HSIZE 300
//#define SW_VSIZE 300


#define PITCH 0.294    // 0.294mm/画素（RDT196M)
#define INTDEG 5       // 刺激配置 5deg
#define OBDIST 500.0   // 視距離


#define TR_NUM 48          // CB実験試行回数(24の倍数とする）実験１：随意性
#define TR_NUM4 (48*4)     // 実験２：自発性における最大試行回数
#define MAX_FRAME_NUM 2000 // しきい値自動算出モードにおける最大フレーム数（30fps*60s=1800frame想定）
#define TH_TR_NUM 5       // しきい値を求めるための試行回数

using namespace std;

// 関数プロトタイプ
int save_basic_data(int[], int);
int save_basic_data2(int, int, std::string, int, int, int, int, int);

int set_ROI();
int bl_detect();
int bl_detect2();
int bl_detect_th();
int prep_cb_exp();
int prep_cb_exp2();
int prep_cb_exp3();
int cb_exp();
int cb_exp1_ssii();
int cb_exp2_ssii();
int cb_exp3_ssii();
int cb_exp1_ssii_ren();
int cb_exp2_ssii_ren();
int cb_exp3_ssii_ren();

int cb_exp1_c2016();
int cb_exp2_c2016();
int cb_exp1_c2016_ren();
int cb_exp2_c2016_ren();



int test();
int time_delay(int);

//int cb_exp3_ssii_ren();
int gen_stimuli_series();
int get_ij(int, int*, int*);
int analysis1();
int analysis2();
int check_volbl_occurence(int, int*, int, int, int, int, int);
int count_edge_sum(cv::Mat gray, int, int, int, int);
int determine_threshold(int);
int adjust_eye_position(int, int, int, int);

/*---グローバル変数 ---*/
int edge_sum[MAX_FRAME_NUM];
int av_edge_sum[MAX_FRAME_NUM];
int dff_edge_sum[MAX_FRAME_NUM];
int itmp[MAX_FRAME_NUM];  // 作業用
int itmp2[MAX_FRAME_NUM]; // 作業用
int eltm[MAX_FRAME_NUM];  // 
int sum_diff[MAX_FRAME_NUM]; //

int bl_th_val; // BL検出用閾値（エッジ差分値）

double startime, stoptime, curtime;

// タイマー用(QueryPerformanceConter)
LARGE_INTEGER nFreq, nBefore, nAfter;

//使用するカメラの設定
int camera_id = 0; //0番のカメラを使用

				   // DirectShowによるキャプチャ設定
cv::VideoCapture cap(CV_CAP_DSHOW + camera_id);

//マウス用グローバル変数
cv::Rect box; // 処理領域格納用
cv::Mat gframe;
bool drawing_box = false;


//マウス用矩形描画関数
void draw_box(cv::Mat* img, cv::Rect rect) {
	cv::rectangle(*img, cv::Point2d(box.x, box.y), cv::Point2d(box.x + box.width, box.y + box.height),
		cv::Scalar(0xff, 0x00, 0x00));
}

// マウス情報取得用コールバック関数
void my_mouse_callback(int event, int x, int y, int flags, void* param) {
	cv::Mat* image = static_cast<cv::Mat*>(param);

	switch (event) {
	case cv::EVENT_MOUSEMOVE:
		if (drawing_box) {
			box.width = x - box.x;
			box.height = y - box.y;
		}
		break;

	case cv::EVENT_LBUTTONDOWN:
		drawing_box = true;
		box = cv::Rect(x, y, 0, 0);
		break;

	case cv::EVENT_LBUTTONUP:
		drawing_box = false;
		if (box.width < 0) {
			box.x += box.width;
			box.width *= -1;
		}
		if (box.height < 0) {
			box.y += box.height;
			box.height *= -1;
		}
		draw_box(image, box);
		break;
	}
}

//種々確認用
/*--------*/
/* test   */
/*--------*/
//
// canny オペレータの実行時間算出

int test()
{
	int64 start;
	int dur, i;
	cv::Mat frame, gray;

	int canny_th1 = 120;
	int canny_th2 = 60;

	cap >> frame;
	start = cvGetTickCount();
	for (i = 0; i < 10000; i++) {
		cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);	// カラー画像をモノクロ画像に変換
		cv::Canny(gray, gray, canny_th1, canny_th2, 3); // キャニーオペレータによるエッジ抽出
	}
	dur = (cvGetTickCount() - start) / (1000.0*cvGetTickFrequency());
	//cv::imwrite("test.bmp", gray);
	cout << dur << endl;
	return 1;

}





/*
int test()
{

time_delay(1000);
cout << "OK";
return 1;
}
*/


/*
int test()
{
// 高精度タイマの動作確認 2016.5.4

LARGE_INTEGER nFreq, nBefore, nAfter;
DWORD dwTime;

//変数の初期化
memset(&nFreq, 0x00, sizeof nFreq);
memset(&nBefore, 0x00, sizeof nBefore);
memset(&nAfter, 0x00, sizeof nAfter);

dwTime = 0;

QueryPerformanceFrequency(&nFreq);
QueryPerformanceCounter(&nBefore);
Sleep(100); // 100ms遅延
QueryPerformanceCounter(&nAfter);

dwTime = (DWORD)((nAfter.QuadPart - nBefore.QuadPart) * 1000 / nFreq.QuadPart);
cout << dwTime << endl;
getchar();
getchar();
return 1;

}
*/

/*
int test()
{
cv::Mat arrow;
cv::namedWindow("test", CV_WINDOW_AUTOSIZE); // ウィンドウの生成
arrow = cv::imread("right_dir.bmp", cv::IMREAD_GRAYSCALE);
cv::imshow("test", arrow);
cout << "幅："<<arrow.cols << " 高さ:"<<arrow.rows << endl;
cv::waitKey(10 * 1000); // 5s待つ
return 1;
}
*/

/*-----------------------*/
/*  BL検出処理領域の設定 */
/*-----------------------*/
//　領域が設定される変数
// is:box.x, ie:box.x+box.width
// js:box.y, je:box.y+boy.width
//
int set_ROI()
{

	std::string name = "Box Example";  // ウィンドウの名称
	box = cv::Rect(-1, -1, 0, 0);      // 処理領域の初期値

	cout << endl;
	cout << "処理領域の設定を行います" << endl;
	cout << "終わるときはESCキーを押してください" << endl;

	// カメラのチェック
	if (!cap.isOpened()) {
		cerr << "カメラが見つかりません" << endl;
		return 0;
	}

	cv::namedWindow(name, CV_WINDOW_AUTOSIZE); // ウィンドウの生成
	cv::moveWindow(name, (1280 - 640) / 2, (1024 - 480) / 2);// ウィンドウを中央に移動

															 // コールバックを設定
	cv::setMouseCallback(name, my_mouse_callback, (void *)&gframe);

	while (1) {
		cap >> gframe;

		// マウスの左クリックを離すまでの間、矩形を一時的に描画
		if (drawing_box) {
			draw_box(&gframe, box);
		}

		cv::imshow(name, gframe);

		// Escで終了
		if (cv::waitKey(5) == 27)
			break;
	}
	//領域の描画
	draw_box(&gframe, box);
	cv::imshow(name, gframe);
	cv::waitKey(5 * 1000); // 5s待つ

	cout << endl;
	cout << "is=" << box.x << "ie=" << box.x + box.width << endl;
	cout << "js=" << box.y << "je=" << box.y + box.height << endl;


	cv::destroyWindow(name); // ウィンドウの破棄

	return 1;
}

// 2016.5.2～
/*---------------------------*/
/* BL検出用しきい値自動設定  */
/*---------------------------*/
// 
// エッジ抽出：canny operator,しきい値固定
//
// 検出範囲を設定してから実行する
// 5回瞬きを行ってもらう
// 実験参加者毎の基礎データを記録しておく
// ５回分の閾値は一時ファイルとしてデフォルトフォルダに格納する.実験時に読み出す
// ファイル名：threshold.csv　
//
//
int bl_detect_th()
{
	cv::Mat frame, gray;

	int canny_th1, canny_th2, bl_th, result, exp_num, itmp;
	int fr, frnm, tr;
	int i, j, is, js, ie, je;
	int cnt, th, blth[TH_TR_NUM];
	int bl_th_max;
	int64 start, elptim;
	std::string subnm;     // 実験参加者名
	std::string msg1, msg2; // 表示メッセージ
	std::string moji;


	cout << "実験参加者=";
	cin >> subnm;

	cout << "実験番号(1-4)=";
	cin >> exp_num;

	// 処理範囲の設定
	is = box.x;
	js = box.y;
	ie = box.x + box.width;
	je = box.y + box.height;

	if (is == -1) {
		cout << "検出範囲が設定されていません";
		return 0;
	}

	cout << "しきい値を自動的に設定します" << endl;

	// 検出範囲の表示
	cout << "is=" << is << "ie=" << ie << endl;
	cout << "js=" << js << "je=" << je << endl;

	// 画像表示用ウィンドウの生成(原画像）
	char *window_input = "input";
	cv::namedWindow(window_input, CV_WINDOW_AUTOSIZE);

	// 画像表示用ウィンドウの生成(エッジ画像）
	char *window_output = "output";
	cv::namedWindow(window_output, CV_WINDOW_AUTOSIZE);

	canny_th1 = CANNY_TH1; // 信頼性の高いエッジの閾値（固定値とする）
	canny_th2 = CANNY_TH2; // 信頼性に低いエッジの閾値（固定値とする）

						   // しきい値格納用一時ファイル（実験時に読み出す）デフォルトのフォルダに生成	
	ofstream fout("threshold.csv"); // 出力ファイルを作成する
	if (!fout) {
		cout << "threshold.csvファイルが開きません" << endl;
		return 0;
	}

	fout << "処理範囲" << endl;
	fout << is << endl;
	fout << js << endl;
	fout << ie << endl;
	fout << je << endl;

	fout << "しきい値" << endl;
	for (tr = 0; tr <TH_TR_NUM; tr++) {   // 現在5回分のデータを取得する
										  // コンソールへの表示
		cout << tr + 1 << "回目の計測を行います" << endl;
		cout << "何かキーを押して,1秒後に軽く瞬きしてください" << endl << endl;
		// ウインドウへの表示
		msg1 = "Starting " + to_string(tr + 1) + "th trial";
		msg2 = "Hit any key ! and blinking after 1 or 2sec.";
		cv::Mat background(cv::Size(640, 480), CV_8UC3, cv::Scalar(255, 255, 255)); // Windowに表示する背景の準備
		cv::putText(background, msg1, cv::Point(10, 100), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 0, 0), 1, CV_AA);
		cv::putText(background, msg2, cv::Point(10, 150), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 0, 0), 1, CV_AA);
		cv::imshow(window_input, background); // 原画像の表示(WaitKey()を受け付けるため)
		cv::imshow(window_output, background);

		result = cv::waitKey(10 * 1000); // 最大10s待つ
		if (result == -1) {// 10秒間キー入力されなかったらやり直し
			tr--;
			continue;
		}

		// キー入力あり:記録スタート(5秒分）
		fr = 0;
		sum_diff[0] = 0; // スタートフレームは0
		start = cvGetTickCount(); // 開始時刻
		while (1) {
			eltm[fr] = (cvGetTickCount() - start) / (1000.0*cvGetTickFrequency());
			if (eltm[fr] >= 5 * 1000) { // 経過時間5s以上なら映像取得終了
				break;
			}
			cap >> frame;
			cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);	// カラー画像をモノクロ画像に変換
			cv::Canny(gray, gray, canny_th1, canny_th2, 3); // キャニーオペレータによるエッジ抽出
			cv::rectangle(frame, cv::Point(is, js), cv::Point(ie, je), cv::Scalar(0, 0, 255), 1, 1);    // 関心領域を描画する
			cv::rectangle(gray, cv::Point(is, js), cv::Point(ie, je), cv::Scalar(255, 255, 255), 1, 1); // 関心領域を描画する
			cv::imshow(window_input, frame); // 原画像の表示
			cv::imshow(window_output, gray); // エッジ検出画像
			cv::waitKey(10);                 // 画像表示 
			edge_sum[fr] = count_edge_sum(gray, is, js, ie, je);           // エッジ総量
																		   //if (fr != 0) sum_diff[fr] = edge_sum[fr] - edge_sum[fr - 1]; // 隣接間フレーム差分(記録せず2016.5.3)
			fr++;
		}//while(1)
		frnm = fr;                             // 総フレーム数(5s間）
		bl_th_val = determine_threshold(frnm); // しきい値算出(bl_th_val:外部変数）
		if (bl_th_val == 0) {                   // しきい値取得失敗、やり直し
			tr--;

			continue;
		}
		blth[tr] = bl_th_val;//コンソール確認用に保持
		fout << bl_th_val << endl;                           // しきい値の格納
		save_basic_data2(tr, frnm, subnm, exp_num, is, js, ie, je); // 基礎データの書き込み
	}//for(tr=
	fout.close();
	cv::destroyAllWindows(); // すべてのwindowを破棄
	cout << endl << "----しきい値の確認---" << endl;
	for (i = 0; i < TH_TR_NUM; i++) {
		cout << blth[i] << endl;
	}
	cout << "確認しましたか(y)" << endl;
	cin >> moji;
	return 1;
}


// 2016.5.3
/*--------------*/
/* 閾値自動算出 */
/*--------------*/
//
// 外部変数使用 
//　edge_sum[],av_edge_sum[],dff_edge_sum[]
//　戻り値：しきい値，0のときはうまく抽出できず
// 
//
int determine_threshold(
	int frnum      // 総フレーム数
)
{
	int fr, i, th;
	int fr_min, fr_max, dff_min, dff_max, dff_av;

	// エッジ総量の平均化処理・差分処理
	for (fr = FR_DUR; fr < frnum; fr++) {
		th = 0;
		for (i = fr - 1; i > fr - 1 - FR_DUR; i--) {// 過去5フレーム分（現フレームを含まない）の平均値
			th += edge_sum[i]; // エッジ点数の積算
		}
		th /= FR_DUR;
		av_edge_sum[fr] = th;                 // 移動平均値
		dff_edge_sum[fr] = edge_sum[fr] - th; // 移動平均値との差分
	}

	// 差分値の最大・最小処理
	dff_av = 0; dff_min = 10000; dff_max = 0;
	for (fr = FR_DUR; fr < frnum; fr++) {
		if (dff_edge_sum[fr] < dff_min) { // 差分値の最小値検索
			dff_min = dff_edge_sum[fr];
			fr_min = fr;
		}

		if (dff_edge_sum[fr] > dff_max) { // 差分値の最大値検索
			dff_max = dff_edge_sum[fr];
			fr_max = fr;
		}
		dff_av += abs(dff_edge_sum[fr]);
	}
	dff_av /= (frnum - FR_DUR); // 差分値の大きさの平均値(算出したが閾値設定では使用していない2016.5.3）

								// 閾値設定
	if (fr_max < fr_min) {
		cout << "しきい値算出エラー:瞬目でない動きの発生あり" << endl << endl;
		return 0;
	}

	if (fr_max - fr_min>10) {
		cout << "しきい値算出エラー：差分値の最小・最大が対をなしていない" << endl << endl;
		return 0;
	}
	return(0.8*dff_min); // 暫定的に最小値の８掛けを閾値とする
}

// 2016.5.3
/*--------------------------*/
/* エッジ強度総量のカウント */
/*--------------------------*/
//
//
int count_edge_sum(
	cv::Mat gray, // エッジ画像
	int is,       // 算出範囲
	int js,
	int ie,
	int je
)
{
	int i, j, cnt;
	cnt = 0;
	for (j = js; j < je; j++) {
		for (i = is; i < ie; i++) {
			if (gray.data[j * H_SIZE + i] == 255)
				cnt++;
		}
	}
	return cnt;
}


// 2016.5.2
/*-----------------------*/
/* 基本データの格納処理2 */
/*-----------------------*/
//
//
int save_basic_data2(
	int tr,            // 試行番号
	int blframe,       // BL生起フレーム
	std::string subnm, // 実験参加者
	int expnum,        // 実験番号
	int is,            // 処理範囲
	int js,
	int ie,
	int je
)
{
	int fr, result;
	std::string fname;

	fname = "c:\\IEEJ_C_2016\\";
	fname = fname + "exp" + std::to_string(expnum) + "\\" + subnm + "\\" + "threshold";
	fname = fname + "\\" + "th_data_tr" + std::to_string(tr) + ".csv";

	ofstream fout(fname); // 出力ファイルを作成する
	if (!fout) {
		cout << fname << "ファイルが開きません" << endl;
		return 0;
	}
	fout << "処理範囲" << endl;
	fout << is << endl;
	fout << js << endl;
	fout << ie << endl;
	fout << je << endl;
	fout << "しきい値" << endl;
	fout << bl_th_val << endl;
	//fout << "BL生起フレーム=" << blframe << endl;
	fout << "fr,時刻,フレーム間隔,エッジ総量,平均エッジ総量,差分値,検出結果" << endl;

	fout << 0 << "," << eltm[0] << "," << 0 << "," << edge_sum[0] << "," << 0 << "," << 0 << endl;

	for (fr = 1; fr < blframe; fr++) {
		result = 0;
		if (dff_edge_sum[fr] < bl_th_val)result = 1;
		fout << fr << "," << eltm[fr] << "," << eltm[fr] - eltm[fr - 1] << "," << edge_sum[fr] << "," << av_edge_sum[fr] << "," << dff_edge_sum[fr] << "," << result << endl;
	}
	fout.close();
	return 1;
}











// 2016.3.29～
/*-----------------*/
/* BL検出確認実験2  */
/*-----------------*/
// 撮影ユニット用の開発
// 検出範囲をまず設定する
// タイムアウト：１分(30fpsなら）
// 
//

int bl_detect2()
{
	cv::Mat frame, gray;

	int canny_th1, canny_th2, bl_th;
	int fr;
	int i, j, is, js, ie, je;
	int cnt, th;

	// double startime, stoptime; グローバル変数で宣言している。

	is = box.x;
	js = box.y;
	ie = box.x + box.width;
	je = box.y + box.height;

	if (is == -1) {
		cout << "検出範囲が設定されていません";
		return 0;
	}

	//検出範囲の設定
	cout << "is=" << is << "ie=" << ie << endl;
	cout << "js=" << js << "je=" << je << endl;

	//画像表示用ウィンドウの設定
	char *window_input = "input";
	cv::namedWindow(window_input, CV_WINDOW_AUTOSIZE);

	char *window_output = "output";
	cv::namedWindow(window_output, CV_WINDOW_AUTOSIZE);

	cout << "BL検出の確認実験に入ります" << endl;
	cout << "canny閾値1(信頼性の高いエッジ）=";
	cin >> canny_th1;

	cout << "canny閾値2(信頼性の低いエッジ）=";
	cin >> canny_th2;

	cout << "BL検出のための閾値=";
	cin >> bl_th;


	fr = 0; // フレーム番号の初期化
	startime = (double)cvGetTickCount(); // 開始時刻
	while (1) {
		fr++;
		if (fr >= 30 * 60) { // 1分以上(30fps)ならタイムアウト
			save_basic_data(edge_sum, fr); // 基本データの書き込み
			break;
		}

		cap >> frame; // 1フレーム読み込み

		cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);	// カラー画像をモノクロ画像に変換
		cv::Canny(gray, gray, canny_th1, canny_th2, 3); // キャニーオペレータによるエッジ抽出

		cv::rectangle(frame, cv::Point(is, js), cv::Point(ie, je), cv::Scalar(0, 0, 255), 1, 1);          // 関心領域を描画する
		cv::rectangle(gray, cv::Point(is, js), cv::Point(ie, je), cv::Scalar(255, 255, 255), 1, 1); // 関心領域を描画する

		cv::imshow(window_input, frame); // 原画像の表示
		cv::imshow(window_output, gray); // エッジ検出画像

										 // エッジ点個数のカウント
		cnt = 0;
		for (j = js; j < je; j++) {
			for (i = is; i < ie; i++) {
				if (gray.data[j * H_SIZE + i] == 255)
					cnt++;
			}
		}
		edge_sum[fr] = cnt;

		// BL判定処理
		if (fr < 30 * 10) { // 試行最初の10秒間は処理しない
			cv::waitKey(15);
			continue;
		}

		th = 0;
		for (i = fr - 1; i > fr - 1 - FR_DUR; i--) {// 過去5フレーム分の平均値
			th += edge_sum[i]; // エッジ点数の積算
		}
		th /= FR_DUR;
		itmp[fr] = th; // 確認用
		itmp2[fr] = edge_sum[fr] - th; // 確認用
		if (edge_sum[fr] - th < -bl_th) {		// 上眼瞼の下降時点の検出処理
			save_basic_data(edge_sum, fr);      // 基本データの格納処理
			cv::imwrite("closebl.bmp", frame);  // BL時の画像を記録
			break; // 提示終了
		}
		stoptime = (double)cvGetTickCount() - startime; // 終了時刻


		if (cv::waitKey(15) >= 0) { // 15ms内で、何かキーが押されたら終了
			break;
		}
	}//while(1)
	cv::destroyAllWindows(); // すべてのwindowを破棄
	return 1;
}











/*-----------------*/
/* BL検出確認実験  */
/*-----------------*/
// 検出範囲をまず設定する
// タイムアウト：１分(30fpsなら）
//
int bl_detect()
{
	cv::Mat frame, gray;

	int canny_th1, canny_th2, bl_th;
	int fr;
	int i, j, is, js, ie, je;
	int cnt, th;

	// double startime, stoptime; グローバル変数で宣言している。

	is = box.x;
	js = box.y;
	ie = box.x + box.width;
	je = box.y + box.height;

	if (is == -1) {
		cout << "検出範囲が設定されていません";
		return 0;
	}

	//検出範囲の設定
	cout << "is=" << is << "ie=" << ie << endl;
	cout << "js=" << js << "je=" << je << endl;

	//画像表示用ウィンドウの設定
	char *window_input = "input";
	cv::namedWindow(window_input, CV_WINDOW_AUTOSIZE);

	char *window_output = "output";
	cv::namedWindow(window_output, CV_WINDOW_AUTOSIZE);

	cout << "BL検出の確認実験に入ります" << endl;
	cout << "canny閾値1(信頼性の高いエッジ）=";
	cin >> canny_th1;

	cout << "canny閾値2(信頼性の低いエッジ）=";
	cin >> canny_th2;

	cout << "BL検出のための閾値=";
	cin >> bl_th;


	fr = 0; // フレーム番号の初期化
	startime = (double)cvGetTickCount(); // 開始時刻
	while (1) {
		fr++;
		if (fr >= 30 * 60) { // 1分以上(30fps)ならタイムアウト
			save_basic_data(edge_sum, fr); // 基本データの書き込み
			break;
		}

		cap >> frame; // 1フレーム読み込み

		cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);	// カラー画像をモノクロ画像に変換
		cv::Canny(gray, gray, canny_th1, canny_th2, 3); // キャニーオペレータによるエッジ抽出

		cv::rectangle(frame, cv::Point(is, js), cv::Point(ie, je), cv::Scalar(0, 0, 255), 1, 1);          // 関心領域を描画する
		cv::rectangle(gray, cv::Point(is, js), cv::Point(ie, je), cv::Scalar(255, 255, 255), 1, 1); // 関心領域を描画する

		cv::imshow(window_input, frame); // 原画像の表示
		cv::imshow(window_output, gray); // エッジ検出画像

										 // エッジ点個数のカウント
		cnt = 0;
		for (j = js; j < je; j++) {
			for (i = is; i < ie; i++) {
				if (gray.data[j * H_SIZE + i] == 255)
					cnt++;
			}
		}
		edge_sum[fr] = cnt;

		// BL判定処理
		if (fr < 30 * 10) { // 試行最初の10秒間は処理しない
			cv::waitKey(15);
			continue;
		}

		th = 0;
		for (i = fr - 1; i > fr - 1 - FR_DUR; i--) {// 過去5フレーム分の平均値
			th += edge_sum[i]; // エッジ点数の積算
		}
		th /= FR_DUR;
		itmp[fr] = th; // 確認用
		itmp2[fr] = edge_sum[fr] - th; // 確認用
		if (edge_sum[fr] - th < -bl_th) {		// 上眼瞼の下降時点の検出処理
			save_basic_data(edge_sum, fr);      // 基本データの格納処理
			cap >> frame;
			cv::imwrite("closebl.bmp", frame);  // BL時の画像を記録
			break; // 提示終了
		}
		stoptime = (double)cvGetTickCount() - startime; // 終了時刻


		if (cv::waitKey(15) >= 0) { // 15ms内で、何かキーが押されたら終了
			break;
		}
	}//while(1)
	cv::destroyAllWindows(); // すべてのwindowを破棄
	return 1;
}

/*----------------------*/
/* 基本データの格納処理 */
/*----------------------*/
//
// fr=1800のときはBL検出できず
//
int save_basic_data(
	int esum[],  // エッジ点数
	int blframe  // BL生起フレーム
)
{
	int fr;
	ofstream fout("edgecount.csv"); // 出力ファイルを作成する
	if (!fout) {
		cout << "ファイルが開きません" << endl;
		return 0;
	}
	fout << "BL生起フレーム=" << blframe << endl;
	fout << "fr,エッジ個数,閾値,差分値" << endl;

	for (fr = 1; fr <= blframe; fr++) {
		fout << fr << "," << esum[fr] << "," << itmp[fr] << "," << itmp2[fr] << endl;
	}

	//つけたし（後で消しておく）
	fout << stoptime / ((double)cvGetTickFrequency()*1000.) << endl;

	fout.close();
	return 1;
}


/****************************************************/
/*                C部門2016実験用　                 */
/****************************************************/
//
//



//2016.4.24～ for C部門2016
/*-------------------------------*/
/* 刺激変化検出実験2             */
/* 自発性瞬目                    */
/*-------------------------------*/
//
//  練習版
//
//  C部門2016用
//  自発性BLのタイミングで切り替え
//　切り替え時点の目領域映像を記録
//
//  実験参加者1名あたり、48×4試行分用意している。
//　　ex.実験参加者:test → test1,test2,test3,test4のフォルダ(test内のサブフォルダ）に48試行ずつ展開している
//
//　TR_NUM回分、自発性BLのデータが取得できた時点で、実験を終了する。
//　最大実験時間は40分間とする。時間が過ぎたら強制終了する。
//
//

int cb_exp2_c2016_ren()
{
	static int st[TR_NUM4][8]; // 各試行において8箇所の周辺位置に提示する刺激番号(0,1,2,3)　
	static int chpos[TR_NUM4]; // 各試行において刺激が変化する周辺位置(0：左上隅,1,・・・,7：右下隅)
	static int prst[TR_NUM4];  // 変化前の刺激番号
	static int chst[TR_NUM4];  // 変化時に提示する刺激番号
	static int intvl[TR_NUM4]; // 最大刺激変化時間間隔

	int tr, fn;
	int basei, basej, dpix;
	int px, py;
	int i, j, k, is, js, ie, je;
	int k0, k1;
	int answer, blcnt;
	int itmp1, itmp2, itmp3, itmp4, itmp5;
	int th[5], bl_th_val;
	int cur_tim, tim_min, tim_sec, tim_msec;
	double dtmp;
	char split;

	int64 exp_start;  // 実験開始時刻
	int64 exp_eltim;  // 実験経過時間
	int   tr_eltim = 0; // １試行開始後の経過時間

	std::string subnm; // 実験参加者名
	std::string sbuf, sbuf1, sbuf2, sbuf3, sbuf4;  // バッファ
	std::string fns, bfname;
	cv::Mat frame, gray;


	cout << "---刺激変化検出実験2(練習）---" << endl;

	cout << "実験参加者=(renと入力）"; // 
	cin >> subnm;

	// BL検出範囲設定・しきい値ファイルの読み出し
	bfname = "threshold.csv";
	ifstream finb(bfname);
	if (!finb) {
		cout << "検出範囲用ファイルがありません" << bfname << endl;
		getchar();
		getchar();
		return 0;
	}

	// 設定範囲読み出し
	finb >> sbuf; // 1行分読み飛ばし
	finb >> is;
	finb >> js;
	finb >> ie;
	finb >> je;
	cout << "設定範囲" << endl;
	cout << is << "," << js << "," << ie << "," << je << endl;

	//しきい値の読み出し
	cout << "しきい値" << endl;
	finb >> sbuf; // 1行分読み飛ばし
	for (i = 0; i < TH_TR_NUM; i++) {
		finb >> th[i];
		cout << th[i] << endl;
	}
	finb.close();

	// しきい値設定(平均値とする）2016.5.4
	bl_th_val = 0;
	for (i = 0; i < TH_TR_NUM; i++) {
		bl_th_val += th[i];
	}
	bl_th_val /= TH_TR_NUM;
	cout << "BL検出しきい値=" << bl_th_val << endl;

	cout << "確認できたら何か入力してください" << endl;
	getchar();
	getchar();


	// 刺激提示順序データ読み込み
	std::string sfname;

	for (fn = 0; fn <= 3; fn++) { // フォルダ番号
		fns = std::to_string(fn + 1);
		sfname = "c:\\IEEJ_C_2016\\";
		sfname = sfname + "exp2" + "\\" + subnm + "\\" + subnm + fns + "\\order_";
		sfname = sfname + "実験2" + "_" + "練習_" + subnm + fns + ".csv";     // ファイル名の生成

																		  // 刺激提示順序データファイルの読み込み
		ifstream fin(sfname);
		if (!fin) {
			cout << "刺激提示用ファイルがありません" << sfname << endl;
			getchar();
			getchar();
			return 0;
		}

		// 変化情報の読み取り
		fin >> sbuf; // 1行分読み飛ばし

					 // 試行番号、変化位置、変化前刺激、変化後刺激,提示間隔
		for (tr = 0; tr < TR_NUM; tr++) {
			fin >> itmp1 >> split >> itmp2 >> split >> itmp3 >> split >> itmp4 >> split >> itmp5;
			chpos[tr + fn*TR_NUM] = itmp2;
			prst[tr + fn*TR_NUM] = itmp3;
			chst[tr + fn*TR_NUM] = itmp4;
			intvl[tr + fn*TR_NUM] = itmp5;
			//cout << tr+1 << ","<< chpos[tr] << "," << prst[tr] << "," << chst[tr]<< endl;//確認
		}

		// 提示パターン（周辺８箇所）
		fin >> sbuf; // 1行分読み飛ばし

		for (tr = 0; tr < TR_NUM; tr++) {
			fin >> itmp1;// 試行番号読み飛ばし
			for (i = 0; i < 8; i++) {
				fin >> split >> itmp1;
				st[tr + fn*TR_NUM][i] = itmp1;
			}
			fin >> split;
		}
		fin.close();
	}//for(fn=


	 // 提示情報の読み込み結果の確認
	cout << "試行番号,変化位置,現在刺激番号,変化後刺激番号,提示間隔" << endl;
	for (tr = 0; tr < TR_NUM4; tr++) {
		cout << tr + 1 << ":" << chpos[tr] << "," << prst[tr] << "," << chst[tr] << "," << intvl[tr] << endl;
	}
	cout << "試行番号,提示刺激（各周辺位置)" << endl;
	for (tr = 0; tr < TR_NUM4; tr++) {
		cout << tr + 1 << ":";

		for (i = 0; i < 8; i++) {
			cout << st[tr][i] << ",";
		}
		cout << endl;
	}


	//ここまでOK(2016.5.17)

	cout << "練習を始めますか：何かキーを押してください";
	getchar();
	getchar();


	// 試行開始準備

	// 回答情報記録準備
	std::string fname, imname, eyename;
	std::ostringstream ostr;

	fname = "c:\\IEEJ_C_2016\\";
	fname = fname + "exp2" + "\\" + subnm + "\\info_";
	fname = fname + "実験2" + "_" + "練習_" + subnm + ".csv"; // ファイル名の生成

														  // 回答情報出力用ファイルの生成
	ofstream fout(fname); // 出力ファイルを作成する
	if (!fout) {
		cout << "ファイルが開きません" << endl;
		getchar();
		getchar();
		return 0;
	}
	fout << "試行番号,変化時間,BL生起,変化位置" << endl;

	char *window_info = "info";
	//cv::namedWindow(window_info, CV_WINDOW_AUTOSIZE);							      　// 刺激提示用windowの生成
	//cv::moveWindow(window_info, (DH_SIZE - SW_HSIZE) / 2, (DV_SIZE - SW_VSIZE) / 2);  // 画面中央へ移動
	cv::Mat background(cv::Size(DH_SIZE, DV_SIZE), CV_8UC3, cv::Scalar(255, 255, 255)); // Windowに表示する背景の準備

																						//提示刺激の準備
	char mark[][10] = { "X", "O", "H", "E" }; // １バイト文字を選定


											  //文字提示位置の中心位置（注視点位置）
	basei = DH_SIZE / 2;
	basej = DV_SIZE / 2;

	// 刺激配置の算出
	dtmp = OBDIST*tan(INTDEG*PAI / 180.0);
	dtmp /= PITCH;
	dpix = (int)dtmp;

	exp_start = cvGetTickCount(); // 実験開始時刻の取得
	blcnt = 0;                   // BL生起試行数の初期化

								 // 実験開始!
	for (tr = 0; tr < TR_NUM4; tr++) { // 最大TR_NUM4回実験実施
		cv::Mat background(cv::Size(DH_SIZE, DV_SIZE), CV_8UC3, cv::Scalar(255, 255, 255)); // Windowに表示する背景の準備
		cv::imshow(window_info, background);  // 画像表示
		cv::waitKey(3 * 1000);                // 背景を3秒間提示

											  // 提示刺激の準備
		int pcnt = 0;
		for (j = -1; j <= 1; j++) {
			for (i = -1; i <= 1; i++) {
				if (i == 0 && j == 0) continue; // 画面中央は別の記号を提示するのでスキップ
				px = basei + i*dpix; // 今回の提示位置
				py = basej + j*dpix; // 今回の提示位置
				k = st[tr][pcnt];    // 提示位置pcntへ提示する刺激番号の設定
				cv::putText(background, (const char*)mark[k], cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0, 0, 0), 3, CV_AA);
				pcnt++; // 次の提示位置(0,1,2,・・・,7)
			}//for(i=
		}//for(j=

		 // 画面中央への記号（注視点提示）
		px = basei; // 提示位置(横）
		py = basej; // 提示位置(縦)
		cv::putText(background, "+", cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(128, 128, 128), 3, CV_AA);

		// 刺激提示
		//startime = (double)cvGetTickCount();  // 開始時刻の取得
		cv::imshow(window_info, background);    // 画像表示(この時点ではまだ提示開始されていない）
		cv::waitKey(30);                        // 刺激画像の提示

												//刺激提示画面の変化準備
		k0 = prst[tr]; // 変化前の刺激番号
		k1 = chst[tr]; // 変化後の刺激番号
		get_ij(chpos[tr], &i, &j); // 変化位置のi,j番号の取得
		px = basei + i*dpix;       // 変化位置の設定
		py = basej + j*dpix;       // 変化位置の設定

		int result = -1;
		result = check_volbl_occurence(intvl[tr] * 1000, &tr_eltim, bl_th_val, is, js, ie, je); // // 自発性BLのチェック(result=1:BL生起、result=0:タイムアウト)
		time_delay(15);    // 15ms待機し、瞼が閉じた状態のときに刺激を切り替える(このぐらいがよさそう）
		cv::putText(background, mark[k0], cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(255, 255, 255), 5, CV_AA); // 古い記号を消す
		cv::putText(background, mark[k1], cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0, 0, 0), 3, CV_AA);       // 新しい記号を描く
		cv::imshow(window_info, background);	// 記号表示更新
		cv::waitKey(5);                      // 即座に描画
		cap >> gframe;                          // 更新時点の目領域映像の取得
		cv::waitKey(3 * 1000);                  // 3秒待機

												// 目領域画像記録
		std::ostringstream ostr1;
		ostr1 << "_tr" << tr;

		//以下、動作確認していない → フォルダとファイル名(2016.4.13)
		eyename = "c:\\IEEJ_C_2016\\";
		eyename = eyename + "exp2" + "\\" + subnm + "\\eye_実験2_";
		eyename = eyename + subnm + ostr1.str() + ".bmp";

		cv::imwrite(eyename, gframe);   // BL時の画像を記録
		cv::destroyWindow(window_info); // windowの破棄

										//経過時間表示準備
		cur_tim = (cvGetTickCount() - exp_start) / (1000 * cvGetTickFrequency()); // ms単位
		tim_min = cur_tim / (60 * 1000);                // 分単位
		tim_sec = (cur_tim - tim_min * 60 * 1000) / 1000; // 秒単位


														  // 回答入力（コンソールで）
		cout << endl;
		cout << "経過時間：" << tim_min << "分" << tim_sec << "秒" << endl;
		cout << endl;
		cout << "試行番号:" << tr + 1 << "/" << TR_NUM4 << endl << endl;
		cout << "------------------------------------------" << endl;
		cout << "どこの位置が変化したか答えてください" << endl;
		cout << "1:左上段、2:中央上段、3:右上段" << endl;
		cout << "4:左中段、　　　　　　5:右中段" << endl;
		cout << "6:左下段、7:中央下段、8:右下段" << endl;
		cout << endl;
		cout << "9:わからない" << endl;
		cin >> answer;

		if (result == 1) { // 自発性BL生起
			fout << tr << "," << tr_eltim << "," << 0 << "," << answer << endl;
			blcnt++;//BL生起回数のカウントアップ
		}
		if (result == 0) { // 自発性BL生起せず、タイムアウト
			fout << tr << "," << tr_eltim << "," << -1 << "," << answer << endl;
		}

		// BL生起試行がTR_NUM回になった時点で実験終了
		if (blcnt >= TR_NUM) {
			break;
		}

		// 3分以上経過したら、途中でも実験終了
		exp_eltim = cvGetTickCount() - exp_start; // 実験経過時間算出

		if (exp_eltim / (1000 * cvGetTickFrequency()) > 3 * 60 * 1000) {
			break;
		}

		if (tr != 0 && (tr % 10) == 0) { // 試行10回毎に目領域位置調整
			adjust_eye_position(is, js, ie, je);
		}
	}//for(tr=
	fout.close();
	return 1;
}





//2016.4.24～ for C部門2016
/*-------------------------------*/
/* 刺激変化検出実験2             */
/* 自発性瞬目                    */
/*-------------------------------*/
//
//  C部門2016用
//  自発性BLのタイミングで切り替え
//　切り替え時点の目領域映像を記録
//
//  実験参加者1名あたり、48×4試行分用意している。
//　　ex.実験参加者:test → test1,test2,test3,test4のフォルダ(test内のサブフォルダ）に48試行ずつ展開している
//
//　TR_NUM回分、自発性BLのデータが取得できた時点で、実験を終了する。
//　最大実験時間は40分間とする。時間が過ぎたら強制終了する。
//
//

int cb_exp2_c2016()
{
	static int st[TR_NUM4][8]; // 各試行において8箇所の周辺位置に提示する刺激番号(0,1,2,3)　
	static int chpos[TR_NUM4]; // 各試行において刺激が変化する周辺位置(0：左上隅,1,・・・,7：右下隅)
	static int prst[TR_NUM4];  // 変化前の刺激番号
	static int chst[TR_NUM4];  // 変化時に提示する刺激番号
	static int intvl[TR_NUM4]; // 最大刺激変化時間間隔

	int tr, fn;
	int basei, basej, dpix;
	int px, py;
	int i, j, k, is, js, ie, je;
	int k0, k1;
	int answer, blcnt;
	int itmp1, itmp2, itmp3, itmp4, itmp5;
	int th[5], bl_th_val;
	int cur_tim, tim_min, tim_sec, tim_msec;
	double dtmp;
	char split;

	int64 exp_start;  // 実験開始時刻
	int64 exp_eltim;  // 実験経過時間
	int   tr_eltim = 0; // １試行開始後の経過時間

	std::string subnm; // 実験参加者名
	std::string sbuf, sbuf1, sbuf2, sbuf3, sbuf4;  // バッファ
	std::string fns, bfname;
	cv::Mat frame, gray;


	cout << "---刺激変化検出実験2---" << endl;

	cout << "実験参加者=(subA,subBなどと入力）"; // subA1などの入力は受け付けない
	cin >> subnm;

	// BL検出範囲設定・しきい値ファイルの読み出し
	bfname = "threshold.csv";
	ifstream finb(bfname);
	if (!finb) {
		cout << "検出範囲用ファイルがありません" << bfname << endl;
		getchar();
		getchar();
		return 0;
	}

	// 設定範囲読み出し
	finb >> sbuf; // 1行分読み飛ばし
	finb >> is;
	finb >> js;
	finb >> ie;
	finb >> je;
	cout << "設定範囲" << endl;
	cout << is << "," << js << "," << ie << "," << je << endl;

	//しきい値の読み出し
	cout << "しきい値" << endl;
	finb >> sbuf; // 1行分読み飛ばし
	for (i = 0; i < TH_TR_NUM; i++) {
		finb >> th[i];
		cout << th[i] << endl;
	}
	finb.close();

	// しきい値設定(平均値とする）2016.5.4
	bl_th_val = 0;
	for (i = 0; i < TH_TR_NUM; i++) {
		bl_th_val += th[i];
	}
	bl_th_val /= TH_TR_NUM;
	cout << "BL検出しきい値=" << bl_th_val << endl;

	cout << "確認できたら何か入力してください" << endl;
	getchar();
	getchar();


	// 刺激提示順序データ読み込み
	std::string sfname;

	for (fn = 0; fn <= 3; fn++) { // フォルダ番号
		fns = std::to_string(fn + 1);
		sfname = "c:\\IEEJ_C_2016\\";
		sfname = sfname + "exp2" + "\\" + subnm + "\\" + subnm + fns + "\\order_";
		sfname = sfname + "実験2" + "_" + subnm + fns + ".csv";     // ファイル名の生成

																  // 刺激提示順序データファイルの読み込み
		ifstream fin(sfname);
		if (!fin) {
			cout << "刺激提示用ファイルがありません" << sfname << endl;
			getchar();
			getchar();
			return 0;
		}

		// 変化情報の読み取り
		fin >> sbuf; // 1行分読み飛ばし

					 // 試行番号、変化位置、変化前刺激、変化後刺激,提示間隔
		for (tr = 0; tr < TR_NUM; tr++) {
			fin >> itmp1 >> split >> itmp2 >> split >> itmp3 >> split >> itmp4 >> split >> itmp5;
			chpos[tr + fn*TR_NUM] = itmp2;
			prst[tr + fn*TR_NUM] = itmp3;
			chst[tr + fn*TR_NUM] = itmp4;
			intvl[tr + fn*TR_NUM] = itmp5;
			//cout << tr+1 << ","<< chpos[tr] << "," << prst[tr] << "," << chst[tr]<< endl;//確認
		}

		// 提示パターン（周辺８箇所）
		fin >> sbuf; // 1行分読み飛ばし

		for (tr = 0; tr < TR_NUM; tr++) {
			fin >> itmp1;// 試行番号読み飛ばし
			for (i = 0; i < 8; i++) {
				fin >> split >> itmp1;
				st[tr + fn*TR_NUM][i] = itmp1;
			}
			fin >> split;
		}
		fin.close();
	}//for(fn=


	 // 提示情報の読み込み結果の確認
	cout << "試行番号,変化位置,現在刺激番号,変化後刺激番号,提示間隔" << endl;
	for (tr = 0; tr < TR_NUM4; tr++) {
		cout << tr + 1 << ":" << chpos[tr] << "," << prst[tr] << "," << chst[tr] << "," << intvl[tr] << endl;
	}
	cout << "試行番号,提示刺激（各周辺位置)" << endl;
	for (tr = 0; tr < TR_NUM4; tr++) {
		cout << tr + 1 << ":";

		for (i = 0; i < 8; i++) {
			cout << st[tr][i] << ",";
		}
		cout << endl;
	}


	//ここまでOK(2016.5.17)

	cout << "実験を始めますか：何かキーを押してください";
	getchar();
	getchar();


	// 試行開始準備

	// 回答情報記録準備
	std::string fname, imname, eyename;
	std::ostringstream ostr;

	fname = "c:\\IEEJ_C_2016\\";
	fname = fname + "exp2" + "\\" + subnm + "\\info_";
	fname = fname + "実験2" + "_" + subnm + ".csv"; // ファイル名の生成

												  // 回答情報出力用ファイルの生成
	ofstream fout(fname); // 出力ファイルを作成する
	if (!fout) {
		cout << "ファイルが開きません" << endl;
		getchar();
		getchar();
		return 0;
	}
	fout << "試行番号,変化時間,BL生起,変化位置" << endl;

	char *window_info = "info";
	//cv::namedWindow(window_info, CV_WINDOW_AUTOSIZE);							      　// 刺激提示用windowの生成
	//cv::moveWindow(window_info, (DH_SIZE - SW_HSIZE) / 2, (DV_SIZE - SW_VSIZE) / 2);  // 画面中央へ移動
	cv::Mat background(cv::Size(DH_SIZE, DV_SIZE), CV_8UC3, cv::Scalar(255, 255, 255)); // Windowに表示する背景の準備

																						//提示刺激の準備
	char mark[][10] = { "X", "O", "H", "E" }; // １バイト文字を選定


											  //文字提示位置の中心位置（注視点位置）
	basei = DH_SIZE / 2;
	basej = DV_SIZE / 2;

	// 刺激配置の算出
	dtmp = OBDIST*tan(INTDEG*PAI / 180.0);
	dtmp /= PITCH;
	dpix = (int)dtmp;

	exp_start = cvGetTickCount(); // 実験開始時刻の取得
	blcnt = 0;                   // BL生起試行数の初期化

								 // 実験開始!
	for (tr = 0; tr < TR_NUM4; tr++) { // 最大TR_NUM4回実験実施
		cv::Mat background(cv::Size(DH_SIZE, DV_SIZE), CV_8UC3, cv::Scalar(255, 255, 255)); // Windowに表示する背景の準備
		cv::imshow(window_info, background);  // 画像表示
		cv::waitKey(3 * 1000);                // 背景を3秒間提示

											  // 提示刺激の準備
		int pcnt = 0;
		for (j = -1; j <= 1; j++) {
			for (i = -1; i <= 1; i++) {
				if (i == 0 && j == 0) continue; // 画面中央は別の記号を提示するのでスキップ
				px = basei + i*dpix; // 今回の提示位置
				py = basej + j*dpix; // 今回の提示位置
				k = st[tr][pcnt];    // 提示位置pcntへ提示する刺激番号の設定
				cv::putText(background, (const char*)mark[k], cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0, 0, 0), 3, CV_AA);
				pcnt++; // 次の提示位置(0,1,2,・・・,7)
			}//for(i=
		}//for(j=

		 // 画面中央への記号（注視点提示）
		px = basei; // 提示位置(横）
		py = basej; // 提示位置(縦)
		cv::putText(background, "+", cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(128, 128, 128), 3, CV_AA);

		// 刺激提示
		//startime = (double)cvGetTickCount();  // 開始時刻の取得
		cv::imshow(window_info, background);    // 画像表示(この時点ではまだ提示開始されていない）
		cv::waitKey(30);                        // 刺激画像の提示

												//刺激提示画面の変化準備
		k0 = prst[tr]; // 変化前の刺激番号
		k1 = chst[tr]; // 変化後の刺激番号
		get_ij(chpos[tr], &i, &j); // 変化位置のi,j番号の取得
		px = basei + i*dpix;       // 変化位置の設定
		py = basej + j*dpix;       // 変化位置の設定

		int result = -1;
		result = check_volbl_occurence(intvl[tr] * 1000, &tr_eltim, bl_th_val, is, js, ie, je); // // 自発性BLのチェック(result=1:BL生起、result=0:タイムアウト)
		time_delay(15);    // 15ms待機し、瞼が閉じた状態のときに刺激を切り替える(このぐらいがよさそう）
		cv::putText(background, mark[k0], cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(255, 255, 255), 5, CV_AA); // 古い記号を消す
		cv::putText(background, mark[k1], cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0, 0, 0), 3, CV_AA);       // 新しい記号を描く
		cv::imshow(window_info, background);	// 記号表示更新
		cv::waitKey(5);                      // 即座に描画
		cap >> gframe;                          // 更新時点の目領域映像の取得
		cv::waitKey(3 * 1000);                  // 3秒待機

												// 目領域画像記録
		std::ostringstream ostr1;
		ostr1 << "_tr" << tr;

		//以下、動作確認していない → フォルダとファイル名(2016.4.13)
		eyename = "c:\\IEEJ_C_2016\\";
		eyename = eyename + "exp2" + "\\" + subnm + "\\eye_実験2_";
		eyename = eyename + subnm + ostr1.str() + ".bmp";

		cv::imwrite(eyename, gframe);   // BL時の画像を記録
		cv::destroyWindow(window_info); // windowの破棄

										//経過時間表示準備
		cur_tim = (cvGetTickCount() - exp_start) / (1000 * cvGetTickFrequency()); // ms単位
		tim_min = cur_tim / (60 * 1000);                // 分単位
		tim_sec = (cur_tim - tim_min * 60 * 1000) / 1000; // 秒単位


														  // 回答入力（コンソールで）
		cout << endl;
		cout << "経過時間：" << tim_min << "分" << tim_sec << "秒" << endl;
		cout << endl;
		cout << "試行番号:" << tr + 1 << "/" << TR_NUM4 << endl << endl;
		cout << "------------------------------------------" << endl;
		cout << "どこの位置が変化したか答えてください" << endl;
		cout << "1:左上段、2:中央上段、3:右上段" << endl;
		cout << "4:左中段、　　　　　　5:右中段" << endl;
		cout << "6:左下段、7:中央下段、8:右下段" << endl;
		cout << endl;
		cout << "9:わからない" << endl;
		cin >> answer;

		if (result == 1) { // 自発性BL生起
			fout << tr << "," << tr_eltim << "," << 0 << "," << answer << endl;
			blcnt++;//BL生起回数のカウントアップ
		}
		if (result == 0) { // 自発性BL生起せず、タイムアウト
			fout << tr << "," << tr_eltim << "," << -1 << "," << answer << endl;
		}

		// BL生起試行がTR_NUM回になった時点で実験終了
		if (blcnt >= TR_NUM) {
			break;
		}

		// 40分以上経過したら、途中でも実験終了
		exp_eltim = cvGetTickCount() - exp_start; // 実験経過時間算出

		if (exp_eltim / (1000 * cvGetTickFrequency()) > 40 * 60 * 1000) {
			break;
		}

		/*
		if (exp_eltim / (1000 * cvGetTickFrequency()) > 5 * 60 * 1000){ // 動作確認のため5分に設定している．
		break;
		}
		*/

		if (tr != 0 && (tr % 10) == 0) { // 試行10回毎に目領域位置調整
			adjust_eye_position(is, js, ie, je);
		}
	}//for(tr=
	fout.close();
	return 1;
}



//2016.4.13～ 練習for C部門2016
/*-------------------------------*/
/* 刺激変化検出実験1             */
/* 随意性瞬目                    *
/* -練習-                        */
/*-------------------------------*/
//
//  練習版
//
//  C部門2016用
//  随意性BLのタイミングで切り替え
//　切り替え時点の目領域映像を記録
//
//  BL検出は組み込んでいないが,動作はOK(2016.4.23)
//  BL検出実装(2016.5.4)
//  試行10回毎に目領域位置の微調整を行う(2016.5.4)
//
int cb_exp1_c2016_ren()
{
	static int st[TR_NUM][8]; // 各試行において8箇所の周辺位置に提示する刺激番号(0,1,2,3)　
	int chpos[TR_NUM];        // 各試行において刺激が変化する周辺位置(0：左上隅,1,・・・,7：右下隅)
	int prst[TR_NUM];         // 変化前の刺激番号
	int chst[TR_NUM];         // 変化時に提示する刺激番号

	int tr;
	int basei, basej, dpix;
	int px, py;
	int i, j, k, is, js, ie, je;
	int k0, k1;
	int answer;
	int th[5], bl_th_val;
	int itmp1, itmp2, itmp3, itmp4;
	double dtmp;
	char split;

	std::string subnm; // 実験参加者名
	std::string sbuf, sbuf1, sbuf2, sbuf3, sbuf4;  // バッファ
	std::string bfname;
	cv::Mat frame, gray;

	int tr_eltime; // 試行開始後からBL生起までの経過時間

	cout << "---刺激変化検出実験1（練習版）---" << endl;

	//cout << "実験参加者=";
	//cin >> subnm;

	subnm = "ren"; // 練習版

				   // BL検出範囲設定・しきい値ファイルの読み出し
	bfname = "threshold.csv";
	ifstream finb(bfname);
	if (!finb) {
		cout << "検出範囲用ファイルがありません" << bfname << endl;
		getchar();
		getchar();
		return 0;
	}

	// 設定範囲読み出し
	finb >> sbuf; // 1行分読み飛ばし
	finb >> is;
	finb >> js;
	finb >> ie;
	finb >> je;
	cout << "設定範囲" << endl;
	cout << is << "," << js << "," << ie << "," << je << endl;

	//しきい値の読み出し
	cout << "しきい値" << endl;
	finb >> sbuf; // 1行分読み飛ばし
	for (i = 0; i < TH_TR_NUM; i++) {
		finb >> th[i];
		cout << th[i] << endl;
	}
	finb.close();

	// しきい値設定(平均値とする）2016.5.4
	bl_th_val = 0;
	for (i = 0; i < TH_TR_NUM; i++) {
		bl_th_val += th[i];
	}
	bl_th_val /= TH_TR_NUM;
	cout << "BL検出しきい値=" << bl_th_val << endl;

	cout << "確認できたら何か入力してください" << endl;
	getchar();
	getchar();


	// 刺激提示順序データ読み込み
	std::string sfname;

	sfname = "c:\\IEEJ_C_2016\\";
	sfname = sfname + "exp1" + "\\" + subnm + "\\order_";
	sfname = sfname + "実験1" + "_" + subnm + ".csv"; // ファイル名の生成

													// 刺激提示順序データファイルの読み込み
	ifstream fin(sfname);
	if (!fin) {
		cout << "刺激提示用ファイルがありません" << endl;
		getchar();
		getchar();
		return 0;
	}


	// 変化情報の読み取り
	fin >> sbuf; // 1行分読み飛ばし

				 // 試行番号、変化位置、変化前刺激、変化後刺激
	for (tr = 0; tr < TR_NUM; tr++) {
		fin >> itmp1 >> split >> itmp2 >> split >> itmp3 >> split >> itmp4;
		chpos[tr] = itmp2;
		prst[tr] = itmp3;
		chst[tr] = itmp4;
		//cout << tr+1 << ","<< chpos[tr] << "," << prst[tr] << "," << chst[tr]<< endl;//確認
	}

	// 提示パターン（周辺８箇所）
	fin >> sbuf; // 1行分読み飛ばし

	for (tr = 0; tr<TR_NUM; tr++) {
		fin >> itmp1;// 試行番号読み飛ばし
		for (i = 0; i < 8; i++) {
			fin >> split >> itmp1;
			st[tr][i] = itmp1;
		}
		fin >> split;
	}
	fin.close();

	// 提示情報の読み込み結果の確認
	cout << "試行番号,変化位置,現在刺激番号,変化後刺激番号" << endl;
	for (tr = 0; tr < TR_NUM; tr++) {
		cout << tr + 1 << ":" << chpos[tr] << "," << prst[tr] << "," << chst[tr] << endl;
	}
	cout << "試行番号,提示刺激（各周辺位置)" << endl;
	for (tr = 0; tr < TR_NUM; tr++) {
		cout << tr + 1 << ":";

		for (i = 0; i < 8; i++) {
			cout << st[tr][i] << ",";
		}
		cout << endl;
	}

	// 目領域位置調整
	cout << "目領域の調整を行います" << endl;
	adjust_eye_position(is, js, ie, je);

	cout << "練習を始めますか：何かキーを押してください";
	getchar();
	getchar();


	// 試行開始準備

	// 回答情報記録準備
	std::string fname, imname, eyename;
	std::ostringstream ostr;

	fname = "c:\\IEEJ_C_2016\\";
	fname = fname + "exp1" + "\\" + subnm + "\\info_";
	fname = fname + "実験1" + "_" + subnm + ".csv"; // ファイル名の生成

												  // 回答情報出力用ファイルの生成
	ofstream fout(fname); // 出力ファイルを作成する
	if (!fout) {
		cout << "ファイルが開きません" << endl;
		getchar();
		getchar();
		return 0;
	}
	fout << "試行番号,変化時間,BL生起,変化位置" << endl;

	char *window_info = "info";
	//cv::namedWindow(window_info, CV_WINDOW_AUTOSIZE);							      　// 刺激提示用windowの生成
	//cv::moveWindow(window_info, (DH_SIZE - SW_HSIZE) / 2, (DV_SIZE - SW_VSIZE) / 2);  // 画面中央へ移動
	cv::Mat background(cv::Size(DH_SIZE, DV_SIZE), CV_8UC3, cv::Scalar(255, 255, 255)); // Windowに表示する背景の準備

																						//提示刺激の準備
	char mark[][10] = { "X", "O", "H", "E" }; // １バイト文字を選定


											  //文字提示位置の中心位置（注視点位置）
	basei = DH_SIZE / 2;
	basej = DV_SIZE / 2;

	// 刺激配置の算出
	dtmp = OBDIST*tan(INTDEG*PAI / 180.0);
	dtmp /= PITCH;
	dpix = (int)dtmp;

	// 実験開始!
	//for (tr = 0; tr < TR_NUM; tr++){ // TR_NUM4回実験実施
	for (tr = 0; tr < 10; tr++) {  // 10回練習実施
		cv::Mat background(cv::Size(DH_SIZE, DV_SIZE), CV_8UC3, cv::Scalar(255, 255, 255)); // Windowに表示する背景の準備
		cv::imshow(window_info, background);  // 画像表示
		cv::waitKey(3 * 1000);                // 背景を3秒間提示

											  // 提示刺激の準備
		int pcnt = 0;
		for (j = -1; j <= 1; j++) {
			for (i = -1; i <= 1; i++) {
				if (i == 0 && j == 0) continue; // 画面中央は別の記号を提示するのでスキップ
				px = basei + i*dpix; // 今回の提示位置
				py = basej + j*dpix; // 今回の提示位置
				k = st[tr][pcnt];    // 提示位置pcntへ提示する刺激番号の設定
				cv::putText(background, (const char*)mark[k], cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0, 0, 0), 3, CV_AA);
				pcnt++; // 次の提示位置(0,1,2,・・・,7)
			}//for(i=
		}//for(j=

		 // 画面中央への記号（注視点提示）
		px = basei; // 提示位置(横）
		py = basej; // 提示位置(縦)
		cv::putText(background, "+", cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(128, 128, 128), 3, CV_AA);

		// 刺激提示
		//startime = (double)cvGetTickCount();  // 開始時刻の取得
		cv::imshow(window_info, background);    // 画像表示(この時点ではまだ提示開始されていない）
		cv::waitKey(30);                        // 刺激画像の提示

												//刺激提示画面の変化準備
		k0 = prst[tr]; // 変化前の刺激番号
		k1 = chst[tr]; // 変化後の刺激番号
		get_ij(chpos[tr], &i, &j); // 変化位置のi,j番号の取得
		px = basei + i*dpix;       // 変化位置の設定
		py = basej + j*dpix;       // 変化位置の設定

		int result = -1;
		result = check_volbl_occurence(10 * 1000, &tr_eltime, bl_th_val, is, js, ie, je); // 随意性BLのチェック(10秒間） 
		if (result == 1) {      // 10秒間で何かBLが生じたら
			time_delay(15);    // 15ms待機し、瞼が閉じた状態のときに刺激を切り替える(このぐらいがよさそう）
			cv::putText(background, mark[k0], cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(255, 255, 255), 5, CV_AA); // 古い記号を消す
			cv::putText(background, mark[k1], cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0, 0, 0), 3, CV_AA);       // 新しい記号を描く
			cv::imshow(window_info, background); // 記号表示更新
			cv::waitKey(5);                      // 即座に描画
			cap >> gframe;                       // 更新時点の目領域映像の取得
			time_delay(3 * 1000);                  // 3秒待機
		}

		// 10秒間,随意性BLが生じなかった場合は、試行を無効とする。タイムアウト
		if (result == 0) {
			fout << tr << "," << tr_eltime << "," << -1 << "," << -1 << endl; // 無効な試行として記録する
			cv::destroyWindow(window_info);  // windowの破棄
			if (tr != 0 && (tr % 10) == 0) {  // 試行10回毎に目領域位置調整
				adjust_eye_position(is, js, ie, je);
			}
			continue;
		}

		// 目領域画像記録
		std::ostringstream ostr1;
		ostr1 << "_tr" << tr;

		eyename = "c:\\IEEJ_C_2016\\";
		eyename = eyename + "exp1" + "\\" + subnm + "\\eye_実験1_";
		eyename = eyename + subnm + ostr1.str() + ".bmp";

		cv::imwrite(eyename, gframe);   // 記号更新時の画像を記録
		cv::destroyWindow(window_info); // windowの破棄


										// 回答入力（コンソールで）
		cout << endl;
		cout << "試行番号:" << tr + 1 << "/" << TR_NUM << endl << endl;
		cout << "------------------------------------------" << endl;
		cout << "どこの位置が変化したか答えてください" << endl;
		cout << "1:左上段、2:中央上段、3:右上段" << endl;
		cout << "4:左中段、　　　　　　5:右中段" << endl;
		cout << "6:左下段、7:中央下段、8:右下段" << endl;
		cout << endl;
		cout << "9:わからない" << endl;
		cin >> answer;

		fout << tr << "," << tr_eltime << "," << 0 << "," << answer << endl;

		if (tr != 0 && (tr % 10) == 0) { // 試行10回毎に目領域位置調整
			adjust_eye_position(is, js, ie, je);
		}

	}//for(tr=
	fout.close();
	return 1;
}








//2016.4.13～ for C部門2016
/*-------------------------------*/
/* 刺激変化検出実験1             */
/* 随意性瞬目                    */
/*-------------------------------*/
//
//  C部門2016用
//  随意性BLのタイミングで切り替え
//　切り替え時点の目領域映像を記録
//
//  BL検出は組み込んでいないが,動作はOK(2016.4.23)
//  BL検出実装(2016.5.4)
//  試行10回毎に目領域位置の微調整を行う(2016.5.4)
//
int cb_exp1_c2016()
{
	static int st[TR_NUM][8]; // 各試行において8箇所の周辺位置に提示する刺激番号(0,1,2,3)　
	int chpos[TR_NUM];        // 各試行において刺激が変化する周辺位置(0：左上隅,1,・・・,7：右下隅)
	int prst[TR_NUM];         // 変化前の刺激番号
	int chst[TR_NUM];         // 変化時に提示する刺激番号

	int tr;
	int basei, basej, dpix;
	int px, py;
	int i, j, k, is, js, ie, je;
	int k0, k1;
	int answer;
	int th[5], bl_th_val;
	int itmp1, itmp2, itmp3, itmp4;
	double dtmp;
	char split;

	std::string subnm; // 実験参加者名
	std::string sbuf, sbuf1, sbuf2, sbuf3, sbuf4;  // バッファ
	std::string bfname;
	cv::Mat frame, gray;

	int tr_eltime; // 試行開始後からBL生起までの経過時間

	cout << "---刺激変化検出実験1---" << endl;

	cout << "実験参加者=";
	cin >> subnm;

	// BL検出範囲設定・しきい値ファイルの読み出し
	bfname = "threshold.csv";
	ifstream finb(bfname);
	if (!finb) {
		cout << "検出範囲用ファイルがありません" << bfname << endl;
		getchar();
		getchar();
		return 0;
	}

	// 設定範囲読み出し
	finb >> sbuf; // 1行分読み飛ばし
	finb >> is;
	finb >> js;
	finb >> ie;
	finb >> je;
	cout << "設定範囲" << endl;
	cout << is << "," << js << "," << ie << "," << je << endl;

	//しきい値の読み出し
	cout << "しきい値" << endl;
	finb >> sbuf; // 1行分読み飛ばし
	for (i = 0; i < TH_TR_NUM; i++) {
		finb >> th[i];
		cout << th[i] << endl;
	}
	finb.close();

	// しきい値設定(平均値とする）2016.5.4
	bl_th_val = 0;
	for (i = 0; i < TH_TR_NUM; i++) {
		bl_th_val += th[i];
	}
	bl_th_val /= TH_TR_NUM;
	cout << "BL検出しきい値=" << bl_th_val << endl;

	cout << "確認できたら何か入力してください" << endl;
	getchar();
	getchar();


	// 刺激提示順序データ読み込み
	std::string sfname;

	sfname = "c:\\IEEJ_C_2016\\";
	sfname = sfname + "exp1" + "\\" + subnm + "\\order_";
	sfname = sfname + "実験1" + "_" + subnm + ".csv"; // ファイル名の生成

													// 刺激提示順序データファイルの読み込み
	ifstream fin(sfname);
	if (!fin) {
		cout << "刺激提示用ファイルがありません" << endl;
		getchar();
		getchar();
		return 0;
	}


	// 変化情報の読み取り
	fin >> sbuf; // 1行分読み飛ばし

				 // 試行番号、変化位置、変化前刺激、変化後刺激
	for (tr = 0; tr < TR_NUM; tr++) {
		fin >> itmp1 >> split >> itmp2 >> split >> itmp3 >> split >> itmp4;
		chpos[tr] = itmp2;
		prst[tr] = itmp3;
		chst[tr] = itmp4;
		//cout << tr+1 << ","<< chpos[tr] << "," << prst[tr] << "," << chst[tr]<< endl;//確認
	}

	// 提示パターン（周辺８箇所）
	fin >> sbuf; // 1行分読み飛ばし

	for (tr = 0; tr<TR_NUM; tr++) {
		fin >> itmp1;// 試行番号読み飛ばし
		for (i = 0; i < 8; i++) {
			fin >> split >> itmp1;
			st[tr][i] = itmp1;
		}
		fin >> split;
	}
	fin.close();

	// 提示情報の読み込み結果の確認
	cout << "試行番号,変化位置,現在刺激番号,変化後刺激番号" << endl;
	for (tr = 0; tr < TR_NUM; tr++) {
		cout << tr + 1 << ":" << chpos[tr] << "," << prst[tr] << "," << chst[tr] << endl;
	}
	cout << "試行番号,提示刺激（各周辺位置)" << endl;
	for (tr = 0; tr < TR_NUM; tr++) {
		cout << tr + 1 << ":";

		for (i = 0; i < 8; i++) {
			cout << st[tr][i] << ",";
		}
		cout << endl;
	}

	// 目領域位置調整
	cout << "目領域の調整を行います" << endl;
	adjust_eye_position(is, js, ie, je);

	cout << "実験を始めますか：何かキーを押してください";
	getchar();
	getchar();


	// 試行開始準備

	// 回答情報記録準備
	std::string fname, imname, eyename;
	std::ostringstream ostr;

	fname = "c:\\IEEJ_C_2016\\";
	fname = fname + "exp1" + "\\" + subnm + "\\info_";
	fname = fname + "実験1" + "_" + subnm + ".csv"; // ファイル名の生成

												  // 回答情報出力用ファイルの生成
	ofstream fout(fname); // 出力ファイルを作成する
	if (!fout) {
		cout << "ファイルが開きません" << endl;
		getchar();
		getchar();
		return 0;
	}
	fout << "試行番号,変化時間,BL生起,変化位置" << endl;

	char *window_info = "info";
	//cv::namedWindow(window_info, CV_WINDOW_AUTOSIZE);							      　// 刺激提示用windowの生成
	//cv::moveWindow(window_info, (DH_SIZE - SW_HSIZE) / 2, (DV_SIZE - SW_VSIZE) / 2);  // 画面中央へ移動
	cv::Mat background(cv::Size(DH_SIZE, DV_SIZE), CV_8UC3, cv::Scalar(255, 255, 255)); // Windowに表示する背景の準備

																						//提示刺激の準備
	char mark[][10] = { "X", "O", "H", "E" }; // １バイト文字を選定


											  //文字提示位置の中心位置（注視点位置）
	basei = DH_SIZE / 2;
	basej = DV_SIZE / 2;

	// 刺激配置の算出
	dtmp = OBDIST*tan(INTDEG*PAI / 180.0);
	dtmp /= PITCH;
	dpix = (int)dtmp;

	// 実験開始!
	for (tr = 0; tr < TR_NUM; tr++) { // TR_NUM回実験実施
									  //for (tr = 0; tr < 1; tr++){ // TR_NUM回実験実施
		cv::Mat background(cv::Size(DH_SIZE, DV_SIZE), CV_8UC3, cv::Scalar(255, 255, 255)); // Windowに表示する背景の準備
		cv::imshow(window_info, background);  // 画像表示
		cv::waitKey(3 * 1000);                // 背景を3秒間提示

											  // 提示刺激の準備
		int pcnt = 0;
		for (j = -1; j <= 1; j++) {
			for (i = -1; i <= 1; i++) {
				if (i == 0 && j == 0) continue; // 画面中央は別の記号を提示するのでスキップ
				px = basei + i*dpix; // 今回の提示位置
				py = basej + j*dpix; // 今回の提示位置
				k = st[tr][pcnt];    // 提示位置pcntへ提示する刺激番号の設定
				cv::putText(background, (const char*)mark[k], cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0, 0, 0), 3, CV_AA);
				pcnt++; // 次の提示位置(0,1,2,・・・,7)
			}//for(i=
		}//for(j=

		 // 画面中央への記号（注視点提示）
		px = basei; // 提示位置(横）
		py = basej; // 提示位置(縦)
		cv::putText(background, "+", cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(128, 128, 128), 3, CV_AA);

		// 刺激提示
		//startime = (double)cvGetTickCount();  // 開始時刻の取得
		cv::imshow(window_info, background);    // 画像表示(この時点ではまだ提示開始されていない）
		cv::waitKey(30);                        // 刺激画像の提示

												//刺激提示画面の変化準備
		k0 = prst[tr]; // 変化前の刺激番号
		k1 = chst[tr]; // 変化後の刺激番号
		get_ij(chpos[tr], &i, &j); // 変化位置のi,j番号の取得
		px = basei + i*dpix;       // 変化位置の設定
		py = basej + j*dpix;       // 変化位置の設定

		int result = -1;
		result = check_volbl_occurence(10 * 1000, &tr_eltime, bl_th_val, is, js, ie, je); // 随意性BLのチェック(10秒間） 
		if (result == 1) {      // 10秒間で何かBLが生じたら
			time_delay(15);    // 15ms待機し、瞼が閉じた状態のときに刺激を切り替える(このぐらいがよさそう）
			cv::putText(background, mark[k0], cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(255, 255, 255), 5, CV_AA); // 古い記号を消す
			cv::putText(background, mark[k1], cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0, 0, 0), 3, CV_AA);       // 新しい記号を描く
			cv::imshow(window_info, background); // 記号表示更新
			cv::waitKey(5);                      // 即座に描画
			cap >> gframe;                       // 更新時点の目領域映像の取得
			time_delay(3 * 1000);                  // 3秒待機
		}

		// 10秒間,随意性BLが生じなかった場合は、試行を無効とする。タイムアウト
		if (result == 0) {
			fout << tr << "," << tr_eltime << "," << -1 << "," << -1 << endl; // 無効な試行として記録する
			cv::destroyWindow(window_info);  // windowの破棄
			if (tr != 0 && (tr % 10) == 0) {  // 試行10回毎に目領域位置調整
				adjust_eye_position(is, js, ie, je);
			}
			continue;
		}

		// 目領域画像記録
		std::ostringstream ostr1;
		ostr1 << "_tr" << tr;

		eyename = "c:\\IEEJ_C_2016\\";
		eyename = eyename + "exp1" + "\\" + subnm + "\\eye_実験1_";
		eyename = eyename + subnm + ostr1.str() + ".bmp";

		cv::imwrite(eyename, gframe);   // 記号更新時の画像を記録
		cv::destroyWindow(window_info); // windowの破棄


										// 回答入力（コンソールで）
		cout << endl;
		cout << "試行番号:" << tr + 1 << "/" << TR_NUM << endl << endl;
		cout << "------------------------------------------" << endl;
		cout << "どこの位置が変化したか答えてください" << endl;
		cout << "1:左上段、2:中央上段、3:右上段" << endl;
		cout << "4:左中段、　　　　　　5:右中段" << endl;
		cout << "6:左下段、7:中央下段、8:右下段" << endl;
		cout << endl;
		cout << "9:わからない" << endl;
		cin >> answer;

		fout << tr << "," << tr_eltime << "," << 0 << "," << answer << endl;

		if (tr != 0 && (tr % 10) == 0) { // 試行10回毎に目領域位置調整
			adjust_eye_position(is, js, ie, je);
		}

	}//for(tr=
	fout.close();
	return 1;
}

// 2016.4.23～
/*------------------*/
/* BL生起のチェック */
/*------------------*/
//　
// 戻り値:0 タイムアップ、BL生起なし。
//        1 BL生起
//
int check_volbl_occurence(
	int maxdur,    // 最大待ち時間(ms)
	int *eltim,    // 試行開始後の経過時間
	int blth,      // BL検出時のしきい値
	int is,        // 検出範囲
	int js,
	int ie,
	int je
)
{
	int64 start, eltime;
	int fr, th, i;
	cv::Mat frame, gray;

	//std::string fname;
	//fname = "kari.csv";
	//ofstream fout(fname);


	// 画像表示用ウィンドウの生成(原画像）
	//char *window_input = "input";
	//cv::namedWindow(window_input, CV_WINDOW_AUTOSIZE);

	// 画像表示用ウィンドウの生成(エッジ画像）
	//char *window_output = "output";
	//cv::namedWindow(window_output, CV_WINDOW_AUTOSIZE);


	int canny_th1 = CANNY_TH1; // 信頼性の高いエッジの閾値（固定値とする）
	int canny_th2 = CANNY_TH2; // 信頼性に低いエッジの閾値（固定値とする）

	start = cvGetTickCount(); // 開始時刻
	fr = 0;
	while (1) {
		eltime = cvGetTickCount() - start; // 経過時間
		if ((int)(eltime / (1000 * cvGetTickFrequency())) > maxdur) {
			*eltim = (int)(eltime / (1000 * cvGetTickFrequency()));
			//fout.close(); 
			return 0; // タイムアウト
		}
		cap >> frame;
		cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);	// カラー画像をモノクロ画像に変換
		cv::Canny(gray, gray, canny_th1, canny_th2, 3); // キャニーオペレータによるエッジ抽出

														//cv::rectangle(frame, cv::Point(is, js), cv::Point(ie, je), cv::Scalar(0, 0, 255), 1, 1);    // 関心領域を描画する
														//cv::rectangle(gray, cv::Point(is, js), cv::Point(ie, je), cv::Scalar(255, 255, 255), 1, 1); // 関心領域を描画する
														//cv::imshow(window_input, frame); // 原画像の表示
														//cv::imshow(window_output, gray); // エッジ検出画像
														//cv::waitKey(10);                 // 画像表示 

		edge_sum[fr] = count_edge_sum(gray, is, js, ie, je); // エッジ総量
		if (fr < FR_DUR) {
			fr++;
			continue;
		}
		th = 0;
		for (i = fr - 1; i > fr - 1 - FR_DUR; i--) {// 過去FR_DURフレーム分（現フレームを含まない）の平均値
			th += edge_sum[i]; // エッジ点数の積算
		}
		th /= FR_DUR;

		//fout << fr << "," << edge_sum[fr] << "," << th << "," << edge_sum[fr] - th << endl;


		if (edge_sum[fr] - th < blth) { // BL検出(しきい値以上の動きあり）
			*eltim = (int)(eltime / (1000 * cvGetTickFrequency()));
			break;

		}
		fr++;

		// BL判定処理の代わり（暫定：2016.4.23)
		//if (cv::waitKey(30) >= 0){ // なにかキーが押されたら(BLが生じたら）終了
		//	*eltim = (int)(eltime / (1000 * cvGetTickFrequency()));
		//	break;
		//}

	}//while(1)
	return 1; // BL生起
}


// 2016.5.4
/*--------------------*/
/* 目領域の位置の調整 */
/*--------------------*/
//
// 表示された枠内に目が入るように調整する
//
int adjust_eye_position(
	int is,
	int js,
	int ie,
	int je
)
{
	cv::Mat frame;
	char *window_input = "input";
	std::string msg;


	cv::namedWindow(window_input, CV_WINDOW_AUTOSIZE);
	cv::moveWindow(window_input, 0, 0);  // 画面左上隅へ移動
	while (1) {
		cap >> frame;
		msg = "Move your eye-region into red ploted square frame";
		cv::putText(frame, msg, cv::Point(10, 40), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 0, 0), 1, CV_AA); // メッセージ表示
		cv::rectangle(frame, cv::Point(is, js), cv::Point(ie, je), cv::Scalar(0, 0, 255), 1, 1);                    // 関心領域を描画する
		cv::imshow(window_input, frame);
		if (cv::waitKey(30) > 0) { // 何かキーが押されたら
			break;
		}
	}
	cv::destroyWindow(window_input); // ウィンドウの破棄
	return 1;
}


//2016.2.28～ for SSII2016
/*-------------------------------*/
/* 刺激変化検出実験3             */
/* CB現象を伴う状態での検出実験  */
/* 注意誘導つき                  */
/*-------------------------------*/
//
//  SSII2016用
//　刺激が変化する位置の方向を示す（4方向：上下左右）
//  特定のSW入力と同時に随意性のBLを行う
//  SW入力から100ms後の時点で切り替える
//　切り替え時点の目領域映像を記録しておく
//

int cb_exp3_ssii()
{
	static int st[TR_NUM][8]; // 各試行において8箇所の周辺位置に提示する刺激番号(0,1,2,3)　
	int chpos[TR_NUM];        // 各試行において刺激が変化する周辺位置(0：左上隅,1,・・・,7：右下隅)
	int prst[TR_NUM];         // 変化前の刺激番号
	int chst[TR_NUM];         // 変化時に提示する刺激番号
	int att[TR_NUM];          // 注意の誘導方向

	int tr;
	int basei, basej, dpix;
	int px, py;
	int i, j, k;
	int k0, k1;
	int answer;
	int itmp1, itmp2, itmp3, itmp4, itmp5;
	int w, h;
	double dtmp;
	char split;

	std::string subnm; // 実験参加者名
	std::string sbuf, sbuf1, sbuf2, sbuf3, sbuf4;  // バッファ
	cv::Mat frame, gray;
	cv::Mat right, left, down, upper; // 注意の方向を示す矢印画像

									  // 矢印画像の読み込み
	right = cv::imread("right_dir.bmp", cv::IMREAD_GRAYSCALE);
	if (right.empty()) {
		cout << "ファイルがありません" << endl;
		return 0;
	}

	left = cv::imread("left_dir.bmp", cv::IMREAD_GRAYSCALE);
	if (left.empty()) {
		cout << "ファイルがありません" << endl;
		return 0;
	}

	down = cv::imread("down_dir.bmp", cv::IMREAD_GRAYSCALE);
	if (down.empty()) {
		cout << "ファイルがありません" << endl;
		return 0;
	}

	upper = cv::imread("upper_dir.bmp", cv::IMREAD_GRAYSCALE);
	if (upper.empty()) {
		cout << "ファイルがありません" << endl;
		return 0;
	}

	// 矢印画像のサイズ取得
	w = upper.cols; // 幅
	h = upper.rows; // 高さ


	cout << "---刺激変化検出実験3---" << endl;

	cout << "実験参加者=";
	cin >> subnm;

	// 刺激提示順序データ読み込み
	std::string sfname;
	sfname = "order_";
	sfname += "実験3_" + subnm + ".csv"; // ファイル名の生成

									   // 刺激提示順序データファイルの読み込み
	ifstream fin(sfname);
	if (!fin) {
		cout << "刺激提示用ファイルがありません" << endl;
		return 0;
	}

	// 変化情報の読み取り
	fin >> sbuf; // 1行分読み飛ばし

				 // 試行番号、変化位置、変化前刺激、変化後刺激、誘導方向
	for (tr = 0; tr < TR_NUM; tr++) {
		fin >> itmp1 >> split >> itmp2 >> split >> itmp3 >> split >> itmp4 >> split >> itmp5;

		chpos[tr] = itmp2;
		prst[tr] = itmp3;
		chst[tr] = itmp4;
		att[tr] = itmp5;
		//cout << tr+1 << ","<< chpos[tr] << "," << prst[tr] << "," << chst[tr]<< endl;//確認
	}

	// 提示パターン（周辺８箇所）
	fin >> sbuf; // 1行分読み飛ばし

	for (tr = 0; tr<TR_NUM; tr++) {
		fin >> itmp1;// 試行番号読み飛ばし
		for (i = 0; i < 8; i++) {
			fin >> split >> itmp1;
			st[tr][i] = itmp1;
		}
		fin >> split;
	}
	fin.close();

	// 提示情報の読み込み結果の確認
	cout << "試行番号,変化位置,現在刺激番号,変化後刺激番号,注意誘導方向" << endl;
	for (tr = 0; tr < TR_NUM; tr++) {
		cout << tr + 1 << ":" << chpos[tr] << "," << prst[tr] << "," << chst[tr] << "," << att[tr] << endl;
	}
	cout << "試行番号,提示刺激（各周辺位置)" << endl;
	for (tr = 0; tr < TR_NUM; tr++) {
		cout << tr + 1 << ":";

		for (i = 0; i < 8; i++) {
			cout << st[tr][i] << ",";
		}
		cout << endl;
	}

	cout << "実験を始めますか：何かキーを押してください";
	getchar();
	getchar();


	// 試行開始準備

	// 回答情報記録準備
	std::string fname, imname, eyename;
	std::ostringstream ostr;
	fname = "info_";
	fname += "実験3_" + subnm + ".csv"; // ファイル名の生成

									  // 回答情報出力用ファイルの生成
	ofstream fout(fname); // 出力ファイルを作成する
	if (!fout) {
		cout << "ファイルが開きません" << endl;
		return 0;
	}
	fout << "試行番号,変化位置" << endl;

	char *window_info = "info";
	//cv::namedWindow(window_info, CV_WINDOW_AUTOSIZE);							      　// 刺激提示用windowの生成
	//cv::moveWindow(window_info, (DH_SIZE - SW_HSIZE) / 2, (DV_SIZE - SW_VSIZE) / 2);  // 画面中央へ移動
	cv::Mat background(cv::Size(DH_SIZE, DV_SIZE), CV_8UC3, cv::Scalar(255, 255, 255)); // Windowに表示する背景の準備

																						//提示刺激の準備
	char mark[][10] = { "X", "O", "H", "E" }; // １バイト文字を選定


											  //文字提示位置の中心位置（注視点位置）
	basei = DH_SIZE / 2;
	basej = DV_SIZE / 2;

	// 刺激配置の算出
	dtmp = OBDIST*tan(INTDEG*PAI / 180.0);
	dtmp /= PITCH;
	dpix = (int)dtmp;

	// 実験開始!
	for (tr = 0; tr < TR_NUM; tr++) { // TR_NUM回実験実施
		cv::Mat background(cv::Size(DH_SIZE, DV_SIZE), CV_8UC3, cv::Scalar(255, 255, 255)); // Windowに表示する背景の準備
		cv::imshow(window_info, background);  // 画像表示
		cv::waitKey(3 * 1000);                // 背景を3秒間提示
											  // 提示刺激の準備

		int pcnt = 0;
		for (j = -1; j <= 1; j++) {
			for (i = -1; i <= 1; i++) {
				if (i == 0 && j == 0) continue; // 画面中央は別の記号を提示するのでスキップ
				px = basei + i*dpix; // 今回の提示位置
				py = basej + j*dpix; // 今回の提示位置
				k = st[tr][pcnt];    // 提示位置pcntへ提示する刺激番号の設定
				cv::putText(background, (const char*)mark[k], cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0, 0, 0), 3, CV_AA);
				pcnt++; // 次の提示位置(0,1,2,・・・,7)
			}//for(i=
		}//for(j=

		 // 画面中央へ注意誘導用の矢印記号を提示
		int att_dir;
		for (j = 0; j < h; j++) {
			for (i = 0; i < w; i++) {
				if (att[tr] == 0) { // 上方
					background.data[(DH_SIZE*(basej - h + j) + basei + i) * 3 + 0] = upper.data[j*w + i];
					background.data[(DH_SIZE*(basej - h + j) + basei + i) * 3 + 1] = upper.data[j*w + i];
					background.data[(DH_SIZE*(basej - h + j) + basei + i) * 3 + 2] = upper.data[j*w + i];
				}

				if (att[tr] == 1) { //左方向
					background.data[(DH_SIZE*(basej - h + j) + basei + i) * 3 + 0] = left.data[j*w + i];
					background.data[(DH_SIZE*(basej - h + j) + basei + i) * 3 + 1] = left.data[j*w + i];
					background.data[(DH_SIZE*(basej - h + j) + basei + i) * 3 + 2] = left.data[j*w + i];
				}

				if (att[tr] == 2) { //下方
					background.data[(DH_SIZE*(basej - h + j) + basei + i) * 3 + 0] = down.data[j*w + i];
					background.data[(DH_SIZE*(basej - h + j) + basei + i) * 3 + 1] = down.data[j*w + i];
					background.data[(DH_SIZE*(basej - h + j) + basei + i) * 3 + 2] = down.data[j*w + i];
				}

				if (att[tr] == 3) { //右方向
					background.data[(DH_SIZE*(basej - h + j) + basei + i) * 3 + 0] = right.data[j*w + i];
					background.data[(DH_SIZE*(basej - h + j) + basei + i) * 3 + 1] = right.data[j*w + i];
					background.data[(DH_SIZE*(basej - h + j) + basei + i) * 3 + 2] = right.data[j*w + i];
				}
			}
		}

		//px = basei; // 提示位置(横）
		//py = basej; // 提示位置(縦)
		//cv::putText(background, "+", cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(128, 128, 128), 3, CV_AA);

		// 刺激提示
		//startime = (double)cvGetTickCount();  // 開始時刻の取得
		cv::imshow(window_info, background);    // 画像表示
												//cv::waitKey(30);                      // 刺激画像の提示

												//刺激提示画面の変化準備
		k0 = prst[tr]; // 変化前の刺激番号
		k1 = chst[tr]; // 変化後の刺激番号
		get_ij(chpos[tr], &i, &j); // 変化位置のi,j番号の取得
		px = basei + i*dpix;       // 変化位置の設定
		py = basej + j*dpix;       // 変化位置の設定

		int result;
		result = cv::waitKey(10 * 1000); // 10秒間ないでキー入力を待つ
		if (result != -1) {     // 10秒間で何かキーが入力されたら
			cv::waitKey(100); // 100ms待機し、瞼が閉じた状態のときに刺激を切り替える
			cv::putText(background, mark[k0], cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(255, 255, 255), 5, CV_AA); // 古い記号を消す
			cv::putText(background, mark[k1], cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0, 0, 0), 3, CV_AA);     // 新しい記号を描く
			cv::imshow(window_info, background);	// 記号表示更新
			cap >> gframe;                          // 更新時点の目領域映像の取得
			cv::waitKey(3 * 1000);                  // 3秒待機
		}

		//10秒間キー入力がなかった場合は、試行を無効とする。
		if (result == -1) {
			fout << tr << "," << -1 << endl; // 無効な試行として記録する
			cv::destroyWindow(window_info);  // windowの破棄
			continue;
		}

		// 目領域画像記録
		std::ostringstream ostr1;
		ostr1 << "_tr" << tr;
		eyename = "";
		eyename = "eye_実験3_" + subnm + ostr1.str() + ".bmp";
		cv::imwrite(eyename, gframe);   // BL時の画像を記録

		cv::destroyWindow(window_info); // windowの破棄


										// 回答入力（コンソールで）
		cout << endl;
		cout << "試行番号:" << tr + 1 << "/" << TR_NUM << endl << endl;
		cout << "------------------------------------------" << endl;
		cout << "どこの位置が変化したか答えてください" << endl;
		cout << "1:左上段、2:中央上段、3:右上段" << endl;
		cout << "4:左中段、　　　　　　5:右中段" << endl;
		cout << "6:左下段、7:中央下段、8:右下段" << endl;
		cout << endl;
		cout << "9:わからない" << endl;
		cin >> answer;

		fout << tr << "," << answer << endl;
	}//for(tr=
	fout.close();
	return 1;
}




//2016.2.28～ for SSII2016
/*-------------------------------*/
/* 刺激変化検出実験3(練習版）    */
/* CB現象を伴う状態での検出実験  */
/* 注意誘導つき                  */
/*-------------------------------*/
//
//  SSII2016用
//　刺激が変化する位置の方向を示す（4方向：上下左右）
//  特定のSW入力と同時に随意性のBLを行う
//  SW入力から100ms後の時点で切り替える
//　切り替え時点の目領域映像を記録しておく
//

int cb_exp3_ssii_ren()
{
	static int st[TR_NUM][8]; // 各試行において8箇所の周辺位置に提示する刺激番号(0,1,2,3)　
	int chpos[TR_NUM];        // 各試行において刺激が変化する周辺位置(0：左上隅,1,・・・,7：右下隅)
	int prst[TR_NUM];         // 変化前の刺激番号
	int chst[TR_NUM];         // 変化時に提示する刺激番号
	int att[TR_NUM];          // 注意の誘導方向

	int tr;
	int basei, basej, dpix;
	int px, py;
	int i, j, k;
	int k0, k1;
	int answer;
	int itmp1, itmp2, itmp3, itmp4, itmp5;
	int w, h;
	double dtmp;
	char split;

	std::string subnm; // 実験参加者名
	std::string sbuf, sbuf1, sbuf2, sbuf3, sbuf4;  // バッファ
	cv::Mat frame, gray;
	cv::Mat right, left, down, upper; // 注意の方向を示す矢印画像

									  // 矢印画像の読み込み
	right = cv::imread("right_dir.bmp", cv::IMREAD_GRAYSCALE);
	if (right.empty()) {
		cout << "ファイルがありません" << endl;
		return 0;
	}

	left = cv::imread("left_dir.bmp", cv::IMREAD_GRAYSCALE);
	if (left.empty()) {
		cout << "ファイルがありません" << endl;
		return 0;
	}

	down = cv::imread("down_dir.bmp", cv::IMREAD_GRAYSCALE);
	if (down.empty()) {
		cout << "ファイルがありません" << endl;
		return 0;
	}

	upper = cv::imread("upper_dir.bmp", cv::IMREAD_GRAYSCALE);
	if (upper.empty()) {
		cout << "ファイルがありません" << endl;
		return 0;
	}

	// 矢印画像のサイズ取得
	w = upper.cols; // 幅
	h = upper.rows; // 高さ


	cout << "---刺激変化検出実験3（練習）---" << endl;

	cout << "実験参加者=";
	cin >> subnm;

	// 刺激提示順序データ読み込み
	std::string sfname;
	sfname = "order_";
	sfname += "実験3_練習_" + subnm + ".csv"; // ファイル名の生成

										  // 刺激提示順序データファイルの読み込み
	ifstream fin(sfname);
	if (!fin) {
		cout << "刺激提示用ファイルがありません" << endl;
		return 0;
	}

	// 変化情報の読み取り
	fin >> sbuf; // 1行分読み飛ばし

				 // 試行番号、変化位置、変化前刺激、変化後刺激、誘導方向
	for (tr = 0; tr < TR_NUM; tr++) { // 
		fin >> itmp1 >> split >> itmp2 >> split >> itmp3 >> split >> itmp4 >> split >> itmp5;

		chpos[tr] = itmp2;
		prst[tr] = itmp3;
		chst[tr] = itmp4;
		att[tr] = itmp5;
		//cout << tr+1 << ","<< chpos[tr] << "," << prst[tr] << "," << chst[tr]<< endl;//確認
	}

	// 提示パターン（周辺８箇所）
	fin >> sbuf; // 1行分読み飛ばし

	for (tr = 0; tr<TR_NUM; tr++) {
		fin >> itmp1;// 試行番号読み飛ばし
		for (i = 0; i < 8; i++) {
			fin >> split >> itmp1;
			st[tr][i] = itmp1;
		}
		fin >> split;
	}
	fin.close();

	// 提示情報の読み込み結果の確認
	cout << "試行番号,変化位置,現在刺激番号,変化後刺激番号,注意誘導方向" << endl;
	for (tr = 0; tr < TR_NUM; tr++) {
		cout << tr + 1 << ":" << chpos[tr] << "," << prst[tr] << "," << chst[tr] << "," << att[tr] << endl;
	}
	cout << "試行番号,提示刺激（各周辺位置)" << endl;
	for (tr = 0; tr < TR_NUM; tr++) {
		cout << tr + 1 << ":";

		for (i = 0; i < 8; i++) {
			cout << st[tr][i] << ",";
		}
		cout << endl;
	}

	cout << "練習を始めますか：何かキーを押してください";
	getchar();
	getchar();


	// 試行開始準備

	// 回答情報記録準備
	std::string fname, imname, eyename;
	std::ostringstream ostr;
	fname = "info_";
	fname += "実験3_練習_" + subnm + ".csv"; // ファイル名の生成

										 // 回答情報出力用ファイルの生成
	ofstream fout(fname); // 出力ファイルを作成する
	if (!fout) {
		cout << "ファイルが開きません" << endl;
		return 0;
	}
	fout << "試行番号,変化位置" << endl;

	char *window_info = "info";
	//cv::namedWindow(window_info, CV_WINDOW_AUTOSIZE);							      　// 刺激提示用windowの生成
	//cv::moveWindow(window_info, (DH_SIZE - SW_HSIZE) / 2, (DV_SIZE - SW_VSIZE) / 2);  // 画面中央へ移動
	cv::Mat background(cv::Size(DH_SIZE, DV_SIZE), CV_8UC3, cv::Scalar(255, 255, 255)); // Windowに表示する背景の準備

																						//提示刺激の準備
	char mark[][10] = { "X", "O", "H", "E" }; // １バイト文字を選定


											  //文字提示位置の中心位置（注視点位置）
	basei = DH_SIZE / 2;
	basej = DV_SIZE / 2;

	// 刺激配置の算出
	dtmp = OBDIST*tan(INTDEG*PAI / 180.0);
	dtmp /= PITCH;
	dpix = (int)dtmp;

	// 実験開始!
	for (tr = 0; tr < TR_NUM / 4; tr++) { // TR_NUM/4回実験実施
		cv::Mat background(cv::Size(DH_SIZE, DV_SIZE), CV_8UC3, cv::Scalar(255, 255, 255)); // Windowに表示する背景の準備
		cv::imshow(window_info, background);  // 画像表示
		cv::waitKey(3 * 1000);                // 背景を3秒間提示
											  // 提示刺激の準備

		int pcnt = 0;
		for (j = -1; j <= 1; j++) {
			for (i = -1; i <= 1; i++) {
				if (i == 0 && j == 0) continue; // 画面中央は別の記号を提示するのでスキップ
				px = basei + i*dpix; // 今回の提示位置
				py = basej + j*dpix; // 今回の提示位置
				k = st[tr][pcnt];    // 提示位置pcntへ提示する刺激番号の設定
				cv::putText(background, (const char*)mark[k], cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0, 0, 0), 3, CV_AA);
				pcnt++; // 次の提示位置(0,1,2,・・・,7)
			}//for(i=
		}//for(j=

		 // 画面中央へ注意誘導用の矢印記号を提示
		int att_dir;
		for (j = 0; j < h; j++) {
			for (i = 0; i < w; i++) {
				if (att[tr] == 0) { // 上方
					background.data[(DH_SIZE*(basej - h + j) + basei + i) * 3 + 0] = upper.data[j*w + i];
					background.data[(DH_SIZE*(basej - h + j) + basei + i) * 3 + 1] = upper.data[j*w + i];
					background.data[(DH_SIZE*(basej - h + j) + basei + i) * 3 + 2] = upper.data[j*w + i];
				}

				if (att[tr] == 1) { //左方向
					background.data[(DH_SIZE*(basej - h + j) + basei + i) * 3 + 0] = left.data[j*w + i];
					background.data[(DH_SIZE*(basej - h + j) + basei + i) * 3 + 1] = left.data[j*w + i];
					background.data[(DH_SIZE*(basej - h + j) + basei + i) * 3 + 2] = left.data[j*w + i];
				}

				if (att[tr] == 2) { //下方
					background.data[(DH_SIZE*(basej - h + j) + basei + i) * 3 + 0] = down.data[j*w + i];
					background.data[(DH_SIZE*(basej - h + j) + basei + i) * 3 + 1] = down.data[j*w + i];
					background.data[(DH_SIZE*(basej - h + j) + basei + i) * 3 + 2] = down.data[j*w + i];
				}

				if (att[tr] == 3) { //右方向
					background.data[(DH_SIZE*(basej - h + j) + basei + i) * 3 + 0] = right.data[j*w + i];
					background.data[(DH_SIZE*(basej - h + j) + basei + i) * 3 + 1] = right.data[j*w + i];
					background.data[(DH_SIZE*(basej - h + j) + basei + i) * 3 + 2] = right.data[j*w + i];
				}
			}
		}

		//px = basei; // 提示位置(横）
		//py = basej; // 提示位置(縦)
		//cv::putText(background, "+", cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(128, 128, 128), 3, CV_AA);

		// 刺激提示
		//startime = (double)cvGetTickCount();  // 開始時刻の取得
		cv::imshow(window_info, background);    // 画像表示
												//cv::waitKey(30);                      // 刺激画像の提示

												//刺激提示画面の変化準備
		k0 = prst[tr]; // 変化前の刺激番号
		k1 = chst[tr]; // 変化後の刺激番号
		get_ij(chpos[tr], &i, &j); // 変化位置のi,j番号の取得
		px = basei + i*dpix;       // 変化位置の設定
		py = basej + j*dpix;       // 変化位置の設定

		int result;
		result = cv::waitKey(10 * 1000); // 10秒間ないでキー入力を待つ
		if (result != -1) {     // 10秒間で何かキーが入力されたら
			cv::waitKey(100); // 100ms待機し、瞼が閉じた状態のときに刺激を切り替える
			cv::putText(background, mark[k0], cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(255, 255, 255), 5, CV_AA); // 古い記号を消す
			cv::putText(background, mark[k1], cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0, 0, 0), 3, CV_AA);     // 新しい記号を描く
			cv::imshow(window_info, background);	// 記号表示更新
			cap >> gframe;                          // 更新時点の目領域映像の取得
			cv::waitKey(3 * 1000);                  // 3秒待機
		}

		//10秒間キー入力がなかった場合は、試行を無効とする。
		if (result == -1) {
			fout << tr << "," << -1 << endl; // 無効な試行として記録する
			cv::destroyWindow(window_info);  // windowの破棄
			continue;
		}

		// 目領域画像記録
		std::ostringstream ostr1;
		ostr1 << "_tr" << tr;
		eyename = "";
		eyename = "eye_実験3_練習_" + subnm + ostr1.str() + ".bmp";
		cv::imwrite(eyename, gframe);   // BL時の画像を記録

		cv::destroyWindow(window_info); // windowの破棄


										// 回答入力（コンソールで）
		cout << endl;
		cout << "試行番号:" << tr + 1 << "/" << TR_NUM / 4 << endl << endl;
		cout << "------------------------------------------" << endl;
		cout << "どこの位置が変化したか答えてください" << endl;
		cout << "1:左上段、2:中央上段、3:右上段" << endl;
		cout << "4:左中段、　　　　　　5:右中段" << endl;
		cout << "6:左下段、7:中央下段、8:右下段" << endl;
		cout << endl;
		cout << "9:わからない" << endl;
		cin >> answer;

		fout << tr << "," << answer << endl;
	}//for(tr=
	fout.close();
	return 1;
}












//2016.2.7～ for SSII2016
/*-------------------------------*/
/* 刺激変化検出実験2             */
/* CB現象を伴う状態での検出実験  */
/*-------------------------------*/
//
//  SSII2016用
//　BLを伴う検出実験(CB現象なし）
//  特定のSW入力と同時に随意性のBLを行う
//  SW入力から100ms後の時点で切り替える
//　切り替え時点の目領域映像を記録しておく

int cb_exp2_ssii()
{
	static int st[TR_NUM][8]; // 各試行において8箇所の周辺位置に提示する刺激番号(0,1,2,3)　
	int chpos[TR_NUM];        // 各試行において刺激が変化する周辺位置(0：左上隅,1,・・・,7：右下隅)
	int prst[TR_NUM];         // 変化前の刺激番号
	int chst[TR_NUM];         // 変化時に提示する刺激番号

	int tr;
	int basei, basej, dpix;
	int px, py;
	int i, j, k;
	int k0, k1;
	int answer;
	int itmp1, itmp2, itmp3, itmp4;
	double dtmp;
	char split;

	std::string subnm; // 実験参加者名
	std::string sbuf, sbuf1, sbuf2, sbuf3, sbuf4;  // バッファ
	cv::Mat frame, gray;


	cout << "---刺激変化検出実験2---" << endl;

	cout << "実験参加者=";
	cin >> subnm;

	// 刺激提示順序データ読み込み
	std::string sfname;
	sfname = "order_";
	sfname += "実験2_" + subnm + ".csv"; // ファイル名の生成

									   // 刺激提示順序データファイルの読み込み
	ifstream fin(sfname);
	if (!fin) {
		cout << "刺激提示用ファイルがありません" << endl;
		return 0;
	}

	// 変化情報の読み取り
	fin >> sbuf; // 1行分読み飛ばし

				 // 試行番号、変化位置、変化前刺激、変化後刺激
	for (tr = 0; tr < TR_NUM; tr++) {
		fin >> itmp1 >> split >> itmp2 >> split >> itmp3 >> split >> itmp4;
		chpos[tr] = itmp2;
		prst[tr] = itmp3;
		chst[tr] = itmp4;
		//cout << tr+1 << ","<< chpos[tr] << "," << prst[tr] << "," << chst[tr]<< endl;//確認
	}

	// 提示パターン（周辺８箇所）
	fin >> sbuf; // 1行分読み飛ばし

	for (tr = 0; tr<TR_NUM; tr++) {
		fin >> itmp1;// 試行番号読み飛ばし
		for (i = 0; i < 8; i++) {
			fin >> split >> itmp1;
			st[tr][i] = itmp1;
		}
		fin >> split;
	}
	fin.close();

	// 提示情報の読み込み結果の確認
	cout << "試行番号,変化位置,現在刺激番号,変化後刺激番号" << endl;
	for (tr = 0; tr < TR_NUM; tr++) {
		cout << tr + 1 << ":" << chpos[tr] << "," << prst[tr] << "," << chst[tr] << endl;
	}
	cout << "試行番号,提示刺激（各周辺位置)" << endl;
	for (tr = 0; tr < TR_NUM; tr++) {
		cout << tr + 1 << ":";

		for (i = 0; i < 8; i++) {
			cout << st[tr][i] << ",";
		}
		cout << endl;
	}

	cout << "実験を始めますか：何かキーを押してください";
	getchar();
	getchar();


	// 試行開始準備

	// 回答情報記録準備
	std::string fname, imname, eyename;
	std::ostringstream ostr;
	fname = "info_";
	fname += "実験2_" + subnm + ".csv"; // ファイル名の生成

									  // 回答情報出力用ファイルの生成
	ofstream fout(fname); // 出力ファイルを作成する
	if (!fout) {
		cout << "ファイルが開きません" << endl;
		return 0;
	}
	fout << "試行番号,変化位置" << endl;

	char *window_info = "info";
	//cv::namedWindow(window_info, CV_WINDOW_AUTOSIZE);							      　// 刺激提示用windowの生成
	//cv::moveWindow(window_info, (DH_SIZE - SW_HSIZE) / 2, (DV_SIZE - SW_VSIZE) / 2);  // 画面中央へ移動
	cv::Mat background(cv::Size(DH_SIZE, DV_SIZE), CV_8UC3, cv::Scalar(255, 255, 255)); // Windowに表示する背景の準備

																						//提示刺激の準備
	char mark[][10] = { "X", "O", "H", "E" }; // １バイト文字を選定


											  //文字提示位置の中心位置（注視点位置）
	basei = DH_SIZE / 2;
	basej = DV_SIZE / 2;

	// 刺激配置の算出
	dtmp = OBDIST*tan(INTDEG*PAI / 180.0);
	dtmp /= PITCH;
	dpix = (int)dtmp;

	// 実験開始!
	for (tr = 0; tr < TR_NUM; tr++) { // TR_NUM回実験実施
		cv::Mat background(cv::Size(DH_SIZE, DV_SIZE), CV_8UC3, cv::Scalar(255, 255, 255)); // Windowに表示する背景の準備
		cv::imshow(window_info, background);  // 画像表示
		cv::waitKey(3 * 1000);                // 背景を3秒間提示
											  // 提示刺激の準備

		int pcnt = 0;
		for (j = -1; j <= 1; j++) {
			for (i = -1; i <= 1; i++) {
				if (i == 0 && j == 0) continue; // 画面中央は別の記号を提示するのでスキップ
				px = basei + i*dpix; // 今回の提示位置
				py = basej + j*dpix; // 今回の提示位置
				k = st[tr][pcnt];    // 提示位置pcntへ提示する刺激番号の設定
				cv::putText(background, (const char*)mark[k], cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0, 0, 0), 3, CV_AA);
				pcnt++; // 次の提示位置(0,1,2,・・・,7)
			}//for(i=
		}//for(j=

		 // 画面中央への記号（注視店提示）
		px = basei; // 提示位置(横）
		py = basej; // 提示位置(縦)
		cv::putText(background, "+", cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(128, 128, 128), 3, CV_AA);

		// 刺激提示
		//startime = (double)cvGetTickCount();  // 開始時刻の取得
		cv::imshow(window_info, background);    // 画像表示
												//cv::waitKey(30);                      // 刺激画像の提示

												//刺激提示画面の変化準備
		k0 = prst[tr]; // 変化前の刺激番号
		k1 = chst[tr]; // 変化後の刺激番号
		get_ij(chpos[tr], &i, &j); // 変化位置のi,j番号の取得
		px = basei + i*dpix;       // 変化位置の設定
		py = basej + j*dpix;       // 変化位置の設定

		int result;
		result = cv::waitKey(10 * 1000); // 10秒間ないでキー入力を待つ
		if (result != -1) {     // 10秒間で何かキーが入力されたら
			cv::waitKey(100); // 100ms待機し、瞼が閉じた状態のときに刺激を切り替える
			cv::putText(background, mark[k0], cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(255, 255, 255), 5, CV_AA); // 古い記号を消す
			cv::putText(background, mark[k1], cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0, 0, 0), 3, CV_AA);     // 新しい記号を描く
			cv::imshow(window_info, background);	// 記号表示更新
			cap >> gframe;                          // 更新時点の目領域映像の取得
			cv::waitKey(3 * 1000);                  // 3秒待機
		}

		//10秒間キー入力がなかった場合は、試行を無効とする。
		if (result == -1) {
			fout << tr << "," << -1 << endl; // 無効な試行として記録する
			cv::destroyWindow(window_info);  // windowの破棄
			continue;
		}

		// 目領域画像記録
		std::ostringstream ostr1;
		ostr1 << "_tr" << tr;
		eyename = "";
		eyename = "eye_実験2_" + subnm + ostr1.str() + ".bmp";
		cv::imwrite(eyename, gframe);   // BL時の画像を記録

		cv::destroyWindow(window_info); // windowの破棄


										// 回答入力（コンソールで）
		cout << endl;
		cout << "試行番号:" << tr + 1 << "/" << TR_NUM << endl << endl;
		cout << "------------------------------------------" << endl;
		cout << "どこの位置が変化したか答えてください" << endl;
		cout << "1:左上段、2:中央上段、3:右上段" << endl;
		cout << "4:左中段、　　　　　　5:右中段" << endl;
		cout << "6:左下段、7:中央下段、8:右下段" << endl;
		cout << endl;
		cout << "9:わからない" << endl;
		cin >> answer;

		fout << tr << "," << answer << endl;
	}//for(tr=
	fout.close();
	return 1;
}



//2016.2.7～ for SSII2016
/*-------------------------------*/
/* 刺激変化検出実験2(練習）      */
/* CB現象を伴う状態での検出実験  */
/*-------------------------------*/
//
//  練習モード
//
//  SSII2016用
//　BLを伴う検出実験(CB現象なし）
//  特定のSW入力と同時に随意性のBLを行う
//  SW入力から100ms後の時点で切り替える
//　切り替え時点の目領域映像を記録しておく

int cb_exp2_ssii_ren()
{
	static int st[TR_NUM][8]; // 各試行において8箇所の周辺位置に提示する刺激番号(0,1,2,3)　
	int chpos[TR_NUM];        // 各試行において刺激が変化する周辺位置(0：左上隅,1,・・・,7：右下隅)
	int prst[TR_NUM];         // 変化前の刺激番号
	int chst[TR_NUM];         // 変化時に提示する刺激番号

	int tr;
	int basei, basej, dpix;
	int px, py;
	int i, j, k;
	int k0, k1;
	int answer;
	int itmp1, itmp2, itmp3, itmp4;
	double dtmp;
	char split;

	std::string subnm; // 実験参加者名
	std::string sbuf, sbuf1, sbuf2, sbuf3, sbuf4;  // バッファ
	cv::Mat frame, gray;


	cout << "---刺激変化検出実験2---" << endl;

	cout << "実験参加者=";
	cin >> subnm;

	// 刺激提示順序データ読み込み
	std::string sfname;
	sfname = "order_";
	sfname += "実験2_練習_" + subnm + ".csv"; // ファイル名の生成

										  // 刺激提示順序データファイルの読み込み
	ifstream fin(sfname);
	if (!fin) {
		cout << "刺激提示用ファイルがありません" << endl;
		return 0;
	}

	// 変化情報の読み取り
	fin >> sbuf; // 1行分読み飛ばし

				 // 試行番号、変化位置、変化前刺激、変化後刺激
	for (tr = 0; tr < TR_NUM; tr++) { // 
		fin >> itmp1 >> split >> itmp2 >> split >> itmp3 >> split >> itmp4;
		chpos[tr] = itmp2;
		prst[tr] = itmp3;
		chst[tr] = itmp4;
		//cout << tr+1 << ","<< chpos[tr] << "," << prst[tr] << "," << chst[tr]<< endl;//確認
	}

	// 提示パターン（周辺８箇所）
	fin >> sbuf; // 1行分読み飛ばし

	for (tr = 0; tr<TR_NUM; tr++) {
		fin >> itmp1;// 試行番号読み飛ばし
		for (i = 0; i < 8; i++) {
			fin >> split >> itmp1;
			st[tr][i] = itmp1;
		}
		fin >> split;
	}
	fin.close();

	// 提示情報の読み込み結果の確認
	cout << "試行番号,変化位置,現在刺激番号,変化後刺激番号" << endl;
	for (tr = 0; tr < TR_NUM; tr++) {
		cout << tr + 1 << ":" << chpos[tr] << "," << prst[tr] << "," << chst[tr] << endl;
	}
	cout << "試行番号,提示刺激（各周辺位置)" << endl;
	for (tr = 0; tr < TR_NUM; tr++) {
		cout << tr + 1 << ":";

		for (i = 0; i < 8; i++) {
			cout << st[tr][i] << ",";
		}
		cout << endl;
	}

	cout << "練習を始めますか：何かキーを押してください";
	getchar();
	getchar();


	// 試行開始準備

	// 回答情報記録準備
	std::string fname, imname, eyename;
	std::ostringstream ostr;
	fname = "info_";
	fname += "実験2_練習_" + subnm + ".csv"; // ファイル名の生成

										 // 回答情報出力用ファイルの生成
	ofstream fout(fname); // 出力ファイルを作成する
	if (!fout) {
		cout << "ファイルが開きません" << endl;
		return 0;
	}
	fout << "試行番号,変化位置" << endl;

	char *window_info = "info";
	//cv::namedWindow(window_info, CV_WINDOW_AUTOSIZE);							      　// 刺激提示用windowの生成
	//cv::moveWindow(window_info, (DH_SIZE - SW_HSIZE) / 2, (DV_SIZE - SW_VSIZE) / 2);  // 画面中央へ移動
	cv::Mat background(cv::Size(DH_SIZE, DV_SIZE), CV_8UC3, cv::Scalar(255, 255, 255)); // Windowに表示する背景の準備

																						//提示刺激の準備
	char mark[][10] = { "X", "O", "H", "E" }; // １バイト文字を選定


											  //文字提示位置の中心位置（注視点位置）
	basei = DH_SIZE / 2;
	basej = DV_SIZE / 2;

	// 刺激配置の算出
	dtmp = OBDIST*tan(INTDEG*PAI / 180.0);
	dtmp /= PITCH;
	dpix = (int)dtmp;

	// 実験開始!
	for (tr = 0; tr < TR_NUM / 4; tr++) { // TR_NUM/4回実験実施(本実験の1/4)
		cv::Mat background(cv::Size(DH_SIZE, DV_SIZE), CV_8UC3, cv::Scalar(255, 255, 255)); // Windowに表示する背景の準備
		cv::imshow(window_info, background);  // 画像表示
		cv::waitKey(3 * 1000);                // 背景を3秒間提示
											  // 提示刺激の準備

		int pcnt = 0;
		for (j = -1; j <= 1; j++) {
			for (i = -1; i <= 1; i++) {
				if (i == 0 && j == 0) continue; // 画面中央は別の記号を提示するのでスキップ
				px = basei + i*dpix; // 今回の提示位置
				py = basej + j*dpix; // 今回の提示位置
				k = st[tr][pcnt];    // 提示位置pcntへ提示する刺激番号の設定
				cv::putText(background, (const char*)mark[k], cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0, 0, 0), 3, CV_AA);
				pcnt++; // 次の提示位置(0,1,2,・・・,7)
			}//for(i=
		}//for(j=

		 // 画面中央への記号（注視店提示）
		px = basei; // 提示位置(横）
		py = basej; // 提示位置(縦)
		cv::putText(background, "+", cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(128, 128, 128), 3, CV_AA);

		// 刺激提示
		//startime = (double)cvGetTickCount();  // 開始時刻の取得
		cv::imshow(window_info, background);    // 画像表示
												//cv::waitKey(30);                      // 刺激画像の提示

												//刺激提示画面の変化準備
		k0 = prst[tr]; // 変化前の刺激番号
		k1 = chst[tr]; // 変化後の刺激番号
		get_ij(chpos[tr], &i, &j); // 変化位置のi,j番号の取得
		px = basei + i*dpix;       // 変化位置の設定
		py = basej + j*dpix;       // 変化位置の設定

		int result;
		result = cv::waitKey(10 * 1000); // 10秒間ないでキー入力を待つ
		if (result != -1) {     // 10秒間で何かキーが入力されたら
			cv::waitKey(100); // 100ms待機し、瞼が閉じた状態のときに刺激を切り替える
			cv::putText(background, mark[k0], cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(255, 255, 255), 5, CV_AA); // 古い記号を消す
			cv::putText(background, mark[k1], cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0, 0, 0), 3, CV_AA);     // 新しい記号を描く
			cv::imshow(window_info, background);	// 記号表示更新
			cap >> gframe;                          // 更新時点の目領域映像の取得
			cv::waitKey(3 * 1000);                  // 3秒待機
		}

		//10秒間キー入力がなかった場合は、試行を無効とする。
		if (result == -1) {
			fout << tr << "," << -1 << endl; // 無効な試行として記録する
			cv::destroyWindow(window_info);  // windowの破棄
			continue;
		}

		// 目領域画像記録
		std::ostringstream ostr1;
		ostr1 << "_tr" << tr;
		eyename = "";
		eyename = "eye_実験2_練習_" + subnm + ostr1.str() + ".bmp";
		cv::imwrite(eyename, gframe);   // BL時の画像を記録

		cv::destroyWindow(window_info); // windowの破棄


										// 回答入力（コンソールで）
		cout << endl;
		cout << "試行番号:" << tr + 1 << "/" << TR_NUM / 4 << endl << endl;
		cout << "------------------------------------------" << endl;
		cout << "どこの位置が変化したか答えてください" << endl;
		cout << "1:左上段、2:中央上段、3:右上段" << endl;
		cout << "4:左中段、　　　　　　5:右中段" << endl;
		cout << "6:左下段、7:中央下段、8:右下段" << endl;
		cout << endl;
		cout << "9:わからない" << endl;
		cin >> answer;

		fout << tr << "," << answer << endl;
	}//for(tr=
	fout.close();
	return 1;
}









//2016.2.6～ for SSII2016
/*-------------------------------*/
/* 刺激変化検出実験１            */
/* CB現象のない状態での検出実験  */
/*-------------------------------*/
//
//  SSII2016用
//　BLなしでの検出実験(CB現象なし）
//  運動検知条件
//
int cb_exp1_ssii()
{
	static int st[TR_NUM][8]; // 各試行において8箇所の周辺位置に提示する刺激番号(0,1,2,3)　
	int chpos[TR_NUM];        // 各試行において刺激が変化する周辺位置(0：左上隅,1,・・・,7：右下隅)
	int prst[TR_NUM];         // 変化前の刺激番号
	int chst[TR_NUM];         // 変化時に提示する刺激番号

	int tr;
	int basei, basej, dpix;
	int px, py;
	int i, j, k;
	int k0, k1;
	int answer;
	int itmp1, itmp2, itmp3, itmp4;
	double dtmp;
	char split;

	std::string subnm; // 実験参加者名
	std::string sbuf, sbuf1, sbuf2, sbuf3, sbuf4;  // バッファ
	cv::Mat frame, gray;


	cout << "---刺激変化検出実験１---" << endl;

	cout << "実験参加者=";
	cin >> subnm;

	// 刺激提示順序データ読み込み
	std::string sfname;
	sfname = "order_";
	sfname += "実験1_" + subnm + ".csv"; // ファイル名の生成

									   // 刺激提示順序データファイルの読み込み
	ifstream fin(sfname);
	if (!fin) {
		cout << "刺激提示用ファイルがありません" << endl;
		return 0;
	}

	// 変化情報の読み取り
	fin >> sbuf; // 1行分読み飛ばし

				 // 試行番号、変化位置、変化前刺激、変化後刺激
	for (tr = 0; tr < TR_NUM; tr++) {
		fin >> itmp1 >> split >> itmp2 >> split >> itmp3 >> split >> itmp4;
		chpos[tr] = itmp2;
		prst[tr] = itmp3;
		chst[tr] = itmp4;
		//cout << tr+1 << ","<< chpos[tr] << "," << prst[tr] << "," << chst[tr]<< endl;//確認
	}

	// 提示パターン（周辺８箇所）
	fin >> sbuf; // 1行分読み飛ばし

	for (tr = 0; tr<TR_NUM; tr++) {
		fin >> itmp1;// 試行番号読み飛ばし
		for (i = 0; i < 8; i++) {
			fin >> split >> itmp1;
			st[tr][i] = itmp1;
		}
		fin >> split;
	}
	fin.close();

	// 提示情報の読み込み結果の確認
	cout << "試行番号,変化位置,現在刺激番号,変化後刺激番号" << endl;
	for (tr = 0; tr < TR_NUM; tr++) {
		cout << tr + 1 << ":" << chpos[tr] << "," << prst[tr] << "," << chst[tr] << endl;
	}
	cout << "試行番号,提示刺激（各周辺位置)" << endl;
	for (tr = 0; tr < TR_NUM; tr++) {
		cout << tr + 1 << ":";
		for (i = 0; i < 8; i++) {
			cout << st[tr][i] << ",";
		}
		cout << endl;
	}

	cout << "実験を始めますか：何かキーを押してください";
	getchar();


	// 試行開始準備

	// 回答情報記録準備
	std::string fname, imname;
	std::ostringstream ostr;
	fname = "info_";
	fname += "実験1_" + subnm + ".csv"; // ファイル名の生成

									  // 回答情報出力用ファイルの生成
	ofstream fout(fname); // 出力ファイルを作成する
	if (!fout) {
		cout << "ファイルが開きません" << endl;
		return 0;
	}
	fout << "試行番号,変化位置" << endl;

	char *window_info = "info";
	//cv::namedWindow(window_info, CV_WINDOW_AUTOSIZE);							      　// 刺激提示用windowの生成
	//cv::moveWindow(window_info, (DH_SIZE - SW_HSIZE) / 2, (DV_SIZE - SW_VSIZE) / 2);  // 画面中央へ移動
	cv::Mat background(cv::Size(DH_SIZE, DV_SIZE), CV_8UC3, cv::Scalar(255, 255, 255)); // Windowに表示する背景の準備

																						//提示刺激の準備
	char mark[][10] = { "X", "O", "H", "E" }; // １バイト文字を選定


											  //文字提示位置の中心位置（注視点位置）
	basei = DH_SIZE / 2;
	basej = DV_SIZE / 2;

	// 刺激配置の算出
	dtmp = OBDIST*tan(INTDEG*PAI / 180.0);
	dtmp /= PITCH;
	dpix = (int)dtmp;

	// 実験開始!
	for (tr = 0; tr < TR_NUM; tr++) { // TR_NUM回実験実施
		cv::Mat background(cv::Size(DH_SIZE, DV_SIZE), CV_8UC3, cv::Scalar(255, 255, 255)); // Windowに表示する背景の準備
		cv::imshow(window_info, background);  // 画像表示
		cv::waitKey(3 * 1000);                  // 背景を3秒間提示
												// 提示刺激の準備

		int pcnt = 0;
		for (j = -1; j <= 1; j++) {
			for (i = -1; i <= 1; i++) {
				if (i == 0 && j == 0) continue; // 画面中央は別の記号を提示するのでスキップ
				px = basei + i*dpix; // 今回の提示位置
				py = basej + j*dpix; // 今回の提示位置
				k = st[tr][pcnt];    // 提示位置pcntへ提示する刺激番号の設定
				cv::putText(background, (const char*)mark[k], cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0, 0, 0), 3, CV_AA);
				pcnt++; // 次の提示位置(0,1,2,・・・,7)
			}//for(i=
		}//for(j=

		 // 画面中央への記号（注視店提示）
		px = basei; // 提示位置(横）
		py = basej; // 提示位置(縦)
		cv::putText(background, "+", cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(128, 128, 128), 3, CV_AA);

		// 刺激提示
		startime = (double)cvGetTickCount();  // 開始時刻の取得
		cv::imshow(window_info, background);  // 画像表示
		cv::waitKey(3 * 1000);                  // 刺激画像を3秒間提示

												//刺激提示画面の変化
		k0 = prst[tr]; // 変化前の刺激番号
		k1 = chst[tr]; // 変化後の刺激番号
		get_ij(chpos[tr], &i, &j); // 変化位置のi,j番号の取得
		px = basei + i*dpix;       // 変化位置の設定
		py = basej + j*dpix;       // 変化位置の設定

		cv::putText(background, mark[k0], cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(255, 255, 255), 5, CV_AA); // 古い記号を消す
		cv::putText(background, mark[k1], cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0, 0, 0), 3, CV_AA);       // 新しい記号を描く
		cv::imshow(window_info, background);	// 記号表示更新
		cv::waitKey(3 * 1000);                  // 3秒待つ
		cv::destroyWindow(window_info);        // windowの破棄


											   // 回答入力（コンソールで）
		cout << endl;
		cout << "試行番号:" << tr + 1 << "/" << TR_NUM << endl << endl;
		cout << "------------------------------------------" << endl;
		cout << "どこの位置が変化したか答えてください" << endl;
		cout << "1:左上段、2:中央上段、3:右上段" << endl;
		cout << "4:左中段、　　　　　　5:右中段" << endl;
		cout << "6:左下段、7:中央下段、8:右下段" << endl;
		cout << endl;
		cout << "9:わからない" << endl;
		cin >> answer;

		fout << tr << "," << answer << endl;
	}//for(tr=
	fout.close();
	return 1;
}



//2016.2.6～ for SSII2016
/*-------------------------------*/
/* 刺激変化検出実験１(練習）     */
/* CB現象のない状態での検出実験  */
/*-------------------------------*/
//
// 練習
//
//  SSII2016用
//　BLなしでの検出実験(CB現象なし）
//  運動検知条件
//
int cb_exp1_ssii_ren()
{
	static int st[TR_NUM][8]; // 各試行において8箇所の周辺位置に提示する刺激番号(0,1,2,3)　
	int chpos[TR_NUM];        // 各試行において刺激が変化する周辺位置(0：左上隅,1,・・・,7：右下隅)
	int prst[TR_NUM];         // 変化前の刺激番号
	int chst[TR_NUM];         // 変化時に提示する刺激番号

	int tr;
	int basei, basej, dpix;
	int px, py;
	int i, j, k;
	int k0, k1;
	int answer;
	int itmp1, itmp2, itmp3, itmp4;
	double dtmp;
	char split;

	std::string subnm; // 実験参加者名
	std::string sbuf, sbuf1, sbuf2, sbuf3, sbuf4;  // バッファ
	cv::Mat frame, gray;


	cout << "---刺激変化検出実験１---" << endl;

	cout << "実験参加者=";
	cin >> subnm;

	// 刺激提示順序データ読み込み
	std::string sfname;
	sfname = "order_";
	sfname += "実験1_練習_" + subnm + ".csv"; // ファイル名の生成

										  // 刺激提示順序データファイルの読み込み
	ifstream fin(sfname);
	if (!fin) {
		cout << "刺激提示用ファイルがありません" << endl;
		return 0;
	}

	// 変化情報の読み取り
	fin >> sbuf; // 1行分読み飛ばし

				 // 試行番号、変化位置、変化前刺激、変化後刺激
	for (tr = 0; tr < TR_NUM; tr++) {
		fin >> itmp1 >> split >> itmp2 >> split >> itmp3 >> split >> itmp4;
		chpos[tr] = itmp2;
		prst[tr] = itmp3;
		chst[tr] = itmp4;
		//cout << tr+1 << ","<< chpos[tr] << "," << prst[tr] << "," << chst[tr]<< endl;//確認
	}

	// 提示パターン（周辺８箇所）
	fin >> sbuf; // 1行分読み飛ばし

	for (tr = 0; tr<TR_NUM; tr++) {
		fin >> itmp1;// 試行番号読み飛ばし
		for (i = 0; i < 8; i++) {
			fin >> split >> itmp1;
			st[tr][i] = itmp1;
		}
		fin >> split;
	}
	fin.close();

	// 提示情報の読み込み結果の確認
	cout << "試行番号,変化位置,現在刺激番号,変化後刺激番号" << endl;
	for (tr = 0; tr < TR_NUM; tr++) {
		cout << tr + 1 << ":" << chpos[tr] << "," << prst[tr] << "," << chst[tr] << endl;
	}
	cout << "試行番号,提示刺激（各周辺位置)" << endl;
	for (tr = 0; tr < TR_NUM; tr++) {
		cout << tr + 1 << ":";
		for (i = 0; i < 8; i++) {
			cout << st[tr][i] << ",";
		}
		cout << endl;
	}

	cout << "練習を始めますか：何かキーを押してください";
	getchar();


	// 試行開始準備

	// 回答情報記録準備
	std::string fname, imname;
	std::ostringstream ostr;
	fname = "info_";
	fname += "実験1_練習_" + subnm + ".csv"; // ファイル名の生成

										 // 回答情報出力用ファイルの生成
	ofstream fout(fname); // 出力ファイルを作成する
	if (!fout) {
		cout << "ファイルが開きません" << endl;
		return 0;
	}
	fout << "試行番号,変化位置" << endl;

	char *window_info = "info";
	//cv::namedWindow(window_info, CV_WINDOW_AUTOSIZE);							      　// 刺激提示用windowの生成
	//cv::moveWindow(window_info, (DH_SIZE - SW_HSIZE) / 2, (DV_SIZE - SW_VSIZE) / 2);  // 画面中央へ移動
	cv::Mat background(cv::Size(DH_SIZE, DV_SIZE), CV_8UC3, cv::Scalar(255, 255, 255)); // Windowに表示する背景の準備

																						//提示刺激の準備
	char mark[][10] = { "X", "O", "H", "E" }; // １バイト文字を選定


											  //文字提示位置の中心位置（注視点位置）
	basei = DH_SIZE / 2;
	basej = DV_SIZE / 2;

	// 刺激配置の算出
	dtmp = OBDIST*tan(INTDEG*PAI / 180.0);
	dtmp /= PITCH;
	dpix = (int)dtmp;

	// 実験開始!
	for (tr = 0; tr < TR_NUM / 9; tr++) { // TR_NUM/9回実験実施
		cv::Mat background(cv::Size(DH_SIZE, DV_SIZE), CV_8UC3, cv::Scalar(255, 255, 255)); // Windowに表示する背景の準備
		cv::imshow(window_info, background);  // 画像表示
		cv::waitKey(3 * 1000);                  // 背景を3秒間提示
												// 提示刺激の準備

		int pcnt = 0;
		for (j = -1; j <= 1; j++) {
			for (i = -1; i <= 1; i++) {
				if (i == 0 && j == 0) continue; // 画面中央は別の記号を提示するのでスキップ
				px = basei + i*dpix; // 今回の提示位置
				py = basej + j*dpix; // 今回の提示位置
				k = st[tr][pcnt];    // 提示位置pcntへ提示する刺激番号の設定
				cv::putText(background, (const char*)mark[k], cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0, 0, 0), 3, CV_AA);
				pcnt++; // 次の提示位置(0,1,2,・・・,7)
			}//for(i=
		}//for(j=

		 // 画面中央への記号（注視店提示）
		px = basei; // 提示位置(横）
		py = basej; // 提示位置(縦)
		cv::putText(background, "+", cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(128, 128, 128), 3, CV_AA);

		// 刺激提示
		startime = (double)cvGetTickCount();  // 開始時刻の取得
		cv::imshow(window_info, background);  // 画像表示
		cv::waitKey(3 * 1000);                  // 刺激画像を3秒間提示

												//刺激提示画面の変化
		k0 = prst[tr]; // 変化前の刺激番号
		k1 = chst[tr]; // 変化後の刺激番号
		get_ij(chpos[tr], &i, &j); // 変化位置のi,j番号の取得
		px = basei + i*dpix;       // 変化位置の設定
		py = basej + j*dpix;       // 変化位置の設定

		cv::putText(background, mark[k0], cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(255, 255, 255), 5, CV_AA); // 古い記号を消す
		cv::putText(background, mark[k1], cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0, 0, 0), 3, CV_AA);       // 新しい記号を描く
		cv::imshow(window_info, background);	// 記号表示更新
		cv::waitKey(3 * 1000);                  // 3秒待つ
		cv::destroyWindow(window_info);        // windowの破棄


											   // 回答入力（コンソールで）
		cout << endl;
		cout << "試行番号:" << tr + 1 << "/" << TR_NUM / 9 << endl << endl;
		cout << "------------------------------------------" << endl;
		cout << "どこの位置が変化したか答えてください" << endl;
		cout << "1:左上段、2:中央上段、3:右上段" << endl;
		cout << "4:左中段、　　　　　　5:右中段" << endl;
		cout << "6:左下段、7:中央下段、8:右下段" << endl;
		cout << endl;
		cout << "9:わからない" << endl;
		cin >> answer;

		fout << tr << "," << answer << endl;
	}//for(tr=
	fout.close();
	return 1;
}


/*------------------------------------*/
/* 周辺位置番号に対するi,j値を取得する*/
/*------------------------------------*/
//　周辺位置:0,1,・・・,7
//
int get_ij(
	int pos,  // 提示位置番号(0,1,2,・・・7)
	int *i0,  // 対応するi番号
	int *j0   // 対応するj番号
)
{
	int i, j;
	int cnt = -1;
	for (j = -1; j <= 1; j++) {
		for (i = -1; i <= 1; i++) {
			if (i == 0 && j == 0) continue; // 中心位置はカウントしない
			cnt++;
			if (cnt == pos) {
				*i0 = i;
				*j0 = j;
				return 1;
			}
		}
	}
	cout << "計算できません" << endl;
	return 0;
}




//2016.1.11～
/*----------------*/
/* CB現象確認実験 */
/*----------------*/
//
// 刺激生成部分の改良(2016.1.16追加）
//
int cb_exp()
{

	static int st[8][TR_NUM]; // 各試行において8箇所の周辺位置に提示する刺激番号(0,1,2,3)
	int chpos[TR_NUM];        // 各試行において刺激が変化する周辺位置(0,1,・・・,7)
	int chst[TR_NUM];         // 変化時に提示する刺激番号
	int fch[4][4];            // 変化パターンの出現回数を把握するための出現記録表（作業用）

	int tr, fr, fr_bl;
	int basei, basej, dpix;
	int px, py;
	int canny_th1, canny_th2, bl_th;
	int i, j, k, is, js, ie, je;
	int i0, j0, k0, k1;
	int cnt, th;
	int answer;
	double dtmp;

	std::string subnm; // 実験参加者名
	std::string sbuf;  // バッファ
	cv::Mat frame, gray;

	cout << "---CB現象確認実験---" << endl;


	//double startime, stoptime; グローバル変数で定義している。

	is = box.x;
	js = box.y;
	ie = box.x + box.width;
	je = box.y + box.height;

	if (is == -1) {
		cout << "検出範囲が設定されていません" << endl;
		return 0;
	}

	cout << "is=" << is << "ie=" << ie << endl;
	cout << "js=" << js << "je=" << je << endl;


	cout << "実験参加者=";
	cin >> subnm;

	cout << "canny閾値1(信頼性の高いエッジ）=";
	cin >> canny_th1;

	cout << "canny閾値2(信頼性の低いエッジ）=";
	cin >> canny_th2;

	cout << "BL検出のための閾値=";
	cin >> bl_th;

	/*　あとで作る(2.6)
	// 刺激提示用データ読み込み
	std::string sfname;
	sfname = "order_";
	//sfname = fname + subnm + ".csv"; // ファイル名の生成


	// 刺激提示データの読み込み用ファイルの生成
	ifstream fin(sfname); // 入力ファイルを作成する
	if (!fin){
	cout << "刺激提示用ファイルがありません" << endl;
	return 0;
	}
	//sfname >> sbuf; // 1行読み飛ばし,ここ要改良
	for (tr = 0; tr < TR_NUM; tr++){
	//sfname >>
	}

	*/

	//提示用Windowの準備(SW_HSIZE×SW_VSIZE画素の大きさに固定）
	char *window_info = "info";
	//cv::namedWindow(window_info, CV_WINDOW_AUTOSIZE);							      // 刺激提示用windowの生成
	//cv::moveWindow(window_info, (DH_SIZE - SW_HSIZE) / 2, (DV_SIZE - SW_VSIZE) / 2);  // 画面中央へ移動
	cv::Mat background(cv::Size(DH_SIZE, DV_SIZE), CV_8UC3, cv::Scalar(255, 255, 255)); // Windowに表示する背景の準備

																						//提示刺激の準備
	char mark[][10] = { "X", "O", "I", "E" }; // １バイト文字を選定
	int mark_num[3][3]; // 提示記号の記憶用配列


						//文字提示位置のベース位置（注視点）
	basei = DH_SIZE / 2;
	basej = DV_SIZE / 2;

	// 刺激配置の算出
	dtmp = OBDIST*tan(INTDEG*PAI / 180.0);
	dtmp /= PITCH;
	dpix = (int)dtmp;
	//cout << dpix;

	// 実験開始!
	//for (tr = 0; tr < TR_NUM; tr++){ // TR_NUM回実験実施
	for (tr = 0; tr < 1; tr++) {
		// 画面上の8箇所に文字を提示する（各文字が同数提示されるように改良する必要あり:2016.1.11)
		srand((unsigned)time(NULL));
		for (j = -1; j <= 1; j++) {
			for (i = -1; i <= 1; i++) {
				if (i == 0 && j == 0) continue; // 画面中央は別の記号を提示する
				px = basei + i*dpix; // 今回の提示位置
				py = basej + j*dpix; // 今回の提示位置
				k = 4 * rand() / RAND_MAX;
				if (k <= 3) { // k=4以上は無効な位置
					mark_num[i + 1][j + 1] = k; // 各提示位置の記号の記憶
					cv::putText(background, (const char*)mark[k], cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0, 0, 0), 3, CV_AA);
				}
			}//for(i=
		}//for(j=

		 // 画面中央への記号提示
		px = basei; // 提示位置(横）
		py = basej; // 提示位置(縦)
		cv::putText(background, "+", cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0, 0, 0), 3, CV_AA);

		// 提示切り替え位置選択(横：i0、縦：j0)
		while (1) {
			i0 = 3 * rand() / RAND_MAX;
			j0 = 3 * rand() / RAND_MAX;
			if (i0 == 1 && j0 == 1) continue; // 中央選択なので無視
			if (i0 >= 3) continue;            // 無効な提示位置
			if (j0 >= 3) continue;            // 無効な提示位置
			k0 = mark_num[i0][j0];            // 切り替え対象位置に提示されている記号の番号の取得
			break;
		}
		// 切り替える記号の選択(k1)
		while (1) {
			k1 = 4 * rand() / RAND_MAX; //
			if (k1 == k0) continue;     // 切り替える前と同じ記号
			if (k1 == 4) continue;      // 選択できない記号
			break;
		}

		// 刺激提示
		startime = (double)cvGetTickCount();  // 開始時刻の取得
		cv::imshow(window_info, background);  // 画像表示
		cv::waitKey(30);                      // 画面表示のための30ms待ち(キー入力イベントへフォーカスを移動しないと画面が表示されない）

											  // 目領域映像キャプチャ開始
											  // 5秒までのBL生起は無視する
		fr = 0; // フレーム番号の初期化
		startime = (double)cvGetTickCount(); // 開始時刻
		while (1) {
			fr++;
			if (fr >= 30 * 60) { // 1分以上(30fps)ならタイムアウト
								 //save_basic_data(edge_sum, fr); // 基本データの書き込み
				break;
			}

			cap >> frame; // 1フレーム読み込み

			cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);	// カラー画像をモノクロ画像に変換
			cv::Canny(gray, gray, canny_th1, canny_th2, 3); // キャニーオペレータによるエッジ抽出

															//cv::rectangle(frame, cv::Point(is, js), cv::Point(ie, je), cv::Scalar(0, 0, 255), 1, 1);    // 関心領域を描画する
															//cv::rectangle(gray, cv::Point(is, js), cv::Point(ie, je), cv::Scalar(255, 255, 255), 1, 1); // 関心領域を描画する

															//cv::imshow(window_input, frame); // 原画像の表示
															//cv::imshow(window_output, gray); // エッジ検出画像

															// エッジ点個数のカウント
			cnt = 0;
			for (j = js; j < je; j++) {
				for (i = is; i < ie; i++) {
					if (gray.data[j * H_SIZE + i] == 255)
						cnt++;
				}
			}
			edge_sum[fr] = cnt;

			// BL判定処理
			if (fr < 30 * 5) { // 試行最初の5秒間は判定処理しない
				cv::waitKey(15);
				continue;
			}

			th = 0;
			for (i = fr - 1; i > fr - 1 - FR_DUR; i--) {// 過去5フレーム分の平均値
				th += edge_sum[i]; // エッジ点数の積算
			}
			th /= FR_DUR;
			itmp[fr] = th; // 確認用
			itmp2[fr] = edge_sum[fr] - th; // 確認用
			if (edge_sum[fr] - th < -bl_th) {		// 上眼瞼の下降時点の検出処理
				fr_bl = fr; // BL検出フレーム:fr_bl
				cout << "BL frame=" << fr_bl;
				//save_basic_data(edge_sum, fr);      // 基本データの格納処理
				//cv::imwrite("closebl.bmp", frame);  // BL時の画像を記録
				break; // 提示終了
			}
			stoptime = (double)cvGetTickCount() - startime; // 終了時刻
			if (cv::waitKey(15) >= 0) { // 15ms内で、何かキーが押されたら終了
				break;
			}
		}//while(1)

		 //刺激提示画面の変化
		if (fr != 30 * 60) { // タイムアウトでなければ提示画面を変化させる
			px = basei + dpix * (i0 - 1); // 今回の提示位置
			py = basej + dpix * (j0 - 1); //
			cv::putText(background, mark[k0], cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(255, 255, 255), 5, CV_AA); // 古い記号を消す
			cv::putText(background, mark[k1], cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0, 0, 255), 3, CV_AA);     // 新しい記号を書く(わかるように今は赤で）
			cv::imshow(window_info, background);	// 記号表示更新
			cv::waitKey(5 * 1000); // 5s待つ
		}

		cv::destroyAllWindows(); // すべてのwindowを破棄

								 // 回答を求める（コンソールで）
		cout << endl;
		cout << "------------------------------------------" << endl;
		cout << "どこの位置が変化したか答えてください" << endl;
		cout << "1:左上段、2:中央上段、3:右上段" << endl;
		cout << "4:左中段、　　　　　　5:右中段" << endl;
		cout << "6:左下段、7:中央下段、8:右下段" << endl;
		cout << endl;
		cout << "9:わからない" << endl;
		cin >> answer;

		// 今回の試行の情報記録
		std::string fname, imname;
		std::ostringstream ostr;
		fname = "info_";
		ostr << "_tr" << tr;
		fname = fname + subnm + ostr.str() + ".csv"; // ファイル名の生成
		imname = subnm + ostr.str() + ".bmp";        // BL時画像格納用ファイル名の生成

													 // 画像を記録する
		cv::imwrite(imname, frame);  // BL時の画像を記録

									 // 情報出力用ファイルの生成
		ofstream fout(fname); // 出力ファイルを作成する
		if (!fout) {
			cout << "ファイルが開きません" << endl;
			return 0;
		}
		fout << "提示文字列" << endl;
		for (j = 0; j <3; j++) {
			for (i = 0; i < 3; i++) {
				if (i == 1 && j == 1) continue;
				k = mark_num[i][j];
				fout << mark[k] << ",";
			}
		}
		fout << endl;

		fout << "変化位置(横),変化位置(縦）,変化文字" << endl;
		fout << i0 << "," << j0 << "," << mark[k1] << endl;
		fout << "回答変化位置(横）,回答変化位置（縦）" << endl;
		if (answer == 1) fout << 0 << "," << 0 << endl;
		if (answer == 2) fout << 1 << "," << 0 << endl;
		if (answer == 3) fout << 2 << "," << 0 << endl;
		if (answer == 4) fout << 0 << "," << 1 << endl;
		if (answer == 5) fout << 2 << "," << 1 << endl;
		if (answer == 6) fout << 0 << "," << 2 << endl;
		if (answer == 7) fout << 1 << "," << 2 << endl;
		if (answer == 8) fout << 2 << "," << 2 << endl;
		if (answer == 9) fout << -1 << "," << -1 << endl;

		fout << "cannyオペレータのパラメータ" << endl;
		fout << "閾値1=" << canny_th1 << endl;
		fout << "閾値2=" << canny_th2 << endl;
		fout << "エッジ点個数の閾値=" << bl_th << endl;

		fout << "BL生起フレーム番号=" << fr_bl << endl;

		fout << "--- 基礎データ---" << endl;
		fout << "fr,エッジ個数,閾値,差分値" << endl;

		for (fr = 1; fr <= fr_bl; fr++) {
			fout << fr << "," << edge_sum[fr] << "," << itmp[fr] << "," << itmp2[fr] << endl;
		}
		fout << "試行時間 =";
		fout << stoptime / ((double)cvGetTickFrequency()*1000.) << endl;
		fout.close();

	}//for(tr=

	return 1;
}



//2016.1.16
/*-----------------------------------*/
/* CB実験の準備（提示刺激系列の生成）*/
/*  実験3用（注意誘導条件)           */
/*-----------------------------------*/
//
// 試行回数：8の倍数(変化位置,変化パターンを同じ回数に設定するため）,24の倍数が望ましい
// 提示位置      ：画面中央を除く周辺の8箇所
// 提示刺激の種類：4種類
// 改良：2016.2.6 :提示パターンそれ自体の記録を忘れていた。
// 練習用の系列生成機能追加
//
//

int prep_cb_exp3()
{
	static int st[TR_NUM][8]; // 各試行において8箇所の周辺位置に提示する刺激番号(0,1,2,3)
	int chpos[TR_NUM];        // 各試行において刺激が変化する周辺位置(0,1,・・・,7)
	int chst[TR_NUM];         // 変化時に提示する刺激番号
	int att[TR_NUM];          // 注意誘導の方向(0:上方,1:左方,2:下方,3:右方)
	int fch[4][4];            // 変化パターンの出現回数を把握するための出現記録表（作業用）

	int tr, i, j, p0, p1, itmp;
	char ren;
	std::string subnm;      // 実験参加者名
	std::string expnum = "3"; // 実験番号3で固定

							  //cout << "刺激系列は生成済みです" << endl; //2016.2.9 刺激は生成済み
							  //return 1;

	cout << "実験参加者=";
	cin >> subnm;

	//cout << "実験番号=";
	//cin >> expnum;

	cout << "練習用(y / n)";
	cin >> ren;

	cout << "---刺激系列の生成中---" << endl;

	// 初期値の設定:刺激の種類(1回の試行で、同じ刺激を2箇所に提示する）
	for (tr = 0; tr < TR_NUM; tr++) {
		for (i = 0; i < 8; i++) { // 周辺提示位置
			st[tr][i] = i % 4;
		}
	}

	// 初期値の設定：変化位置(全試行を通じて選択される周辺位置は同じ回数に設定する）
	for (tr = 0; tr < TR_NUM; tr++) {
		chpos[tr] = tr % 8;
	}

	// ランダム化 
	srand((unsigned)time(NULL));

	// 各試行における刺激提示位置のランダム化
	for (tr = 0; tr < TR_NUM; tr++) {
		for (i = 0; i < 20; i++) {
			p0 = rand() % 8;
			p1 = rand() % 8;
			if (p0 < 8 && p1 < 8) { // 不要かもしれないが念のため
				itmp = st[tr][p0];
				st[tr][p0] = st[tr][p1];
				st[tr][p1] = itmp;
			}
		}//for(i=
	}//for(tr=


	 // 全試行を通じての刺激変化位置のランダム化
	while (1) {
		for (i = 0; i < 200; i++) {
			p0 = rand() % TR_NUM;
			p1 = rand() % TR_NUM;
			if (p0 < TR_NUM && p1 < TR_NUM) { // 不要かもしれないが
				itmp = chpos[p0];
				chpos[p0] = chpos[p1];
				chpos[p1] = itmp;
			}
		}//for(i=
		 // 同じ変化位置が3回続いたら再調整する
		int cnt = 0;
		p0 = chpos[0];
		for (j = 0; j < TR_NUM; j++) {
			if (p0 == chpos[j]) {
				cnt++;
				if (cnt == 3) break;
			}
			else {
				p0 = chpos[j]; // 位置の更新
				cnt = 1;
			}
		}
		if (cnt == 3) continue; // 3回続いたらもう一度ランダム化
		break; // 3回未満で最後まで到達
	}//while(1);


	 // 変化パターンの生成
	int ovlp;
	ovlp = TR_NUM / 6; // 仮設定（同一の変化パターンを許す回数：経験的に設定、値が小さいと処理が終了せず）

	for (i = 0; i < 4; i++) { // 出現記録表の初期化
		for (j = 0; j < 4; j++) {
			fch[i][j] = 0;
		}
	}

	int chp, chs;
	for (tr = 0; tr < TR_NUM; tr++) {
		while (1) {
			chp = chpos[tr];   // 本試行の変化位置
			chs = st[tr][chp]; // 刺激提示位置に変化前に提示される刺激番号
			p0 = rand() % 4;
			if (chs == p0) continue; // 同じ番号では変化しない
			if (fch[chs][p0] < ovlp) {
				chst[tr] = p0; // 変化した後の刺激番号の登録
				fch[chs][p0]++;
				break;
			}
		}//while(1)
	}//for(

	 // 注意誘導方向の設定
	 //
	 // 
	int pos, val;

	int dirptn[TR_NUM];


	// 注意誘導方向の振り分け
	for (pos = 0; pos < 8; pos++) {
		if (pos == 1 || pos == 3 || pos == 4 || pos == 6)
			continue; // 注意誘導の方向は確定しているので無視
		val = 0;// 0,1の値をとる
		for (tr = 0; tr < TR_NUM; tr++) { // 誘導方向の２方向への振り分け(0 or 1で）
			if (chpos[tr] == pos) {
				dirptn[tr] = val;
				val = (val + 1) % 2; // 振り分け先を２方向交互とする。
			}
		}
	}

	// 誘導方向のセット
	for (tr = 0; tr < TR_NUM; tr++) {
		if (chpos[tr] == 0) {  // 0:上方向, 1:左方向
			if (dirptn[tr] == 0)
				att[tr] = 0; // 上方向
			else
				att[tr] = 1; // 左方向
		}

		if (chpos[tr] == 1) { // 0:上方向
			att[tr] = 0;
		}

		if (chpos[tr] == 2) {  // 0:上方向, 3:右方向
			if (dirptn[tr] == 0)
				att[tr] = 0; // 上方向
			else
				att[tr] = 3; // 右方向
		}

		if (chpos[tr] == 3) { // 1:左方向
			att[tr] = 1;
		}

		if (chpos[tr] == 4) { // 3:右方向
			att[tr] = 3;
		}

		if (chpos[tr] == 5) {  // 2:下方向, 1:左方向
			if (dirptn[tr] == 0)
				att[tr] = 2; // 下方向
			else
				att[tr] = 1; // 左方向
		}

		if (chpos[tr] == 6) { // 2:下方向
			att[tr] = 2;
		}

		if (chpos[tr] == 7) {  // 2:下方向, 3:右方向
			if (dirptn[tr] == 0)
				att[tr] = 2; // 下方向
			else
				att[tr] = 3; // 右方向
		}
	} //for(tr=


	  /*
	  int dcnt[4] = { 0 };
	  for(tr = 0; tr < TR_NUM; tr++){
	  dcnt[att[tr]]++;
	  }
	  cout << "0:" << dcnt[0] << "  1:" << dcnt[1] << "  2:" << dcnt[2] << "  3:" << dcnt[3] << endl;
	  */


	  // 刺激生成結果の確認
	cout << "試行番号, 変化位置, 変化前刺激 → 変化後刺激 " << endl;
	for (tr = 0; tr < TR_NUM; tr++) {
		cout << "Tr-" << tr + 1 << " " << chpos[tr] << " " << st[tr][chpos[tr]] << "→" << chst[tr] << endl;
	}

	getchar();
	getchar();

	//刺激系列へのファイルへの格納
	//
	//
	// 今回の試行の情報記録
	std::string fname, imname;
	std::ostringstream ostr;
	fname = "order_";
	if (ren == 'n')
		fname = fname + "実験" + expnum + "_" + subnm + ".csv"; // ファイル名の生成
	else // 練習なら
		fname = fname + "実験" + expnum + "_練習_" + subnm + ".csv"; // ファイル名の生成

																 // 情報出力用ファイルの生成
	ofstream fout(fname); // 出力ファイルを作成する
	if (!fout) {
		cout << "ファイルが開きません" << endl;
		return 0;
	}
	fout << "試行番号,変化位置,変化前刺激,変化後刺激,誘導方向 " << endl; // 誘導方向を加えた(2016.2.28)
	for (tr = 0; tr < TR_NUM; tr++) {
		fout << tr + 1 << "," << chpos[tr] << "," << st[tr][chpos[tr]] << "," << chst[tr] << "," << att[tr] << endl;
	}

	fout << "試行番号,提示刺激系列" << endl;
	for (tr = 0; tr < TR_NUM; tr++) {
		fout << tr + 1 << ",";
		for (i = 0; i < 8; i++) {
			fout << st[tr][i] << ",";
		}
		fout << endl;
	}
	fout.close();

	return 1;
}



//2016.4.10
/*-----------------------------------*/
/* CB実験の準備（提示刺激系列の生成）*/
/*-----------------------------------*/
//
// 試行回数：8の倍数(変化位置,変化パターンを同じ回数に設定するため）,24の倍数が望ましい
// 提示位置      ：画面中央を除く周辺の8箇所
// 提示刺激の種類：4種類
// 改良：2016.2.6 :提示パターンそれ自体の記録を忘れていた。
// 練習用の系列生成機能追加
//
// C部門用に保存するフォルダを変更(2016.4.10～）
// 実験2用

int prep_cb_exp2()
{
	static int st[TR_NUM][8]; // 各試行において8箇所の周辺位置に提示する刺激番号(0,1,2,3)
	int chpos[TR_NUM];        // 各試行において刺激が変化する周辺位置(0,1,・・・,7)
	int chst[TR_NUM];         // 変化時に提示する刺激番号
	int fch[4][4];            // 変化パターンの出現回数を把握するための出現記録表（作業用）
	int intvl[TR_NUM];        // （最大）更新間隔（6,8,10,12s)


	int tr, i, j, p0, p1, itmp;
	char ren;
	std::string subnm;  // 実験参加者名
	std::string expnum; // 実験番号(2:不随意性瞬目実験)


						//cout << "刺激系列は生成済みです" << endl; //2016.4.3 刺激は生成済み
						//return 1;

	cout << "実験参加者=";
	cin >> subnm;

	cout << "実験番号=(2のみ）";
	cin >> expnum;

	// expnumが2以外のとき戻るコードを追加する

	cout << "練習用(y / n)";
	cin >> ren;

	cout << "---刺激系列の生成中---" << endl;

	// 初期値の設定:刺激の種類(1回の試行で、同じ刺激を2箇所に提示する）
	for (tr = 0; tr < TR_NUM; tr++) {
		for (i = 0; i < 8; i++) { // 周辺提示位置
			st[tr][i] = i % 4;
		}
	}

	// 初期値の設定：変化位置(全試行を通じて選択される周辺位置は同じ回数に設定する）
	for (tr = 0; tr < TR_NUM; tr++) {
		chpos[tr] = tr % 8;
	}

	// 初期値の設定：更新間隔(6,8,10,12)
	tr = 0;
	while (1) {
		for (i = 6; i <= 12; i += 2) {
			intvl[tr] = i;
			tr++;
		}
		if (tr == TR_NUM)
			break;
	}

	// ランダム化 
	srand((unsigned)time(NULL));

	// 各試行における刺激提示位置のランダム化
	for (tr = 0; tr < TR_NUM; tr++) {
		for (i = 0; i < 20; i++) {
			p0 = rand() % 8;
			p1 = rand() % 8;
			if (p0 < 8 && p1 < 8) { // 不要かもしれないが念のため
				itmp = st[tr][p0];
				st[tr][p0] = st[tr][p1];
				st[tr][p1] = itmp;
			}
		}//for(i=
	}//for(tr=


	 // 全試行を通じての刺激変化位置のランダム化
	while (1) {
		for (i = 0; i < 200; i++) {
			p0 = rand() % TR_NUM;
			p1 = rand() % TR_NUM;
			if (p0 < TR_NUM && p1 < TR_NUM) { // 不要かもしれないが
				itmp = chpos[p0];
				chpos[p0] = chpos[p1];
				chpos[p1] = itmp;
			}
		}//for(i=
		 // 同じ変化位置が3回続いたら再調整する
		int cnt = 0;
		p0 = chpos[0];
		for (j = 0; j < TR_NUM; j++) {
			if (p0 == chpos[j]) {
				cnt++;
				if (cnt == 3) break;
			}
			else {
				p0 = chpos[j]; // 位置の更新
				cnt = 1;
			}
		}
		if (cnt == 3) continue; // 3回続いたらもう一度ランダム化
		break; // 3回未満で最後まで到達
	}//while(1);


	 // 変化パターンの生成
	int ovlp;
	ovlp = TR_NUM / 6; // 仮設定（同一の変化パターンを許す回数：経験的に設定、値が小さいと処理が終了せず）

	for (i = 0; i < 4; i++) { // 出現記録表の初期化
		for (j = 0; j < 4; j++) {
			fch[i][j] = 0;
		}
	}

	int chp, chs;
	for (tr = 0; tr < TR_NUM; tr++) {
		while (1) {
			chp = chpos[tr];   // 本試行の変化位置
			chs = st[tr][chp]; // 刺激提示位置に変化前に提示される刺激番号
			p0 = rand() % 4;
			if (chs == p0) continue; // 同じ番号では変化しない
			if (fch[chs][p0] < ovlp) {
				chst[tr] = p0; // 変化した後の刺激番号の登録
				fch[chs][p0]++;
				break;
			}
		}//while(1)
	}//for(



	 // 各試行における更新間隔のランダム化
	while (1) {
		for (i = 0; i < 30; i++) {
			p0 = rand() % (TR_NUM + 1);
			p1 = rand() % (TR_NUM + 1);
			if (p0 < TR_NUM && p1 < TR_NUM) { // 不要かもしれないが念のため
				itmp = intvl[p0];
				intvl[p0] = intvl[p1];
				intvl[p1] = itmp;
			}
		}//for(i=

		 // 同じ変化位置が3回続いたら再調整する
		int cnt = 0;
		p0 = intvl[0];
		for (j = 0; j < TR_NUM; j++) {
			if (p0 == intvl[j]) {
				cnt++;
				if (cnt == 3) break;
			}
			else {
				p0 = intvl[j]; // 位置の更新
				cnt = 1;
			}
		}
		if (cnt == 3) continue; // 3回続いたらもう一度ランダム化
		break; // 3回未満で最後まで到達
	}

	// 刺激生成結果の確認
	cout << "試行番号, 変化位置, 変化前刺激 → 変化後刺激,更新間隔 " << endl;
	for (tr = 0; tr < TR_NUM; tr++) {
		cout << "Tr-" << tr + 1 << " " << chpos[tr] << " " << st[tr][chpos[tr]] << "→" << chst[tr] << " " << intvl[tr] << endl;
	}

	getchar();
	getchar();

	//刺激系列へのファイルへの格納
	//
	// 各周辺位置に提示する刺激番号を組み入れていない（要改良）2016.1.16
	//
	// 今回の試行の情報記録
	std::string fname, imname;
	std::ostringstream ostr;

	fname = "c:\\IEEJ_C_2016\\";
	fname = fname + "exp" + expnum + "\\" + subnm + "\\order_";
	if (ren == 'n')
		fname = fname + "実験" + expnum + "_" + subnm + ".csv"; // ファイル名の生成
	else // 練習なら
		fname = fname + "実験" + expnum + "_練習_" + subnm + ".csv"; // ファイル名の生成

																 /*
																 fname = "order_";
																 if (ren == 'n')
																 fname = fname + "実験"+expnum+"_"+subnm + ".csv"; // ファイル名の生成
																 else // 練習なら
																 fname = fname + "実験" + expnum + "_練習_" + subnm + ".csv"; // ファイル名の生成
																 */



																 // 情報出力用ファイルの生成
	ofstream fout(fname); // 出力ファイルを作成する
	if (!fout) {
		cout << fname << endl;
		cout << "ファイルが開きません" << endl;
		getchar();
		getchar();
		return 0;
	}
	fout << "試行番号,変化位置,変化前刺激,変化後刺激,更新間隔 " << endl;
	for (tr = 0; tr < TR_NUM; tr++) {
		fout << tr + 1 << "," << chpos[tr] << "," << st[tr][chpos[tr]] << "," << chst[tr] << "," << intvl[tr] << endl;
	}

	fout << "試行番号,提示刺激系列" << endl;
	for (tr = 0; tr < TR_NUM; tr++) {
		fout << tr + 1 << ",";
		for (i = 0; i < 8; i++) {
			fout << st[tr][i] << ",";
		}
		fout << endl;
	}
	fout.close();

	return 1;
}

















//2016.1.16
/*-----------------------------------*/
/* CB実験の準備（提示刺激系列の生成）*/
/*-----------------------------------*/
//
// 試行回数：8の倍数(変化位置,変化パターンを同じ回数に設定するため）,24の倍数が望ましい
// 提示位置      ：画面中央を除く周辺の8箇所
// 提示刺激の種類：4種類
// 改良：2016.2.6 :提示パターンそれ自体の記録を忘れていた。
// 練習用の系列生成機能追加
//
// C部門2016用に保存するフォルダを変更(2016.4.3～）
// 実験1用

int prep_cb_exp()
{
	static int st[TR_NUM][8]; // 各試行において8箇所の周辺位置に提示する刺激番号(0,1,2,3)
	int chpos[TR_NUM];        // 各試行において刺激が変化する周辺位置(0,1,・・・,7)
	int chst[TR_NUM];         // 変化時に提示する刺激番号
	int fch[4][4];            // 変化パターンの出現回数を把握するための出現記録表（作業用）

	int tr, i, j, p0, p1, itmp;
	char ren;
	std::string subnm;  // 実験参加者名
	std::string expnum; // 実験番号(1:随意性瞬目実験、2:不随意性瞬目実験)


	cout << "刺激系列は生成済みです" << endl; //2016.4.3 刺激は生成済み
	return 1;

	cout << "実験参加者=";
	cin >> subnm;

	cout << "実験番号=";
	cin >> expnum;

	// expnumが3以上のとき戻るコードを追加する

	cout << "練習用(y / n)";
	cin >> ren;

	cout << "---刺激系列の生成中---" << endl;

	// 初期値の設定:刺激の種類(1回の試行で、同じ刺激を2箇所に提示する）
	for (tr = 0; tr < TR_NUM; tr++) {
		for (i = 0; i < 8; i++) { // 周辺提示位置
			st[tr][i] = i % 4;
		}
	}

	// 初期値の設定：変化位置(全試行を通じて選択される周辺位置は同じ回数に設定する）
	for (tr = 0; tr < TR_NUM; tr++) {
		chpos[tr] = tr % 8;
	}

	// ランダム化 
	srand((unsigned)time(NULL));

	// 各試行における刺激提示位置のランダム化
	for (tr = 0; tr < TR_NUM; tr++) {
		for (i = 0; i < 20; i++) {
			p0 = rand() % 8;
			p1 = rand() % 8;
			if (p0 < 8 && p1 < 8) { // 不要かもしれないが念のため
				itmp = st[tr][p0];
				st[tr][p0] = st[tr][p1];
				st[tr][p1] = itmp;
			}
		}//for(i=
	}//for(tr=


	 // 全試行を通じての刺激変化位置のランダム化
	while (1) {
		for (i = 0; i < 200; i++) {
			p0 = rand() % TR_NUM;
			p1 = rand() % TR_NUM;
			if (p0 < TR_NUM && p1 < TR_NUM) { // 不要かもしれないが
				itmp = chpos[p0];
				chpos[p0] = chpos[p1];
				chpos[p1] = itmp;
			}
		}//for(i=
		 // 同じ変化位置が3回続いたら再調整する
		int cnt = 0;
		p0 = chpos[0];
		for (j = 0; j < TR_NUM; j++) {
			if (p0 == chpos[j]) {
				cnt++;
				if (cnt == 3) break;
			}
			else {
				p0 = chpos[j]; // 位置の更新
				cnt = 1;
			}
		}
		if (cnt == 3) continue; // 3回続いたらもう一度ランダム化
		break; // 3回未満で最後まで到達
	}//while(1);


	 // 変化パターンの生成
	int ovlp;
	ovlp = TR_NUM / 6; // 仮設定（同一の変化パターンを許す回数：経験的に設定、値が小さいと処理が終了せず）

	for (i = 0; i < 4; i++) { // 出現記録表の初期化
		for (j = 0; j < 4; j++) {
			fch[i][j] = 0;
		}
	}

	int chp, chs;
	for (tr = 0; tr < TR_NUM; tr++) {
		while (1) {
			chp = chpos[tr];   // 本試行の変化位置
			chs = st[tr][chp]; // 刺激提示位置に変化前に提示される刺激番号
			p0 = rand() % 4;
			if (chs == p0) continue; // 同じ番号では変化しない
			if (fch[chs][p0] < ovlp) {
				chst[tr] = p0; // 変化した後の刺激番号の登録
				fch[chs][p0]++;
				break;
			}
		}//while(1)
	}//for(


	 // 刺激生成結果の確認
	cout << "試行番号, 変化位置, 変化前刺激 → 変化後刺激 " << endl;
	for (tr = 0; tr < TR_NUM; tr++) {
		cout << "Tr-" << tr + 1 << " " << chpos[tr] << " " << st[tr][chpos[tr]] << "→" << chst[tr] << endl;
	}

	getchar();
	getchar();

	//刺激系列へのファイルへの格納
	//
	// 各周辺位置に提示する刺激番号を組み入れていない（要改良）2016.1.16
	//
	// 今回の試行の情報記録
	std::string fname, imname;
	std::ostringstream ostr;

	fname = "c:\\IEEJ_C_2016\\";
	fname = fname + "exp" + expnum + "\\" + subnm + "\\order_";
	if (ren == 'n')
		fname = fname + "実験" + expnum + "_" + subnm + ".csv"; // ファイル名の生成
	else // 練習なら
		fname = fname + "実験" + expnum + "_練習_" + subnm + ".csv"; // ファイル名の生成

																 /*
																 fname = "order_";
																 if (ren == 'n')
																 fname = fname + "実験"+expnum+"_"+subnm + ".csv"; // ファイル名の生成
																 else // 練習なら
																 fname = fname + "実験" + expnum + "_練習_" + subnm + ".csv"; // ファイル名の生成
																 */



																 // 情報出力用ファイルの生成
	ofstream fout(fname); // 出力ファイルを作成する
	if (!fout) {
		cout << fname << endl;
		cout << "ファイルが開きません" << endl;
		return 0;
	}
	fout << "試行番号,変化位置,変化前刺激,変化後刺激 " << endl;
	for (tr = 0; tr < TR_NUM; tr++) {
		fout << tr + 1 << "," << chpos[tr] << "," << st[tr][chpos[tr]] << "," << chst[tr] << endl;
	}

	fout << "試行番号,提示刺激系列" << endl;
	for (tr = 0; tr < TR_NUM; tr++) {
		fout << tr + 1 << ",";
		for (i = 0; i < 8; i++) {
			fout << st[tr][i] << ",";
		}
		fout << endl;
	}
	fout.close();

	return 1;
}

// 2016.5.4
/*---------------*/
/* 高精度タイマー*/
/*---------------*/
//
//
int time_delay(
	int tim // 遅延時間(ms)
)
{
	DWORD dwTime;
	QueryPerformanceFrequency(&nFreq);
	QueryPerformanceCounter(&nBefore);

	while (1) {
		QueryPerformanceCounter(&nAfter);
		dwTime = (DWORD)((nAfter.QuadPart - nBefore.QuadPart) * 1000 / nFreq.QuadPart);
		if ((int)dwTime - tim >= 0) {
			break;
		}
	}
	return 1;
}




/*----- データ分析------*/


// 2016.5.21
/*--------------------*/
/* 正答率算出(実験2)  */
/*--------------------*/
//
// BL生起せずの試行はとりあえず無視する(2016.5.21) 
//
//
int analysis2()
{
	int chpos[TR_NUM4];        // 各試行において刺激が変化する周辺位置(0,1,・・・,7)
	int chst[TR_NUM4];         // 変化時に提示する刺激番号
	int prst[TR_NUM4];         // 変化前の刺激番号
	int intvl[TR_NUM4];        // 提示間隔

	int anspos[TR_NUM4];       // 刺激変化位置（回答）(1,2,3,・・・8, 9:わからない）
	int blon[TR_NUM4];         // BL生起の有無(-1:無，0:有）

	int sn, fn, tr, itmp1, itmp2, itmp3, itmp4, itmp5;
	int trnum;
	char sbuf[100];
	char split;
	std::string sfname, fns;
	std::string subnm[] = { "subA", "subB", "subC", "subD", "subE", "subF", "subG" };
	ifstream fin;
	ofstream fo;


	int exp = 1;


	cout << "---正答率算出（実験2）---" << endl;

	for (sn = 0; sn < 6; sn++) { // 実験参加者6名分
		if (sn != 1) continue;  // subBだけ処理する(2016.5.27)

								// 刺激提示順序データ読み込み
		std::string sfname;

		for (fn = 0; fn <= 3; fn++) { // フォルダ番号
			fns = std::to_string(fn + 1);
			sfname = "c:\\IEEJ_C_2016\\";
			sfname = sfname + "exp2" + "\\" + subnm[sn] + "\\" + subnm[sn] + fns + "\\order_";
			sfname = sfname + "実験2" + "_" + subnm[sn] + fns + ".csv";     // ファイル名の生成

																		  // 刺激提示順序データファイルの読み込み
			ifstream fin(sfname);
			if (!fin) {
				cout << "刺激提示用ファイルがありません" << sfname << endl;
				getchar();
				getchar();
				return 0;
			}

			// 変化情報の読み取り
			fin >> sbuf; // 1行分読み飛ばし

						 // 試行番号、変化位置、変化前刺激、変化後刺激,提示間隔
			for (tr = 0; tr < TR_NUM; tr++) {
				fin >> itmp1 >> split >> itmp2 >> split >> itmp3 >> split >> itmp4 >> split >> itmp5;
				chpos[tr + fn*TR_NUM] = itmp2;
				prst[tr + fn*TR_NUM] = itmp3;
				chst[tr + fn*TR_NUM] = itmp4;
				intvl[tr + fn*TR_NUM] = itmp5;
				//cout << tr+1 << ","<< chpos[tr] << "," << prst[tr] << "," << chst[tr]<< endl;//確認
			}
			fin.close();
		}//for(fn=


		 // 提示情報の読み込み結果の確認
		cout << "試行番号,変化位置,現在刺激番号,変化後刺激番号,提示間隔" << endl;
		for (tr = 0; tr < TR_NUM4; tr++) {
			cout << tr + 1 << ":" << chpos[tr] << "," << prst[tr] << "," << chst[tr] << "," << intvl[tr] << endl;
		}


		// 回答データの読み込み
		sfname = "c:\\IEEJ_C_2016\\exp2\\";
		sfname += subnm[sn] + "\\";
		sfname += "info_実験2_" + subnm[sn] + ".csv"; // ファイル名の生成

													// 回答データファイルの読み込み
		fin.open(sfname);
		if (!fin) {
			cout << "回答データファイル" << sfname << "がありません" << endl;
			return 0;
		}

		// 変化情報の読み取り
		fin >> sbuf; // 1行分読み飛ばし
					 // 試行番号、変化位置
		for (tr = 0; tr < TR_NUM4; tr++) {
			if (fin.eof()) { // 試行回数が一定ではないので，ファイルの最後を探す．
				break;
			}
			fin >> itmp1 >> split >> itmp2 >> split >> itmp3 >> split >> itmp4;
			anspos[tr] = itmp4;
			blon[tr] = itmp3;
			cout << tr + 1 << "," << anspos[tr] << endl;//確認
		}
		fin.close(); // ファイルクローズ

		trnum = tr; // 試行回数の設定

					// 正解率算出処理
					// 分析結果の書き込み
		sfname = "c:\\IEEJ_C_2016\\exp2\\";
		sfname += subnm[sn] + "\\";
		sfname += "anal1_実験2_" + subnm[sn] + ".csv"; // ファイル名の生成
		fo.open(sfname);
		if (!fo) {
			cout << "ファイルが開きません" << endl;
			return 0;
		}

		std::string judge;
		int mukou = 0; // BL非生起試行回数
		int seikai = 0;  //正解数
		int num9 = 0;    // 9（わからない）と答えた回数
		fo << "変化位置,回答位置,判定" << endl;
		for (tr = 0; tr < trnum; tr++) {
			if (blon[tr] == -1) { //BL非生起試行）
				judge = "－";
				mukou++;
			}

			else if (chpos[tr] + 1 == anspos[tr]) {
				judge = "○";
				seikai++;
			}
			else {
				judge = "×";
				if (anspos[tr] == 9)
					num9++;
			}
			fo << chpos[tr] + 1 << "," << anspos[tr] << "," << judge << endl;
		}
		fo << "-----------------------------" << endl;
		fo << "正解,正解率(%)" << endl;
		fo << seikai << "," << 100.0*seikai / (trnum - mukou) << endl;
		fo << "わからない回答数" << endl;
		fo << num9 << endl;
		fo.close();
	}//for(sn=
}








// 2016.5.5
/*--------------------*/
/* 正答率算出(実験1)  */
/*--------------------*/
//
// 無効試行(BL生起せず）を無視する(2016.5.6) 
//
int analysis1()
{
	int chpos[TR_NUM];        // 各試行において刺激が変化する周辺位置(0,1,・・・,7)
	int chst[TR_NUM];         // 変化時に提示する刺激番号
	int prst[TR_NUM];         // 変化前の刺激番号

	int anspos[TR_NUM];       // 刺激変化位置（回答）(1,2,3,・・・8, 9:わからない）


	int sn, tr, itmp1, itmp2, itmp3, itmp4, itmp5;
	char sbuf[100];
	char split;
	std::string sfname;
	std::string subnm[] = { "subA", "subB", "subC", "subD","subE", "subF", "subG" };
	ifstream fin;
	ofstream fo;


	int exp = 1;


	cout << "---正答率算出（実験1）---" << endl;

	for (sn = 1; sn < 6; sn++) { // 実験参加者5名分
								 // 刺激提示順序データ読み込み
		sfname = "c:\\IEEJ_C_2016\\exp1\\";
		sfname += subnm[sn] + "\\";
		sfname += "order_実験1_" + subnm[sn] + ".csv"; // ファイル名の生成

													 // 刺激提示順序データファイルの読み込み
		fin.open(sfname);
		if (!fin) {
			cout << "刺激提示用ファイル" << sfname << "がありません" << endl;
			return 0;
		}

		// 変化情報の読み取り
		fin >> sbuf; // 1行分読み飛ばし

					 // 試行番号、変化位置、変化前刺激、変化後刺激
		for (tr = 0; tr < TR_NUM; tr++) {
			if (exp == 3) { // 実験3
				fin >> itmp1 >> split >> itmp2 >> split >> itmp3 >> split >> itmp4 >> split >> itmp5;
			}
			else {
				fin >> itmp1 >> split >> itmp2 >> split >> itmp3 >> split >> itmp4;
			}
			chpos[tr] = itmp2;
			prst[tr] = itmp3;
			chst[tr] = itmp4;
			cout << tr + 1 << "," << chpos[tr] << "," << prst[tr] << "," << chst[tr] << endl;//確認
		}
		fin.close(); // ファイルクローズ


					 // 回答データの読み込み
		sfname = "c:\\IEEJ_C_2016\\exp1\\";
		sfname += subnm[sn] + "\\";
		sfname += "info_実験1_" + subnm[sn] + ".csv"; // ファイル名の生成

													// 回答データファイルの読み込み
		fin.open(sfname);
		if (!fin) {
			cout << "回答データファイル" << sfname << "がありません" << endl;
			return 0;
		}

		// 変化情報の読み取り
		fin >> sbuf; // 1行分読み飛ばし
					 // 試行番号、変化位置
		for (tr = 0; tr < TR_NUM; tr++) {
			fin >> itmp1 >> split >> itmp2 >> split >> itmp3 >> split >> itmp4;
			anspos[tr] = itmp4;
			cout << tr + 1 << "," << anspos[tr] << endl;//確認
		}
		fin.close(); // ファイルクローズ


					 // 正解率算出処理
					 // 回答データの読み込み
		sfname = "c:\\IEEJ_C_2016\\exp1\\";
		sfname += subnm[sn] + "\\";
		sfname += "anal1_実験1_" + subnm[sn] + ".csv"; // ファイル名の生成
		fo.open(sfname);
		if (!fo) {
			cout << "ファイルが開きません" << endl;
			return 0;
		}

		std::string judge;
		int mukou = 0; // 無効試行回数
		int seikai = 0;  //正解数
		int num9 = 0;    // 9（わからない）と答えた回数
		fo << "変化位置,回答位置,判定" << endl;
		for (tr = 0; tr < TR_NUM; tr++) {
			if (anspos[tr] == -1) { //無効試行（BL検出できず）
				judge = "－";
				mukou++;
			}

			else if (chpos[tr] + 1 == anspos[tr]) {
				judge = "○";
				seikai++;
			}
			else {
				judge = "×";
				if (anspos[tr] == 9)
					num9++;
			}
			fo << chpos[tr] + 1 << "," << anspos[tr] << "," << judge << endl;
		}
		fo << "-----------------------------" << endl;
		fo << "正解,正解率(%)" << endl;
		fo << seikai << "," << 100.0*seikai / (TR_NUM - mukou) << endl;
		fo << "わからない回答数" << endl;
		fo << num9 << endl;
		fo.close();
	}//for(sn=
}