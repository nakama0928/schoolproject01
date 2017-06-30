#define NOMINMAX //<windows.h>との競合を防ぐため
#define _USE_MATH_DEFINES //M_PI


//刺激提示回数
#define TIMES 40


#include "stdafx.h"
#include "fanc.h"


//グローバル関数
using namespace std;



//メイン関数
int main(int argc, char **argv) 
{
	int cmd;
	int preSti[TIMES];
	int timing[TIMES-1];
	int Count = 0;				 //実験回数カウント変数


	while (1) 
	{
		cout << "********** MENU ***********" << endl;
		cout << " 1:test" << endl;
		cout << " 99:終了" << endl;
		cout << "***************************" << endl;
		cout << "Menu=";
		cin >> cmd;
		switch ( cmd ) {
		case 1:
			show(preSti, timing, Count);
			break;
		}
		if (cmd == 99)
			break;
	}
}