// 2016.1.10 �`
/************************/
/* BL���o�{CB�m�F�����P */
/*   for SSII2016       */
/************************/
//  BL���o�A���S���Y���g�ݍ��񂾁i2016.1.10)�@
// �}�E�XCallBack�֐��̑g�ݍ���.���^���ꉞ����Ă݂�(2016.1.10) 
// �}�E�X�ɂ��̈�ݒ芮��(2016.1.11)
// �h�����������̉���(�ݒ莎�s�񐔕���������^�C�v�ɂ����j�i2016.1.16)
// ����������ł���悤�Ɏ�������K�v����(�Ή�����j�i2016.1.16)
// release���[�h�ŃR���p�C���ł���悤�Ƀp�X���w�肷��(�Ή��ς�)
// SSII2016�p�̎����ɑg�ݏグ��(2016.2.2����j
// �h�����o����1(CB�Ȃ��j����(2016.2.7)
// ���K���[�h�̑g�ݍ���(2016.2.9)����
// �ǉ������̑g�ݍ���(���ӗU�������j(2016.2.23����j
// release���[�h���Ǝ������[�h��key���͂��������󂯕t�����Ȃ�.(�v���ǁj
//
//
//

// 2016.3.29 �`
/**********************/
/* 2016�NC������p�@*/
/**********************/
// �ڗ̈�B�e���j�b�g�p�v���O�����̊J��
//
// ����1:���Ӑ��u�ڏ���
// ����2:�s���Ӑ��u�ڏ���
// ����3:���Ӑ��u�ڏ����{���ӗU��
// ����4:�s���Ӑ��u�ڏ����{���ӗU��
//

#include "include.h"
#include "fromTeacherFanc.h"


int main(int argc, char **argv)
{
	int cmd;

	// �J�����̃`�F�b�N
	if (!cap.isOpened()) {
		cerr << "�J������������܂���" << endl;
		return 0;
	}

	// �t���[�����[�g�̐ݒ�(30fps)
	//cap.set(CV_CAP_PROP_FPS, 30.0); // �����ݒ肷��Ɠ��삵�Ȃ��i�Ȃ��H�j

	box = cv::Rect(-1, -1, 0, 0);      // BL�����̈�̏����l

	while (1) {
		cout << "********** MENU ***********" << endl;
		cout << "                           " << endl;
		cout << " 1:�h���n�񐶐�(����1�p�jfor C����2016" << endl;
		cout << " 2:�h���n�񐶐�(����2�p�jfor C����2016" << endl;
		cout << "                                " << endl;
		cout << " 3:�����̈�̐ݒ�" << endl;
		cout << " 4:BL���o臒l�̎����ݒ� for C����2016" << endl;
		cout << "                           " << endl;
		cout << " 5:�h���ω����o����1(���Ӑ�BL) for C����2016" << endl;
		cout << " 6:�h���ω����o����2(������BL) for C����2016" << endl;
		cout << "�@�@                                            " << endl;
		cout << " 7:�h���ω����o����1(���Ӑ�BL�j-���K- for C����2016" << endl;
		cout << " 8:�h���ω����o����2(������BL�j-���K- for C����2016" << endl;
		cout << "�@�@                                      " << endl;
		cout << " 9:�h���n�񐶐�(����3�p)" << endl;
		cout << " 10:�h���ω����o����3(BL���蒍�ӗU���jfor SSII2016" << endl;
		cout << " 11:�h���ω����o����3(���ӗU���j-���K-" << endl;
		cout << "                                " << endl;
		cout << " 20:BL���o�m�F����2(for �ڗ̈�B�e���j�b�g�j" << endl;
		cout << " " << endl;
		cout << " 50:�������Z�o(����1) for C����2016" << endl;
		cout << " 51:�������Z�o(����2) for C����2016" << endl;
		cout << "                                          " << endl;
		cout << " 80:BL���o�m�F����" << endl;
		cout << " 90: test" << endl;
		cout << " 99:�I��" << endl;
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




