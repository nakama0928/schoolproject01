#define NOMINMAX //<windows.h>�Ƃ̋�����h������
#define _USE_MATH_DEFINES //M_PI


//�h���񎦉�
#define TIMES 40


#include "stdafx.h"
#include "fanc.h"


//�O���[�o���֐�
using namespace std;



//���C���֐�
int main(int argc, char **argv) 
{
	int cmd;
	int preSti[TIMES];
	int timing[TIMES-1];
	int Count = 0;				 //�����񐔃J�E���g�ϐ�


	while (1) 
	{
		cout << "********** MENU ***********" << endl;
		cout << " 1:test" << endl;
		cout << " 99:�I��" << endl;
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