// 2016.1.10 ～
/************************/
/* BL検出＋CB確認実験１ */
/*   for SSII2016       */
/************************/
//  BL検出アルゴリズム組み込んだ（2016.1.10)　
// マウスCallBack関数の組み込み.原型を一応入れてみた(2016.1.10) 
// マウスによる領域設定完成(2016.1.11)
// 刺激生成部分の改良(設定試行回数分生成するタイプにした）（2016.1.16)
// 複数回実験できるように実装する必要あり(対応着手）（2016.1.16)
// releaseモードでコンパイルできるようにパスを指定する(対応済み)
// SSII2016用の実験に組み上げる(2016.2.2着手）
// 刺激検出実験1(CBなし）完成(2016.2.7)
// 練習モードの組み込み(2016.2.9)着手
// 追加実験の組み込み(注意誘導条件）(2016.2.23着手）
// releaseモードだと実験モードでkey入力が正しく受け付けられない.(要改良）
//
//
//

// 2016.3.29 ～
/**********************/
/* 2016年C部門大会用　*/
/**********************/
// 目領域撮影ユニット用プログラムの開発
//
// 実験1:随意性瞬目条件
// 実験2:不随意性瞬目条件
// 実験3:随意性瞬目条件＋注意誘導
// 実験4:不随意性瞬目条件＋注意誘導
//

#include "include.h"
#include "fromTeacherFanc.h"


int main(int argc, char **argv)
{
	int cmd;

	// カメラのチェック
	if (!cap.isOpened()) {
		cerr << "カメラが見つかりません" << endl;
		return 0;
	}

	// フレームレートの設定(30fps)
	//cap.set(CV_CAP_PROP_FPS, 30.0); // これを設定すると動作しない（なぜ？）

	box = cv::Rect(-1, -1, 0, 0);      // BL処理領域の初期値

	while (1) {
		cout << "********** MENU ***********" << endl;
		cout << "                           " << endl;
		cout << " 1:刺激系列生成(実験1用）for C部門2016" << endl;
		cout << " 2:刺激系列生成(実験2用）for C部門2016" << endl;
		cout << "                                " << endl;
		cout << " 3:処理領域の設定" << endl;
		cout << " 4:BL検出閾値の自動設定 for C部門2016" << endl;
		cout << "                           " << endl;
		cout << " 5:刺激変化検出実験1(随意性BL) for C部門2016" << endl;
		cout << " 6:刺激変化検出実験2(自発性BL) for C部門2016" << endl;
		cout << "　　                                            " << endl;
		cout << " 7:刺激変化検出実験1(随意性BL）-練習- for C部門2016" << endl;
		cout << " 8:刺激変化検出実験2(自発性BL）-練習- for C部門2016" << endl;
		cout << "　　                                      " << endl;
		cout << " 9:刺激系列生成(実験3用)" << endl;
		cout << " 10:刺激変化検出実験3(BLあり注意誘導）for SSII2016" << endl;
		cout << " 11:刺激変化検出実験3(注意誘導）-練習-" << endl;
		cout << "                                " << endl;
		cout << " 20:BL検出確認実験2(for 目領域撮影ユニット）" << endl;
		cout << " " << endl;
		cout << " 50:正答率算出(実験1) for C部門2016" << endl;
		cout << " 51:正答率算出(実験2) for C部門2016" << endl;
		cout << "                                          " << endl;
		cout << " 80:BL検出確認実験" << endl;
		cout << " 90: test" << endl;
		cout << " 99:終了" << endl;
		cout << "***************************" << endl;
		cout << "Menu=";
		cin >> cmd;
		switch (cmd) {
		case 1: prep_cb_exp();
			break;
		case 2: prep_cb_exp2();
			break;
		case 3: set_ROI();
			break;
		case 4: bl_detect_th();
			break;
		case 5: cb_exp1_c2016();
			break;
		case 6: cb_exp2_c2016();
			break;
		case 7: cb_exp1_c2016_ren();
			break;
		case 8: cb_exp2_c2016_ren();
			break;
		case 9: prep_cb_exp3();
			break;
		case 10:cb_exp3_ssii();
			break;
		case 11:cb_exp3_ssii_ren();
			break;
		case 20:bl_detect2();
			break;
		case 80: bl_detect();
			break;
		case 90:test();
			break;
		case 50: analysis1();
			break;
		case 51: analysis2();
			break;
		}
		if (cmd == 99)
			break;
	}
}




