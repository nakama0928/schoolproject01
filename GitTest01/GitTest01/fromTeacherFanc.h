#pragma once
#define PAI 3.14159265359

//BL���o�p�̃p�����[�^
#define FR_DUR	 5	  // ����5�t���[���Ԃ̕��ϒl�Ɣ�r����
#define CANNY_TH1 120 // canny�I�y���[�^�������l�P
#define CANNY_TH2  60 // canny�I�y���[�^�������l�Q  

//Display����( RDT196LM ,�O�H)
#define DH_SIZE 1280
#define DV_SIZE 1024


// VGA��ʃT�C�Y
#define H_SIZE 640
#define V_SIZE 480

//CB�m�F�����ɂ�����h���񎦉�ʃT�C�Y(�g�p�����j
//#define SW_HSIZE 300
//#define SW_VSIZE 300


#define PITCH 0.294    // 0.294mm/��f�iRDT196M)
#define INTDEG 5       // �h���z�u 5deg
#define OBDIST 500.0   // ������


#define TR_NUM 48          // CB�������s��(24�̔{���Ƃ���j�����P�F���Ӑ�
#define TR_NUM4 (48*4)     // �����Q�F�������ɂ�����ő厎�s��
#define MAX_FRAME_NUM 2000 // �������l�����Z�o���[�h�ɂ�����ő�t���[�����i30fps*60s=1800frame�z��j
#define TH_TR_NUM 5       // �������l�����߂邽�߂̎��s��

using namespace std;

// �֐��v���g�^�C�v
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

/*---�O���[�o���ϐ� ---*/
int edge_sum[MAX_FRAME_NUM];
int av_edge_sum[MAX_FRAME_NUM];
int dff_edge_sum[MAX_FRAME_NUM];
int itmp[MAX_FRAME_NUM];  // ��Ɨp
int itmp2[MAX_FRAME_NUM]; // ��Ɨp
int eltm[MAX_FRAME_NUM];  // 
int sum_diff[MAX_FRAME_NUM]; //

int bl_th_val; // BL���o�p臒l�i�G�b�W�����l�j

double startime, stoptime, curtime;

// �^�C�}�[�p(QueryPerformanceConter)
LARGE_INTEGER nFreq, nBefore, nAfter;

//�g�p����J�����̐ݒ�
int camera_id = 0; //0�Ԃ̃J�������g�p

				   // DirectShow�ɂ��L���v�`���ݒ�
cv::VideoCapture cap(CV_CAP_DSHOW + camera_id);

//�}�E�X�p�O���[�o���ϐ�
cv::Rect box; // �����̈�i�[�p
cv::Mat gframe;
bool drawing_box = false;


//�}�E�X�p��`�`��֐�
void draw_box(cv::Mat* img, cv::Rect rect) {
	cv::rectangle(*img, cv::Point2d(box.x, box.y), cv::Point2d(box.x + box.width, box.y + box.height),
		cv::Scalar(0xff, 0x00, 0x00));
}

// �}�E�X���擾�p�R�[���o�b�N�֐�
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

//��X�m�F�p
/*--------*/
/* test   */
/*--------*/
//
// canny �I�y���[�^�̎��s���ԎZ�o

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
		cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);	// �J���[�摜�����m�N���摜�ɕϊ�
		cv::Canny(gray, gray, canny_th1, canny_th2, 3); // �L���j�[�I�y���[�^�ɂ��G�b�W���o
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
// �����x�^�C�}�̓���m�F 2016.5.4

LARGE_INTEGER nFreq, nBefore, nAfter;
DWORD dwTime;

//�ϐ��̏�����
memset(&nFreq, 0x00, sizeof nFreq);
memset(&nBefore, 0x00, sizeof nBefore);
memset(&nAfter, 0x00, sizeof nAfter);

dwTime = 0;

QueryPerformanceFrequency(&nFreq);
QueryPerformanceCounter(&nBefore);
Sleep(100); // 100ms�x��
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
cv::namedWindow("test", CV_WINDOW_AUTOSIZE); // �E�B���h�E�̐���
arrow = cv::imread("right_dir.bmp", cv::IMREAD_GRAYSCALE);
cv::imshow("test", arrow);
cout << "���F"<<arrow.cols << " ����:"<<arrow.rows << endl;
cv::waitKey(10 * 1000); // 5s�҂�
return 1;
}
*/

/*-----------------------*/
/*  BL���o�����̈�̐ݒ� */
/*-----------------------*/
//�@�̈悪�ݒ肳���ϐ�
// is:box.x, ie:box.x+box.width
// js:box.y, je:box.y+boy.width
//
int set_ROI()
{

	std::string name = "Box Example";  // �E�B���h�E�̖���
	box = cv::Rect(-1, -1, 0, 0);      // �����̈�̏����l

	cout << endl;
	cout << "�����̈�̐ݒ���s���܂�" << endl;
	cout << "�I���Ƃ���ESC�L�[�������Ă�������" << endl;

	// �J�����̃`�F�b�N
	if (!cap.isOpened()) {
		cerr << "�J������������܂���" << endl;
		return 0;
	}

	cv::namedWindow(name, CV_WINDOW_AUTOSIZE); // �E�B���h�E�̐���
	cv::moveWindow(name, (1280 - 640) / 2, (1024 - 480) / 2);// �E�B���h�E�𒆉��Ɉړ�

															 // �R�[���o�b�N��ݒ�
	cv::setMouseCallback(name, my_mouse_callback, (void *)&gframe);

	while (1) {
		cap >> gframe;

		// �}�E�X�̍��N���b�N�𗣂��܂ł̊ԁA��`���ꎞ�I�ɕ`��
		if (drawing_box) {
			draw_box(&gframe, box);
		}

		cv::imshow(name, gframe);

		// Esc�ŏI��
		if (cv::waitKey(5) == 27)
			break;
	}
	//�̈�̕`��
	draw_box(&gframe, box);
	cv::imshow(name, gframe);
	cv::waitKey(5 * 1000); // 5s�҂�

	cout << endl;
	cout << "is=" << box.x << "ie=" << box.x + box.width << endl;
	cout << "js=" << box.y << "je=" << box.y + box.height << endl;


	cv::destroyWindow(name); // �E�B���h�E�̔j��

	return 1;
}

// 2016.5.2�`
/*---------------------------*/
/* BL���o�p�������l�����ݒ�  */
/*---------------------------*/
// 
// �G�b�W���o�Fcanny operator,�������l�Œ�
//
// ���o�͈͂�ݒ肵�Ă�����s����
// 5��u�����s���Ă��炤
// �����Q���Җ��̊�b�f�[�^���L�^���Ă���
// �T�񕪂�臒l�͈ꎞ�t�@�C���Ƃ��ăf�t�H���g�t�H���_�Ɋi�[����.�������ɓǂݏo��
// �t�@�C�����Fthreshold.csv�@
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
	std::string subnm;     // �����Q���Җ�
	std::string msg1, msg2; // �\�����b�Z�[�W
	std::string moji;


	cout << "�����Q����=";
	cin >> subnm;

	cout << "�����ԍ�(1-4)=";
	cin >> exp_num;

	// �����͈͂̐ݒ�
	is = box.x;
	js = box.y;
	ie = box.x + box.width;
	je = box.y + box.height;

	if (is == -1) {
		cout << "���o�͈͂��ݒ肳��Ă��܂���";
		return 0;
	}

	cout << "�������l�������I�ɐݒ肵�܂�" << endl;

	// ���o�͈͂̕\��
	cout << "is=" << is << "ie=" << ie << endl;
	cout << "js=" << js << "je=" << je << endl;

	// �摜�\���p�E�B���h�E�̐���(���摜�j
	char *window_input = "input";
	cv::namedWindow(window_input, CV_WINDOW_AUTOSIZE);

	// �摜�\���p�E�B���h�E�̐���(�G�b�W�摜�j
	char *window_output = "output";
	cv::namedWindow(window_output, CV_WINDOW_AUTOSIZE);

	canny_th1 = CANNY_TH1; // �M�����̍����G�b�W��臒l�i�Œ�l�Ƃ���j
	canny_th2 = CANNY_TH2; // �M�����ɒႢ�G�b�W��臒l�i�Œ�l�Ƃ���j

						   // �������l�i�[�p�ꎞ�t�@�C���i�������ɓǂݏo���j�f�t�H���g�̃t�H���_�ɐ���	
	ofstream fout("threshold.csv"); // �o�̓t�@�C�����쐬����
	if (!fout) {
		cout << "threshold.csv�t�@�C�����J���܂���" << endl;
		return 0;
	}

	fout << "�����͈�" << endl;
	fout << is << endl;
	fout << js << endl;
	fout << ie << endl;
	fout << je << endl;

	fout << "�������l" << endl;
	for (tr = 0; tr <TH_TR_NUM; tr++) {   // ����5�񕪂̃f�[�^���擾����
										  // �R���\�[���ւ̕\��
		cout << tr + 1 << "��ڂ̌v�����s���܂�" << endl;
		cout << "�����L�[��������,1�b��Ɍy���u�����Ă�������" << endl << endl;
		// �E�C���h�E�ւ̕\��
		msg1 = "Starting " + to_string(tr + 1) + "th trial";
		msg2 = "Hit any key ! and blinking after 1 or 2sec.";
		cv::Mat background(cv::Size(640, 480), CV_8UC3, cv::Scalar(255, 255, 255)); // Window�ɕ\������w�i�̏���
		cv::putText(background, msg1, cv::Point(10, 100), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 0, 0), 1, CV_AA);
		cv::putText(background, msg2, cv::Point(10, 150), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 0, 0), 1, CV_AA);
		cv::imshow(window_input, background); // ���摜�̕\��(WaitKey()���󂯕t���邽��)
		cv::imshow(window_output, background);

		result = cv::waitKey(10 * 1000); // �ő�10s�҂�
		if (result == -1) {// 10�b�ԃL�[���͂���Ȃ��������蒼��
			tr--;
			continue;
		}

		// �L�[���͂���:�L�^�X�^�[�g(5�b���j
		fr = 0;
		sum_diff[0] = 0; // �X�^�[�g�t���[����0
		start = cvGetTickCount(); // �J�n����
		while (1) {
			eltm[fr] = (cvGetTickCount() - start) / (1000.0*cvGetTickFrequency());
			if (eltm[fr] >= 5 * 1000) { // �o�ߎ���5s�ȏ�Ȃ�f���擾�I��
				break;
			}
			cap >> frame;
			cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);	// �J���[�摜�����m�N���摜�ɕϊ�
			cv::Canny(gray, gray, canny_th1, canny_th2, 3); // �L���j�[�I�y���[�^�ɂ��G�b�W���o
			cv::rectangle(frame, cv::Point(is, js), cv::Point(ie, je), cv::Scalar(0, 0, 255), 1, 1);    // �֐S�̈��`�悷��
			cv::rectangle(gray, cv::Point(is, js), cv::Point(ie, je), cv::Scalar(255, 255, 255), 1, 1); // �֐S�̈��`�悷��
			cv::imshow(window_input, frame); // ���摜�̕\��
			cv::imshow(window_output, gray); // �G�b�W���o�摜
			cv::waitKey(10);                 // �摜�\�� 
			edge_sum[fr] = count_edge_sum(gray, is, js, ie, je);           // �G�b�W����
																		   //if (fr != 0) sum_diff[fr] = edge_sum[fr] - edge_sum[fr - 1]; // �אڊԃt���[������(�L�^����2016.5.3)
			fr++;
		}//while(1)
		frnm = fr;                             // ���t���[����(5s�ԁj
		bl_th_val = determine_threshold(frnm); // �������l�Z�o(bl_th_val:�O���ϐ��j
		if (bl_th_val == 0) {                   // �������l�擾���s�A��蒼��
			tr--;

			continue;
		}
		blth[tr] = bl_th_val;//�R���\�[���m�F�p�ɕێ�
		fout << bl_th_val << endl;                           // �������l�̊i�[
		save_basic_data2(tr, frnm, subnm, exp_num, is, js, ie, je); // ��b�f�[�^�̏�������
	}//for(tr=
	fout.close();
	cv::destroyAllWindows(); // ���ׂĂ�window��j��
	cout << endl << "----�������l�̊m�F---" << endl;
	for (i = 0; i < TH_TR_NUM; i++) {
		cout << blth[i] << endl;
	}
	cout << "�m�F���܂�����(y)" << endl;
	cin >> moji;
	return 1;
}


// 2016.5.3
/*--------------*/
/* 臒l�����Z�o */
/*--------------*/
//
// �O���ϐ��g�p 
//�@edge_sum[],av_edge_sum[],dff_edge_sum[]
//�@�߂�l�F�������l�C0�̂Ƃ��͂��܂����o�ł���
// 
//
int determine_threshold(
	int frnum      // ���t���[����
)
{
	int fr, i, th;
	int fr_min, fr_max, dff_min, dff_max, dff_av;

	// �G�b�W���ʂ̕��ω������E��������
	for (fr = FR_DUR; fr < frnum; fr++) {
		th = 0;
		for (i = fr - 1; i > fr - 1 - FR_DUR; i--) {// �ߋ�5�t���[�����i���t���[�����܂܂Ȃ��j�̕��ϒl
			th += edge_sum[i]; // �G�b�W�_���̐ώZ
		}
		th /= FR_DUR;
		av_edge_sum[fr] = th;                 // �ړ����ϒl
		dff_edge_sum[fr] = edge_sum[fr] - th; // �ړ����ϒl�Ƃ̍���
	}

	// �����l�̍ő�E�ŏ�����
	dff_av = 0; dff_min = 10000; dff_max = 0;
	for (fr = FR_DUR; fr < frnum; fr++) {
		if (dff_edge_sum[fr] < dff_min) { // �����l�̍ŏ��l����
			dff_min = dff_edge_sum[fr];
			fr_min = fr;
		}

		if (dff_edge_sum[fr] > dff_max) { // �����l�̍ő�l����
			dff_max = dff_edge_sum[fr];
			fr_max = fr;
		}
		dff_av += abs(dff_edge_sum[fr]);
	}
	dff_av /= (frnum - FR_DUR); // �����l�̑傫���̕��ϒl(�Z�o������臒l�ݒ�ł͎g�p���Ă��Ȃ�2016.5.3�j

								// 臒l�ݒ�
	if (fr_max < fr_min) {
		cout << "�������l�Z�o�G���[:�u�ڂłȂ������̔�������" << endl << endl;
		return 0;
	}

	if (fr_max - fr_min>10) {
		cout << "�������l�Z�o�G���[�F�����l�̍ŏ��E�ő傪�΂��Ȃ��Ă��Ȃ�" << endl << endl;
		return 0;
	}
	return(0.8*dff_min); // �b��I�ɍŏ��l�̂W�|����臒l�Ƃ���
}

// 2016.5.3
/*--------------------------*/
/* �G�b�W���x���ʂ̃J�E���g */
/*--------------------------*/
//
//
int count_edge_sum(
	cv::Mat gray, // �G�b�W�摜
	int is,       // �Z�o�͈�
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
/* ��{�f�[�^�̊i�[����2 */
/*-----------------------*/
//
//
int save_basic_data2(
	int tr,            // ���s�ԍ�
	int blframe,       // BL���N�t���[��
	std::string subnm, // �����Q����
	int expnum,        // �����ԍ�
	int is,            // �����͈�
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

	ofstream fout(fname); // �o�̓t�@�C�����쐬����
	if (!fout) {
		cout << fname << "�t�@�C�����J���܂���" << endl;
		return 0;
	}
	fout << "�����͈�" << endl;
	fout << is << endl;
	fout << js << endl;
	fout << ie << endl;
	fout << je << endl;
	fout << "�������l" << endl;
	fout << bl_th_val << endl;
	//fout << "BL���N�t���[��=" << blframe << endl;
	fout << "fr,����,�t���[���Ԋu,�G�b�W����,���σG�b�W����,�����l,���o����" << endl;

	fout << 0 << "," << eltm[0] << "," << 0 << "," << edge_sum[0] << "," << 0 << "," << 0 << endl;

	for (fr = 1; fr < blframe; fr++) {
		result = 0;
		if (dff_edge_sum[fr] < bl_th_val)result = 1;
		fout << fr << "," << eltm[fr] << "," << eltm[fr] - eltm[fr - 1] << "," << edge_sum[fr] << "," << av_edge_sum[fr] << "," << dff_edge_sum[fr] << "," << result << endl;
	}
	fout.close();
	return 1;
}











// 2016.3.29�`
/*-----------------*/
/* BL���o�m�F����2  */
/*-----------------*/
// �B�e���j�b�g�p�̊J��
// ���o�͈͂��܂��ݒ肷��
// �^�C���A�E�g�F�P��(30fps�Ȃ�j
// 
//

int bl_detect2()
{
	cv::Mat frame, gray;

	int canny_th1, canny_th2, bl_th;
	int fr;
	int i, j, is, js, ie, je;
	int cnt, th;

	// double startime, stoptime; �O���[�o���ϐ��Ő錾���Ă���B

	is = box.x;
	js = box.y;
	ie = box.x + box.width;
	je = box.y + box.height;

	if (is == -1) {
		cout << "���o�͈͂��ݒ肳��Ă��܂���";
		return 0;
	}

	//���o�͈͂̐ݒ�
	cout << "is=" << is << "ie=" << ie << endl;
	cout << "js=" << js << "je=" << je << endl;

	//�摜�\���p�E�B���h�E�̐ݒ�
	char *window_input = "input";
	cv::namedWindow(window_input, CV_WINDOW_AUTOSIZE);

	char *window_output = "output";
	cv::namedWindow(window_output, CV_WINDOW_AUTOSIZE);

	cout << "BL���o�̊m�F�����ɓ���܂�" << endl;
	cout << "canny臒l1(�M�����̍����G�b�W�j=";
	cin >> canny_th1;

	cout << "canny臒l2(�M�����̒Ⴂ�G�b�W�j=";
	cin >> canny_th2;

	cout << "BL���o�̂��߂�臒l=";
	cin >> bl_th;


	fr = 0; // �t���[���ԍ��̏�����
	startime = (double)cvGetTickCount(); // �J�n����
	while (1) {
		fr++;
		if (fr >= 30 * 60) { // 1���ȏ�(30fps)�Ȃ�^�C���A�E�g
			save_basic_data(edge_sum, fr); // ��{�f�[�^�̏�������
			break;
		}

		cap >> frame; // 1�t���[���ǂݍ���

		cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);	// �J���[�摜�����m�N���摜�ɕϊ�
		cv::Canny(gray, gray, canny_th1, canny_th2, 3); // �L���j�[�I�y���[�^�ɂ��G�b�W���o

		cv::rectangle(frame, cv::Point(is, js), cv::Point(ie, je), cv::Scalar(0, 0, 255), 1, 1);          // �֐S�̈��`�悷��
		cv::rectangle(gray, cv::Point(is, js), cv::Point(ie, je), cv::Scalar(255, 255, 255), 1, 1); // �֐S�̈��`�悷��

		cv::imshow(window_input, frame); // ���摜�̕\��
		cv::imshow(window_output, gray); // �G�b�W���o�摜

										 // �G�b�W�_���̃J�E���g
		cnt = 0;
		for (j = js; j < je; j++) {
			for (i = is; i < ie; i++) {
				if (gray.data[j * H_SIZE + i] == 255)
					cnt++;
			}
		}
		edge_sum[fr] = cnt;

		// BL���菈��
		if (fr < 30 * 10) { // ���s�ŏ���10�b�Ԃ͏������Ȃ�
			cv::waitKey(15);
			continue;
		}

		th = 0;
		for (i = fr - 1; i > fr - 1 - FR_DUR; i--) {// �ߋ�5�t���[�����̕��ϒl
			th += edge_sum[i]; // �G�b�W�_���̐ώZ
		}
		th /= FR_DUR;
		itmp[fr] = th; // �m�F�p
		itmp2[fr] = edge_sum[fr] - th; // �m�F�p
		if (edge_sum[fr] - th < -bl_th) {		// ����ق̉��~���_�̌��o����
			save_basic_data(edge_sum, fr);      // ��{�f�[�^�̊i�[����
			cv::imwrite("closebl.bmp", frame);  // BL���̉摜���L�^
			break; // �񎦏I��
		}
		stoptime = (double)cvGetTickCount() - startime; // �I������


		if (cv::waitKey(15) >= 0) { // 15ms���ŁA�����L�[�������ꂽ��I��
			break;
		}
	}//while(1)
	cv::destroyAllWindows(); // ���ׂĂ�window��j��
	return 1;
}











/*-----------------*/
/* BL���o�m�F����  */
/*-----------------*/
// ���o�͈͂��܂��ݒ肷��
// �^�C���A�E�g�F�P��(30fps�Ȃ�j
//
int bl_detect()
{
	cv::Mat frame, gray;

	int canny_th1, canny_th2, bl_th;
	int fr;
	int i, j, is, js, ie, je;
	int cnt, th;

	// double startime, stoptime; �O���[�o���ϐ��Ő錾���Ă���B

	is = box.x;
	js = box.y;
	ie = box.x + box.width;
	je = box.y + box.height;

	if (is == -1) {
		cout << "���o�͈͂��ݒ肳��Ă��܂���";
		return 0;
	}

	//���o�͈͂̐ݒ�
	cout << "is=" << is << "ie=" << ie << endl;
	cout << "js=" << js << "je=" << je << endl;

	//�摜�\���p�E�B���h�E�̐ݒ�
	char *window_input = "input";
	cv::namedWindow(window_input, CV_WINDOW_AUTOSIZE);

	char *window_output = "output";
	cv::namedWindow(window_output, CV_WINDOW_AUTOSIZE);

	cout << "BL���o�̊m�F�����ɓ���܂�" << endl;
	cout << "canny臒l1(�M�����̍����G�b�W�j=";
	cin >> canny_th1;

	cout << "canny臒l2(�M�����̒Ⴂ�G�b�W�j=";
	cin >> canny_th2;

	cout << "BL���o�̂��߂�臒l=";
	cin >> bl_th;


	fr = 0; // �t���[���ԍ��̏�����
	startime = (double)cvGetTickCount(); // �J�n����
	while (1) {
		fr++;
		if (fr >= 30 * 60) { // 1���ȏ�(30fps)�Ȃ�^�C���A�E�g
			save_basic_data(edge_sum, fr); // ��{�f�[�^�̏�������
			break;
		}

		cap >> frame; // 1�t���[���ǂݍ���

		cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);	// �J���[�摜�����m�N���摜�ɕϊ�
		cv::Canny(gray, gray, canny_th1, canny_th2, 3); // �L���j�[�I�y���[�^�ɂ��G�b�W���o

		cv::rectangle(frame, cv::Point(is, js), cv::Point(ie, je), cv::Scalar(0, 0, 255), 1, 1);          // �֐S�̈��`�悷��
		cv::rectangle(gray, cv::Point(is, js), cv::Point(ie, je), cv::Scalar(255, 255, 255), 1, 1); // �֐S�̈��`�悷��

		cv::imshow(window_input, frame); // ���摜�̕\��
		cv::imshow(window_output, gray); // �G�b�W���o�摜

										 // �G�b�W�_���̃J�E���g
		cnt = 0;
		for (j = js; j < je; j++) {
			for (i = is; i < ie; i++) {
				if (gray.data[j * H_SIZE + i] == 255)
					cnt++;
			}
		}
		edge_sum[fr] = cnt;

		// BL���菈��
		if (fr < 30 * 10) { // ���s�ŏ���10�b�Ԃ͏������Ȃ�
			cv::waitKey(15);
			continue;
		}

		th = 0;
		for (i = fr - 1; i > fr - 1 - FR_DUR; i--) {// �ߋ�5�t���[�����̕��ϒl
			th += edge_sum[i]; // �G�b�W�_���̐ώZ
		}
		th /= FR_DUR;
		itmp[fr] = th; // �m�F�p
		itmp2[fr] = edge_sum[fr] - th; // �m�F�p
		if (edge_sum[fr] - th < -bl_th) {		// ����ق̉��~���_�̌��o����
			save_basic_data(edge_sum, fr);      // ��{�f�[�^�̊i�[����
			cap >> frame;
			cv::imwrite("closebl.bmp", frame);  // BL���̉摜���L�^
			break; // �񎦏I��
		}
		stoptime = (double)cvGetTickCount() - startime; // �I������


		if (cv::waitKey(15) >= 0) { // 15ms���ŁA�����L�[�������ꂽ��I��
			break;
		}
	}//while(1)
	cv::destroyAllWindows(); // ���ׂĂ�window��j��
	return 1;
}

/*----------------------*/
/* ��{�f�[�^�̊i�[���� */
/*----------------------*/
//
// fr=1800�̂Ƃ���BL���o�ł���
//
int save_basic_data(
	int esum[],  // �G�b�W�_��
	int blframe  // BL���N�t���[��
)
{
	int fr;
	ofstream fout("edgecount.csv"); // �o�̓t�@�C�����쐬����
	if (!fout) {
		cout << "�t�@�C�����J���܂���" << endl;
		return 0;
	}
	fout << "BL���N�t���[��=" << blframe << endl;
	fout << "fr,�G�b�W��,臒l,�����l" << endl;

	for (fr = 1; fr <= blframe; fr++) {
		fout << fr << "," << esum[fr] << "," << itmp[fr] << "," << itmp2[fr] << endl;
	}

	//�������i��ŏ����Ă����j
	fout << stoptime / ((double)cvGetTickFrequency()*1000.) << endl;

	fout.close();
	return 1;
}


/****************************************************/
/*                C����2016�����p�@                 */
/****************************************************/
//
//



//2016.4.24�` for C����2016
/*-------------------------------*/
/* �h���ω����o����2             */
/* �������u��                    */
/*-------------------------------*/
//
//  ���K��
//
//  C����2016�p
//  ������BL�̃^�C�~���O�Ő؂�ւ�
//�@�؂�ւ����_�̖ڗ̈�f�����L�^
//
//  �����Q����1��������A48�~4���s���p�ӂ��Ă���B
//�@�@ex.�����Q����:test �� test1,test2,test3,test4�̃t�H���_(test���̃T�u�t�H���_�j��48���s���W�J���Ă���
//
//�@TR_NUM�񕪁A������BL�̃f�[�^���擾�ł������_�ŁA�������I������B
//�@�ő�������Ԃ�40���ԂƂ���B���Ԃ��߂����狭���I������B
//
//

int cb_exp2_c2016_ren()
{
	static int st[TR_NUM4][8]; // �e���s�ɂ�����8�ӏ��̎��ӈʒu�ɒ񎦂���h���ԍ�(0,1,2,3)�@
	static int chpos[TR_NUM4]; // �e���s�ɂ����Ďh�����ω�������ӈʒu(0�F�����,1,�E�E�E,7�F�E����)
	static int prst[TR_NUM4];  // �ω��O�̎h���ԍ�
	static int chst[TR_NUM4];  // �ω����ɒ񎦂���h���ԍ�
	static int intvl[TR_NUM4]; // �ő�h���ω����ԊԊu

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

	int64 exp_start;  // �����J�n����
	int64 exp_eltim;  // �����o�ߎ���
	int   tr_eltim = 0; // �P���s�J�n��̌o�ߎ���

	std::string subnm; // �����Q���Җ�
	std::string sbuf, sbuf1, sbuf2, sbuf3, sbuf4;  // �o�b�t�@
	std::string fns, bfname;
	cv::Mat frame, gray;


	cout << "---�h���ω����o����2(���K�j---" << endl;

	cout << "�����Q����=(ren�Ɠ��́j"; // 
	cin >> subnm;

	// BL���o�͈͐ݒ�E�������l�t�@�C���̓ǂݏo��
	bfname = "threshold.csv";
	ifstream finb(bfname);
	if (!finb) {
		cout << "���o�͈͗p�t�@�C��������܂���" << bfname << endl;
		getchar();
		getchar();
		return 0;
	}

	// �ݒ�͈͓ǂݏo��
	finb >> sbuf; // 1�s���ǂݔ�΂�
	finb >> is;
	finb >> js;
	finb >> ie;
	finb >> je;
	cout << "�ݒ�͈�" << endl;
	cout << is << "," << js << "," << ie << "," << je << endl;

	//�������l�̓ǂݏo��
	cout << "�������l" << endl;
	finb >> sbuf; // 1�s���ǂݔ�΂�
	for (i = 0; i < TH_TR_NUM; i++) {
		finb >> th[i];
		cout << th[i] << endl;
	}
	finb.close();

	// �������l�ݒ�(���ϒl�Ƃ���j2016.5.4
	bl_th_val = 0;
	for (i = 0; i < TH_TR_NUM; i++) {
		bl_th_val += th[i];
	}
	bl_th_val /= TH_TR_NUM;
	cout << "BL���o�������l=" << bl_th_val << endl;

	cout << "�m�F�ł����牽�����͂��Ă�������" << endl;
	getchar();
	getchar();


	// �h���񎦏����f�[�^�ǂݍ���
	std::string sfname;

	for (fn = 0; fn <= 3; fn++) { // �t�H���_�ԍ�
		fns = std::to_string(fn + 1);
		sfname = "c:\\IEEJ_C_2016\\";
		sfname = sfname + "exp2" + "\\" + subnm + "\\" + subnm + fns + "\\order_";
		sfname = sfname + "����2" + "_" + "���K_" + subnm + fns + ".csv";     // �t�@�C�����̐���

																		  // �h���񎦏����f�[�^�t�@�C���̓ǂݍ���
		ifstream fin(sfname);
		if (!fin) {
			cout << "�h���񎦗p�t�@�C��������܂���" << sfname << endl;
			getchar();
			getchar();
			return 0;
		}

		// �ω����̓ǂݎ��
		fin >> sbuf; // 1�s���ǂݔ�΂�

					 // ���s�ԍ��A�ω��ʒu�A�ω��O�h���A�ω���h��,�񎦊Ԋu
		for (tr = 0; tr < TR_NUM; tr++) {
			fin >> itmp1 >> split >> itmp2 >> split >> itmp3 >> split >> itmp4 >> split >> itmp5;
			chpos[tr + fn*TR_NUM] = itmp2;
			prst[tr + fn*TR_NUM] = itmp3;
			chst[tr + fn*TR_NUM] = itmp4;
			intvl[tr + fn*TR_NUM] = itmp5;
			//cout << tr+1 << ","<< chpos[tr] << "," << prst[tr] << "," << chst[tr]<< endl;//�m�F
		}

		// �񎦃p�^�[���i���ӂW�ӏ��j
		fin >> sbuf; // 1�s���ǂݔ�΂�

		for (tr = 0; tr < TR_NUM; tr++) {
			fin >> itmp1;// ���s�ԍ��ǂݔ�΂�
			for (i = 0; i < 8; i++) {
				fin >> split >> itmp1;
				st[tr + fn*TR_NUM][i] = itmp1;
			}
			fin >> split;
		}
		fin.close();
	}//for(fn=


	 // �񎦏��̓ǂݍ��݌��ʂ̊m�F
	cout << "���s�ԍ�,�ω��ʒu,���ݎh���ԍ�,�ω���h���ԍ�,�񎦊Ԋu" << endl;
	for (tr = 0; tr < TR_NUM4; tr++) {
		cout << tr + 1 << ":" << chpos[tr] << "," << prst[tr] << "," << chst[tr] << "," << intvl[tr] << endl;
	}
	cout << "���s�ԍ�,�񎦎h���i�e���ӈʒu)" << endl;
	for (tr = 0; tr < TR_NUM4; tr++) {
		cout << tr + 1 << ":";

		for (i = 0; i < 8; i++) {
			cout << st[tr][i] << ",";
		}
		cout << endl;
	}


	//�����܂�OK(2016.5.17)

	cout << "���K���n�߂܂����F�����L�[�������Ă�������";
	getchar();
	getchar();


	// ���s�J�n����

	// �񓚏��L�^����
	std::string fname, imname, eyename;
	std::ostringstream ostr;

	fname = "c:\\IEEJ_C_2016\\";
	fname = fname + "exp2" + "\\" + subnm + "\\info_";
	fname = fname + "����2" + "_" + "���K_" + subnm + ".csv"; // �t�@�C�����̐���

														  // �񓚏��o�͗p�t�@�C���̐���
	ofstream fout(fname); // �o�̓t�@�C�����쐬����
	if (!fout) {
		cout << "�t�@�C�����J���܂���" << endl;
		getchar();
		getchar();
		return 0;
	}
	fout << "���s�ԍ�,�ω�����,BL���N,�ω��ʒu" << endl;

	char *window_info = "info";
	//cv::namedWindow(window_info, CV_WINDOW_AUTOSIZE);							      �@// �h���񎦗pwindow�̐���
	//cv::moveWindow(window_info, (DH_SIZE - SW_HSIZE) / 2, (DV_SIZE - SW_VSIZE) / 2);  // ��ʒ����ֈړ�
	cv::Mat background(cv::Size(DH_SIZE, DV_SIZE), CV_8UC3, cv::Scalar(255, 255, 255)); // Window�ɕ\������w�i�̏���

																						//�񎦎h���̏���
	char mark[][10] = { "X", "O", "H", "E" }; // �P�o�C�g������I��


											  //�����񎦈ʒu�̒��S�ʒu�i�����_�ʒu�j
	basei = DH_SIZE / 2;
	basej = DV_SIZE / 2;

	// �h���z�u�̎Z�o
	dtmp = OBDIST*tan(INTDEG*PAI / 180.0);
	dtmp /= PITCH;
	dpix = (int)dtmp;

	exp_start = cvGetTickCount(); // �����J�n�����̎擾
	blcnt = 0;                   // BL���N���s���̏�����

								 // �����J�n!
	for (tr = 0; tr < TR_NUM4; tr++) { // �ő�TR_NUM4��������{
		cv::Mat background(cv::Size(DH_SIZE, DV_SIZE), CV_8UC3, cv::Scalar(255, 255, 255)); // Window�ɕ\������w�i�̏���
		cv::imshow(window_info, background);  // �摜�\��
		cv::waitKey(3 * 1000);                // �w�i��3�b�Ԓ�

											  // �񎦎h���̏���
		int pcnt = 0;
		for (j = -1; j <= 1; j++) {
			for (i = -1; i <= 1; i++) {
				if (i == 0 && j == 0) continue; // ��ʒ����͕ʂ̋L����񎦂���̂ŃX�L�b�v
				px = basei + i*dpix; // ����̒񎦈ʒu
				py = basej + j*dpix; // ����̒񎦈ʒu
				k = st[tr][pcnt];    // �񎦈ʒupcnt�֒񎦂���h���ԍ��̐ݒ�
				cv::putText(background, (const char*)mark[k], cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0, 0, 0), 3, CV_AA);
				pcnt++; // ���̒񎦈ʒu(0,1,2,�E�E�E,7)
			}//for(i=
		}//for(j=

		 // ��ʒ����ւ̋L���i�����_�񎦁j
		px = basei; // �񎦈ʒu(���j
		py = basej; // �񎦈ʒu(�c)
		cv::putText(background, "+", cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(128, 128, 128), 3, CV_AA);

		// �h����
		//startime = (double)cvGetTickCount();  // �J�n�����̎擾
		cv::imshow(window_info, background);    // �摜�\��(���̎��_�ł͂܂��񎦊J�n����Ă��Ȃ��j
		cv::waitKey(30);                        // �h���摜�̒�

												//�h���񎦉�ʂ̕ω�����
		k0 = prst[tr]; // �ω��O�̎h���ԍ�
		k1 = chst[tr]; // �ω���̎h���ԍ�
		get_ij(chpos[tr], &i, &j); // �ω��ʒu��i,j�ԍ��̎擾
		px = basei + i*dpix;       // �ω��ʒu�̐ݒ�
		py = basej + j*dpix;       // �ω��ʒu�̐ݒ�

		int result = -1;
		result = check_volbl_occurence(intvl[tr] * 1000, &tr_eltim, bl_th_val, is, js, ie, je); // // ������BL�̃`�F�b�N(result=1:BL���N�Aresult=0:�^�C���A�E�g)
		time_delay(15);    // 15ms�ҋ@���A�ق�������Ԃ̂Ƃ��Ɏh����؂�ւ���(���̂��炢���悳�����j
		cv::putText(background, mark[k0], cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(255, 255, 255), 5, CV_AA); // �Â��L��������
		cv::putText(background, mark[k1], cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0, 0, 0), 3, CV_AA);       // �V�����L����`��
		cv::imshow(window_info, background);	// �L���\���X�V
		cv::waitKey(5);                      // �����ɕ`��
		cap >> gframe;                          // �X�V���_�̖ڗ̈�f���̎擾
		cv::waitKey(3 * 1000);                  // 3�b�ҋ@

												// �ڗ̈�摜�L�^
		std::ostringstream ostr1;
		ostr1 << "_tr" << tr;

		//�ȉ��A����m�F���Ă��Ȃ� �� �t�H���_�ƃt�@�C����(2016.4.13)
		eyename = "c:\\IEEJ_C_2016\\";
		eyename = eyename + "exp2" + "\\" + subnm + "\\eye_����2_";
		eyename = eyename + subnm + ostr1.str() + ".bmp";

		cv::imwrite(eyename, gframe);   // BL���̉摜���L�^
		cv::destroyWindow(window_info); // window�̔j��

										//�o�ߎ��ԕ\������
		cur_tim = (cvGetTickCount() - exp_start) / (1000 * cvGetTickFrequency()); // ms�P��
		tim_min = cur_tim / (60 * 1000);                // ���P��
		tim_sec = (cur_tim - tim_min * 60 * 1000) / 1000; // �b�P��


														  // �񓚓��́i�R���\�[���Łj
		cout << endl;
		cout << "�o�ߎ��ԁF" << tim_min << "��" << tim_sec << "�b" << endl;
		cout << endl;
		cout << "���s�ԍ�:" << tr + 1 << "/" << TR_NUM4 << endl << endl;
		cout << "------------------------------------------" << endl;
		cout << "�ǂ��̈ʒu���ω������������Ă�������" << endl;
		cout << "1:����i�A2:������i�A3:�E��i" << endl;
		cout << "4:�����i�A�@�@�@�@�@�@5:�E���i" << endl;
		cout << "6:�����i�A7:�������i�A8:�E���i" << endl;
		cout << endl;
		cout << "9:�킩��Ȃ�" << endl;
		cin >> answer;

		if (result == 1) { // ������BL���N
			fout << tr << "," << tr_eltim << "," << 0 << "," << answer << endl;
			blcnt++;//BL���N�񐔂̃J�E���g�A�b�v
		}
		if (result == 0) { // ������BL���N�����A�^�C���A�E�g
			fout << tr << "," << tr_eltim << "," << -1 << "," << answer << endl;
		}

		// BL���N���s��TR_NUM��ɂȂ������_�Ŏ����I��
		if (blcnt >= TR_NUM) {
			break;
		}

		// 3���ȏ�o�߂�����A�r���ł������I��
		exp_eltim = cvGetTickCount() - exp_start; // �����o�ߎ��ԎZ�o

		if (exp_eltim / (1000 * cvGetTickFrequency()) > 3 * 60 * 1000) {
			break;
		}

		if (tr != 0 && (tr % 10) == 0) { // ���s10�񖈂ɖڗ̈�ʒu����
			adjust_eye_position(is, js, ie, je);
		}
	}//for(tr=
	fout.close();
	return 1;
}





//2016.4.24�` for C����2016
/*-------------------------------*/
/* �h���ω����o����2             */
/* �������u��                    */
/*-------------------------------*/
//
//  C����2016�p
//  ������BL�̃^�C�~���O�Ő؂�ւ�
//�@�؂�ւ����_�̖ڗ̈�f�����L�^
//
//  �����Q����1��������A48�~4���s���p�ӂ��Ă���B
//�@�@ex.�����Q����:test �� test1,test2,test3,test4�̃t�H���_(test���̃T�u�t�H���_�j��48���s���W�J���Ă���
//
//�@TR_NUM�񕪁A������BL�̃f�[�^���擾�ł������_�ŁA�������I������B
//�@�ő�������Ԃ�40���ԂƂ���B���Ԃ��߂����狭���I������B
//
//

int cb_exp2_c2016()
{
	static int st[TR_NUM4][8]; // �e���s�ɂ�����8�ӏ��̎��ӈʒu�ɒ񎦂���h���ԍ�(0,1,2,3)�@
	static int chpos[TR_NUM4]; // �e���s�ɂ����Ďh�����ω�������ӈʒu(0�F�����,1,�E�E�E,7�F�E����)
	static int prst[TR_NUM4];  // �ω��O�̎h���ԍ�
	static int chst[TR_NUM4];  // �ω����ɒ񎦂���h���ԍ�
	static int intvl[TR_NUM4]; // �ő�h���ω����ԊԊu

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

	int64 exp_start;  // �����J�n����
	int64 exp_eltim;  // �����o�ߎ���
	int   tr_eltim = 0; // �P���s�J�n��̌o�ߎ���

	std::string subnm; // �����Q���Җ�
	std::string sbuf, sbuf1, sbuf2, sbuf3, sbuf4;  // �o�b�t�@
	std::string fns, bfname;
	cv::Mat frame, gray;


	cout << "---�h���ω����o����2---" << endl;

	cout << "�����Q����=(subA,subB�ȂǂƓ��́j"; // subA1�Ȃǂ̓��͎͂󂯕t���Ȃ�
	cin >> subnm;

	// BL���o�͈͐ݒ�E�������l�t�@�C���̓ǂݏo��
	bfname = "threshold.csv";
	ifstream finb(bfname);
	if (!finb) {
		cout << "���o�͈͗p�t�@�C��������܂���" << bfname << endl;
		getchar();
		getchar();
		return 0;
	}

	// �ݒ�͈͓ǂݏo��
	finb >> sbuf; // 1�s���ǂݔ�΂�
	finb >> is;
	finb >> js;
	finb >> ie;
	finb >> je;
	cout << "�ݒ�͈�" << endl;
	cout << is << "," << js << "," << ie << "," << je << endl;

	//�������l�̓ǂݏo��
	cout << "�������l" << endl;
	finb >> sbuf; // 1�s���ǂݔ�΂�
	for (i = 0; i < TH_TR_NUM; i++) {
		finb >> th[i];
		cout << th[i] << endl;
	}
	finb.close();

	// �������l�ݒ�(���ϒl�Ƃ���j2016.5.4
	bl_th_val = 0;
	for (i = 0; i < TH_TR_NUM; i++) {
		bl_th_val += th[i];
	}
	bl_th_val /= TH_TR_NUM;
	cout << "BL���o�������l=" << bl_th_val << endl;

	cout << "�m�F�ł����牽�����͂��Ă�������" << endl;
	getchar();
	getchar();


	// �h���񎦏����f�[�^�ǂݍ���
	std::string sfname;

	for (fn = 0; fn <= 3; fn++) { // �t�H���_�ԍ�
		fns = std::to_string(fn + 1);
		sfname = "c:\\IEEJ_C_2016\\";
		sfname = sfname + "exp2" + "\\" + subnm + "\\" + subnm + fns + "\\order_";
		sfname = sfname + "����2" + "_" + subnm + fns + ".csv";     // �t�@�C�����̐���

																  // �h���񎦏����f�[�^�t�@�C���̓ǂݍ���
		ifstream fin(sfname);
		if (!fin) {
			cout << "�h���񎦗p�t�@�C��������܂���" << sfname << endl;
			getchar();
			getchar();
			return 0;
		}

		// �ω����̓ǂݎ��
		fin >> sbuf; // 1�s���ǂݔ�΂�

					 // ���s�ԍ��A�ω��ʒu�A�ω��O�h���A�ω���h��,�񎦊Ԋu
		for (tr = 0; tr < TR_NUM; tr++) {
			fin >> itmp1 >> split >> itmp2 >> split >> itmp3 >> split >> itmp4 >> split >> itmp5;
			chpos[tr + fn*TR_NUM] = itmp2;
			prst[tr + fn*TR_NUM] = itmp3;
			chst[tr + fn*TR_NUM] = itmp4;
			intvl[tr + fn*TR_NUM] = itmp5;
			//cout << tr+1 << ","<< chpos[tr] << "," << prst[tr] << "," << chst[tr]<< endl;//�m�F
		}

		// �񎦃p�^�[���i���ӂW�ӏ��j
		fin >> sbuf; // 1�s���ǂݔ�΂�

		for (tr = 0; tr < TR_NUM; tr++) {
			fin >> itmp1;// ���s�ԍ��ǂݔ�΂�
			for (i = 0; i < 8; i++) {
				fin >> split >> itmp1;
				st[tr + fn*TR_NUM][i] = itmp1;
			}
			fin >> split;
		}
		fin.close();
	}//for(fn=


	 // �񎦏��̓ǂݍ��݌��ʂ̊m�F
	cout << "���s�ԍ�,�ω��ʒu,���ݎh���ԍ�,�ω���h���ԍ�,�񎦊Ԋu" << endl;
	for (tr = 0; tr < TR_NUM4; tr++) {
		cout << tr + 1 << ":" << chpos[tr] << "," << prst[tr] << "," << chst[tr] << "," << intvl[tr] << endl;
	}
	cout << "���s�ԍ�,�񎦎h���i�e���ӈʒu)" << endl;
	for (tr = 0; tr < TR_NUM4; tr++) {
		cout << tr + 1 << ":";

		for (i = 0; i < 8; i++) {
			cout << st[tr][i] << ",";
		}
		cout << endl;
	}


	//�����܂�OK(2016.5.17)

	cout << "�������n�߂܂����F�����L�[�������Ă�������";
	getchar();
	getchar();


	// ���s�J�n����

	// �񓚏��L�^����
	std::string fname, imname, eyename;
	std::ostringstream ostr;

	fname = "c:\\IEEJ_C_2016\\";
	fname = fname + "exp2" + "\\" + subnm + "\\info_";
	fname = fname + "����2" + "_" + subnm + ".csv"; // �t�@�C�����̐���

												  // �񓚏��o�͗p�t�@�C���̐���
	ofstream fout(fname); // �o�̓t�@�C�����쐬����
	if (!fout) {
		cout << "�t�@�C�����J���܂���" << endl;
		getchar();
		getchar();
		return 0;
	}
	fout << "���s�ԍ�,�ω�����,BL���N,�ω��ʒu" << endl;

	char *window_info = "info";
	//cv::namedWindow(window_info, CV_WINDOW_AUTOSIZE);							      �@// �h���񎦗pwindow�̐���
	//cv::moveWindow(window_info, (DH_SIZE - SW_HSIZE) / 2, (DV_SIZE - SW_VSIZE) / 2);  // ��ʒ����ֈړ�
	cv::Mat background(cv::Size(DH_SIZE, DV_SIZE), CV_8UC3, cv::Scalar(255, 255, 255)); // Window�ɕ\������w�i�̏���

																						//�񎦎h���̏���
	char mark[][10] = { "X", "O", "H", "E" }; // �P�o�C�g������I��


											  //�����񎦈ʒu�̒��S�ʒu�i�����_�ʒu�j
	basei = DH_SIZE / 2;
	basej = DV_SIZE / 2;

	// �h���z�u�̎Z�o
	dtmp = OBDIST*tan(INTDEG*PAI / 180.0);
	dtmp /= PITCH;
	dpix = (int)dtmp;

	exp_start = cvGetTickCount(); // �����J�n�����̎擾
	blcnt = 0;                   // BL���N���s���̏�����

								 // �����J�n!
	for (tr = 0; tr < TR_NUM4; tr++) { // �ő�TR_NUM4��������{
		cv::Mat background(cv::Size(DH_SIZE, DV_SIZE), CV_8UC3, cv::Scalar(255, 255, 255)); // Window�ɕ\������w�i�̏���
		cv::imshow(window_info, background);  // �摜�\��
		cv::waitKey(3 * 1000);                // �w�i��3�b�Ԓ�

											  // �񎦎h���̏���
		int pcnt = 0;
		for (j = -1; j <= 1; j++) {
			for (i = -1; i <= 1; i++) {
				if (i == 0 && j == 0) continue; // ��ʒ����͕ʂ̋L����񎦂���̂ŃX�L�b�v
				px = basei + i*dpix; // ����̒񎦈ʒu
				py = basej + j*dpix; // ����̒񎦈ʒu
				k = st[tr][pcnt];    // �񎦈ʒupcnt�֒񎦂���h���ԍ��̐ݒ�
				cv::putText(background, (const char*)mark[k], cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0, 0, 0), 3, CV_AA);
				pcnt++; // ���̒񎦈ʒu(0,1,2,�E�E�E,7)
			}//for(i=
		}//for(j=

		 // ��ʒ����ւ̋L���i�����_�񎦁j
		px = basei; // �񎦈ʒu(���j
		py = basej; // �񎦈ʒu(�c)
		cv::putText(background, "+", cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(128, 128, 128), 3, CV_AA);

		// �h����
		//startime = (double)cvGetTickCount();  // �J�n�����̎擾
		cv::imshow(window_info, background);    // �摜�\��(���̎��_�ł͂܂��񎦊J�n����Ă��Ȃ��j
		cv::waitKey(30);                        // �h���摜�̒�

												//�h���񎦉�ʂ̕ω�����
		k0 = prst[tr]; // �ω��O�̎h���ԍ�
		k1 = chst[tr]; // �ω���̎h���ԍ�
		get_ij(chpos[tr], &i, &j); // �ω��ʒu��i,j�ԍ��̎擾
		px = basei + i*dpix;       // �ω��ʒu�̐ݒ�
		py = basej + j*dpix;       // �ω��ʒu�̐ݒ�

		int result = -1;
		result = check_volbl_occurence(intvl[tr] * 1000, &tr_eltim, bl_th_val, is, js, ie, je); // // ������BL�̃`�F�b�N(result=1:BL���N�Aresult=0:�^�C���A�E�g)
		time_delay(15);    // 15ms�ҋ@���A�ق�������Ԃ̂Ƃ��Ɏh����؂�ւ���(���̂��炢���悳�����j
		cv::putText(background, mark[k0], cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(255, 255, 255), 5, CV_AA); // �Â��L��������
		cv::putText(background, mark[k1], cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0, 0, 0), 3, CV_AA);       // �V�����L����`��
		cv::imshow(window_info, background);	// �L���\���X�V
		cv::waitKey(5);                      // �����ɕ`��
		cap >> gframe;                          // �X�V���_�̖ڗ̈�f���̎擾
		cv::waitKey(3 * 1000);                  // 3�b�ҋ@

												// �ڗ̈�摜�L�^
		std::ostringstream ostr1;
		ostr1 << "_tr" << tr;

		//�ȉ��A����m�F���Ă��Ȃ� �� �t�H���_�ƃt�@�C����(2016.4.13)
		eyename = "c:\\IEEJ_C_2016\\";
		eyename = eyename + "exp2" + "\\" + subnm + "\\eye_����2_";
		eyename = eyename + subnm + ostr1.str() + ".bmp";

		cv::imwrite(eyename, gframe);   // BL���̉摜���L�^
		cv::destroyWindow(window_info); // window�̔j��

										//�o�ߎ��ԕ\������
		cur_tim = (cvGetTickCount() - exp_start) / (1000 * cvGetTickFrequency()); // ms�P��
		tim_min = cur_tim / (60 * 1000);                // ���P��
		tim_sec = (cur_tim - tim_min * 60 * 1000) / 1000; // �b�P��


														  // �񓚓��́i�R���\�[���Łj
		cout << endl;
		cout << "�o�ߎ��ԁF" << tim_min << "��" << tim_sec << "�b" << endl;
		cout << endl;
		cout << "���s�ԍ�:" << tr + 1 << "/" << TR_NUM4 << endl << endl;
		cout << "------------------------------------------" << endl;
		cout << "�ǂ��̈ʒu���ω������������Ă�������" << endl;
		cout << "1:����i�A2:������i�A3:�E��i" << endl;
		cout << "4:�����i�A�@�@�@�@�@�@5:�E���i" << endl;
		cout << "6:�����i�A7:�������i�A8:�E���i" << endl;
		cout << endl;
		cout << "9:�킩��Ȃ�" << endl;
		cin >> answer;

		if (result == 1) { // ������BL���N
			fout << tr << "," << tr_eltim << "," << 0 << "," << answer << endl;
			blcnt++;//BL���N�񐔂̃J�E���g�A�b�v
		}
		if (result == 0) { // ������BL���N�����A�^�C���A�E�g
			fout << tr << "," << tr_eltim << "," << -1 << "," << answer << endl;
		}

		// BL���N���s��TR_NUM��ɂȂ������_�Ŏ����I��
		if (blcnt >= TR_NUM) {
			break;
		}

		// 40���ȏ�o�߂�����A�r���ł������I��
		exp_eltim = cvGetTickCount() - exp_start; // �����o�ߎ��ԎZ�o

		if (exp_eltim / (1000 * cvGetTickFrequency()) > 40 * 60 * 1000) {
			break;
		}

		/*
		if (exp_eltim / (1000 * cvGetTickFrequency()) > 5 * 60 * 1000){ // ����m�F�̂���5���ɐݒ肵�Ă���D
		break;
		}
		*/

		if (tr != 0 && (tr % 10) == 0) { // ���s10�񖈂ɖڗ̈�ʒu����
			adjust_eye_position(is, js, ie, je);
		}
	}//for(tr=
	fout.close();
	return 1;
}



//2016.4.13�` ���Kfor C����2016
/*-------------------------------*/
/* �h���ω����o����1             */
/* ���Ӑ��u��                    *
/* -���K-                        */
/*-------------------------------*/
//
//  ���K��
//
//  C����2016�p
//  ���Ӑ�BL�̃^�C�~���O�Ő؂�ւ�
//�@�؂�ւ����_�̖ڗ̈�f�����L�^
//
//  BL���o�͑g�ݍ���ł��Ȃ���,�����OK(2016.4.23)
//  BL���o����(2016.5.4)
//  ���s10�񖈂ɖڗ̈�ʒu�̔��������s��(2016.5.4)
//
int cb_exp1_c2016_ren()
{
	static int st[TR_NUM][8]; // �e���s�ɂ�����8�ӏ��̎��ӈʒu�ɒ񎦂���h���ԍ�(0,1,2,3)�@
	int chpos[TR_NUM];        // �e���s�ɂ����Ďh�����ω�������ӈʒu(0�F�����,1,�E�E�E,7�F�E����)
	int prst[TR_NUM];         // �ω��O�̎h���ԍ�
	int chst[TR_NUM];         // �ω����ɒ񎦂���h���ԍ�

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

	std::string subnm; // �����Q���Җ�
	std::string sbuf, sbuf1, sbuf2, sbuf3, sbuf4;  // �o�b�t�@
	std::string bfname;
	cv::Mat frame, gray;

	int tr_eltime; // ���s�J�n�ォ��BL���N�܂ł̌o�ߎ���

	cout << "---�h���ω����o����1�i���K�Łj---" << endl;

	//cout << "�����Q����=";
	//cin >> subnm;

	subnm = "ren"; // ���K��

				   // BL���o�͈͐ݒ�E�������l�t�@�C���̓ǂݏo��
	bfname = "threshold.csv";
	ifstream finb(bfname);
	if (!finb) {
		cout << "���o�͈͗p�t�@�C��������܂���" << bfname << endl;
		getchar();
		getchar();
		return 0;
	}

	// �ݒ�͈͓ǂݏo��
	finb >> sbuf; // 1�s���ǂݔ�΂�
	finb >> is;
	finb >> js;
	finb >> ie;
	finb >> je;
	cout << "�ݒ�͈�" << endl;
	cout << is << "," << js << "," << ie << "," << je << endl;

	//�������l�̓ǂݏo��
	cout << "�������l" << endl;
	finb >> sbuf; // 1�s���ǂݔ�΂�
	for (i = 0; i < TH_TR_NUM; i++) {
		finb >> th[i];
		cout << th[i] << endl;
	}
	finb.close();

	// �������l�ݒ�(���ϒl�Ƃ���j2016.5.4
	bl_th_val = 0;
	for (i = 0; i < TH_TR_NUM; i++) {
		bl_th_val += th[i];
	}
	bl_th_val /= TH_TR_NUM;
	cout << "BL���o�������l=" << bl_th_val << endl;

	cout << "�m�F�ł����牽�����͂��Ă�������" << endl;
	getchar();
	getchar();


	// �h���񎦏����f�[�^�ǂݍ���
	std::string sfname;

	sfname = "c:\\IEEJ_C_2016\\";
	sfname = sfname + "exp1" + "\\" + subnm + "\\order_";
	sfname = sfname + "����1" + "_" + subnm + ".csv"; // �t�@�C�����̐���

													// �h���񎦏����f�[�^�t�@�C���̓ǂݍ���
	ifstream fin(sfname);
	if (!fin) {
		cout << "�h���񎦗p�t�@�C��������܂���" << endl;
		getchar();
		getchar();
		return 0;
	}


	// �ω����̓ǂݎ��
	fin >> sbuf; // 1�s���ǂݔ�΂�

				 // ���s�ԍ��A�ω��ʒu�A�ω��O�h���A�ω���h��
	for (tr = 0; tr < TR_NUM; tr++) {
		fin >> itmp1 >> split >> itmp2 >> split >> itmp3 >> split >> itmp4;
		chpos[tr] = itmp2;
		prst[tr] = itmp3;
		chst[tr] = itmp4;
		//cout << tr+1 << ","<< chpos[tr] << "," << prst[tr] << "," << chst[tr]<< endl;//�m�F
	}

	// �񎦃p�^�[���i���ӂW�ӏ��j
	fin >> sbuf; // 1�s���ǂݔ�΂�

	for (tr = 0; tr<TR_NUM; tr++) {
		fin >> itmp1;// ���s�ԍ��ǂݔ�΂�
		for (i = 0; i < 8; i++) {
			fin >> split >> itmp1;
			st[tr][i] = itmp1;
		}
		fin >> split;
	}
	fin.close();

	// �񎦏��̓ǂݍ��݌��ʂ̊m�F
	cout << "���s�ԍ�,�ω��ʒu,���ݎh���ԍ�,�ω���h���ԍ�" << endl;
	for (tr = 0; tr < TR_NUM; tr++) {
		cout << tr + 1 << ":" << chpos[tr] << "," << prst[tr] << "," << chst[tr] << endl;
	}
	cout << "���s�ԍ�,�񎦎h���i�e���ӈʒu)" << endl;
	for (tr = 0; tr < TR_NUM; tr++) {
		cout << tr + 1 << ":";

		for (i = 0; i < 8; i++) {
			cout << st[tr][i] << ",";
		}
		cout << endl;
	}

	// �ڗ̈�ʒu����
	cout << "�ڗ̈�̒������s���܂�" << endl;
	adjust_eye_position(is, js, ie, je);

	cout << "���K���n�߂܂����F�����L�[�������Ă�������";
	getchar();
	getchar();


	// ���s�J�n����

	// �񓚏��L�^����
	std::string fname, imname, eyename;
	std::ostringstream ostr;

	fname = "c:\\IEEJ_C_2016\\";
	fname = fname + "exp1" + "\\" + subnm + "\\info_";
	fname = fname + "����1" + "_" + subnm + ".csv"; // �t�@�C�����̐���

												  // �񓚏��o�͗p�t�@�C���̐���
	ofstream fout(fname); // �o�̓t�@�C�����쐬����
	if (!fout) {
		cout << "�t�@�C�����J���܂���" << endl;
		getchar();
		getchar();
		return 0;
	}
	fout << "���s�ԍ�,�ω�����,BL���N,�ω��ʒu" << endl;

	char *window_info = "info";
	//cv::namedWindow(window_info, CV_WINDOW_AUTOSIZE);							      �@// �h���񎦗pwindow�̐���
	//cv::moveWindow(window_info, (DH_SIZE - SW_HSIZE) / 2, (DV_SIZE - SW_VSIZE) / 2);  // ��ʒ����ֈړ�
	cv::Mat background(cv::Size(DH_SIZE, DV_SIZE), CV_8UC3, cv::Scalar(255, 255, 255)); // Window�ɕ\������w�i�̏���

																						//�񎦎h���̏���
	char mark[][10] = { "X", "O", "H", "E" }; // �P�o�C�g������I��


											  //�����񎦈ʒu�̒��S�ʒu�i�����_�ʒu�j
	basei = DH_SIZE / 2;
	basej = DV_SIZE / 2;

	// �h���z�u�̎Z�o
	dtmp = OBDIST*tan(INTDEG*PAI / 180.0);
	dtmp /= PITCH;
	dpix = (int)dtmp;

	// �����J�n!
	//for (tr = 0; tr < TR_NUM; tr++){ // TR_NUM4��������{
	for (tr = 0; tr < 10; tr++) {  // 10����K���{
		cv::Mat background(cv::Size(DH_SIZE, DV_SIZE), CV_8UC3, cv::Scalar(255, 255, 255)); // Window�ɕ\������w�i�̏���
		cv::imshow(window_info, background);  // �摜�\��
		cv::waitKey(3 * 1000);                // �w�i��3�b�Ԓ�

											  // �񎦎h���̏���
		int pcnt = 0;
		for (j = -1; j <= 1; j++) {
			for (i = -1; i <= 1; i++) {
				if (i == 0 && j == 0) continue; // ��ʒ����͕ʂ̋L����񎦂���̂ŃX�L�b�v
				px = basei + i*dpix; // ����̒񎦈ʒu
				py = basej + j*dpix; // ����̒񎦈ʒu
				k = st[tr][pcnt];    // �񎦈ʒupcnt�֒񎦂���h���ԍ��̐ݒ�
				cv::putText(background, (const char*)mark[k], cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0, 0, 0), 3, CV_AA);
				pcnt++; // ���̒񎦈ʒu(0,1,2,�E�E�E,7)
			}//for(i=
		}//for(j=

		 // ��ʒ����ւ̋L���i�����_�񎦁j
		px = basei; // �񎦈ʒu(���j
		py = basej; // �񎦈ʒu(�c)
		cv::putText(background, "+", cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(128, 128, 128), 3, CV_AA);

		// �h����
		//startime = (double)cvGetTickCount();  // �J�n�����̎擾
		cv::imshow(window_info, background);    // �摜�\��(���̎��_�ł͂܂��񎦊J�n����Ă��Ȃ��j
		cv::waitKey(30);                        // �h���摜�̒�

												//�h���񎦉�ʂ̕ω�����
		k0 = prst[tr]; // �ω��O�̎h���ԍ�
		k1 = chst[tr]; // �ω���̎h���ԍ�
		get_ij(chpos[tr], &i, &j); // �ω��ʒu��i,j�ԍ��̎擾
		px = basei + i*dpix;       // �ω��ʒu�̐ݒ�
		py = basej + j*dpix;       // �ω��ʒu�̐ݒ�

		int result = -1;
		result = check_volbl_occurence(10 * 1000, &tr_eltime, bl_th_val, is, js, ie, je); // ���Ӑ�BL�̃`�F�b�N(10�b�ԁj 
		if (result == 1) {      // 10�b�Ԃŉ���BL����������
			time_delay(15);    // 15ms�ҋ@���A�ق�������Ԃ̂Ƃ��Ɏh����؂�ւ���(���̂��炢���悳�����j
			cv::putText(background, mark[k0], cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(255, 255, 255), 5, CV_AA); // �Â��L��������
			cv::putText(background, mark[k1], cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0, 0, 0), 3, CV_AA);       // �V�����L����`��
			cv::imshow(window_info, background); // �L���\���X�V
			cv::waitKey(5);                      // �����ɕ`��
			cap >> gframe;                       // �X�V���_�̖ڗ̈�f���̎擾
			time_delay(3 * 1000);                  // 3�b�ҋ@
		}

		// 10�b��,���Ӑ�BL�������Ȃ������ꍇ�́A���s�𖳌��Ƃ���B�^�C���A�E�g
		if (result == 0) {
			fout << tr << "," << tr_eltime << "," << -1 << "," << -1 << endl; // �����Ȏ��s�Ƃ��ċL�^����
			cv::destroyWindow(window_info);  // window�̔j��
			if (tr != 0 && (tr % 10) == 0) {  // ���s10�񖈂ɖڗ̈�ʒu����
				adjust_eye_position(is, js, ie, je);
			}
			continue;
		}

		// �ڗ̈�摜�L�^
		std::ostringstream ostr1;
		ostr1 << "_tr" << tr;

		eyename = "c:\\IEEJ_C_2016\\";
		eyename = eyename + "exp1" + "\\" + subnm + "\\eye_����1_";
		eyename = eyename + subnm + ostr1.str() + ".bmp";

		cv::imwrite(eyename, gframe);   // �L���X�V���̉摜���L�^
		cv::destroyWindow(window_info); // window�̔j��


										// �񓚓��́i�R���\�[���Łj
		cout << endl;
		cout << "���s�ԍ�:" << tr + 1 << "/" << TR_NUM << endl << endl;
		cout << "------------------------------------------" << endl;
		cout << "�ǂ��̈ʒu���ω������������Ă�������" << endl;
		cout << "1:����i�A2:������i�A3:�E��i" << endl;
		cout << "4:�����i�A�@�@�@�@�@�@5:�E���i" << endl;
		cout << "6:�����i�A7:�������i�A8:�E���i" << endl;
		cout << endl;
		cout << "9:�킩��Ȃ�" << endl;
		cin >> answer;

		fout << tr << "," << tr_eltime << "," << 0 << "," << answer << endl;

		if (tr != 0 && (tr % 10) == 0) { // ���s10�񖈂ɖڗ̈�ʒu����
			adjust_eye_position(is, js, ie, je);
		}

	}//for(tr=
	fout.close();
	return 1;
}








//2016.4.13�` for C����2016
/*-------------------------------*/
/* �h���ω����o����1             */
/* ���Ӑ��u��                    */
/*-------------------------------*/
//
//  C����2016�p
//  ���Ӑ�BL�̃^�C�~���O�Ő؂�ւ�
//�@�؂�ւ����_�̖ڗ̈�f�����L�^
//
//  BL���o�͑g�ݍ���ł��Ȃ���,�����OK(2016.4.23)
//  BL���o����(2016.5.4)
//  ���s10�񖈂ɖڗ̈�ʒu�̔��������s��(2016.5.4)
//
int cb_exp1_c2016()
{
	static int st[TR_NUM][8]; // �e���s�ɂ�����8�ӏ��̎��ӈʒu�ɒ񎦂���h���ԍ�(0,1,2,3)�@
	int chpos[TR_NUM];        // �e���s�ɂ����Ďh�����ω�������ӈʒu(0�F�����,1,�E�E�E,7�F�E����)
	int prst[TR_NUM];         // �ω��O�̎h���ԍ�
	int chst[TR_NUM];         // �ω����ɒ񎦂���h���ԍ�

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

	std::string subnm; // �����Q���Җ�
	std::string sbuf, sbuf1, sbuf2, sbuf3, sbuf4;  // �o�b�t�@
	std::string bfname;
	cv::Mat frame, gray;

	int tr_eltime; // ���s�J�n�ォ��BL���N�܂ł̌o�ߎ���

	cout << "---�h���ω����o����1---" << endl;

	cout << "�����Q����=";
	cin >> subnm;

	// BL���o�͈͐ݒ�E�������l�t�@�C���̓ǂݏo��
	bfname = "threshold.csv";
	ifstream finb(bfname);
	if (!finb) {
		cout << "���o�͈͗p�t�@�C��������܂���" << bfname << endl;
		getchar();
		getchar();
		return 0;
	}

	// �ݒ�͈͓ǂݏo��
	finb >> sbuf; // 1�s���ǂݔ�΂�
	finb >> is;
	finb >> js;
	finb >> ie;
	finb >> je;
	cout << "�ݒ�͈�" << endl;
	cout << is << "," << js << "," << ie << "," << je << endl;

	//�������l�̓ǂݏo��
	cout << "�������l" << endl;
	finb >> sbuf; // 1�s���ǂݔ�΂�
	for (i = 0; i < TH_TR_NUM; i++) {
		finb >> th[i];
		cout << th[i] << endl;
	}
	finb.close();

	// �������l�ݒ�(���ϒl�Ƃ���j2016.5.4
	bl_th_val = 0;
	for (i = 0; i < TH_TR_NUM; i++) {
		bl_th_val += th[i];
	}
	bl_th_val /= TH_TR_NUM;
	cout << "BL���o�������l=" << bl_th_val << endl;

	cout << "�m�F�ł����牽�����͂��Ă�������" << endl;
	getchar();
	getchar();


	// �h���񎦏����f�[�^�ǂݍ���
	std::string sfname;

	sfname = "c:\\IEEJ_C_2016\\";
	sfname = sfname + "exp1" + "\\" + subnm + "\\order_";
	sfname = sfname + "����1" + "_" + subnm + ".csv"; // �t�@�C�����̐���

													// �h���񎦏����f�[�^�t�@�C���̓ǂݍ���
	ifstream fin(sfname);
	if (!fin) {
		cout << "�h���񎦗p�t�@�C��������܂���" << endl;
		getchar();
		getchar();
		return 0;
	}


	// �ω����̓ǂݎ��
	fin >> sbuf; // 1�s���ǂݔ�΂�

				 // ���s�ԍ��A�ω��ʒu�A�ω��O�h���A�ω���h��
	for (tr = 0; tr < TR_NUM; tr++) {
		fin >> itmp1 >> split >> itmp2 >> split >> itmp3 >> split >> itmp4;
		chpos[tr] = itmp2;
		prst[tr] = itmp3;
		chst[tr] = itmp4;
		//cout << tr+1 << ","<< chpos[tr] << "," << prst[tr] << "," << chst[tr]<< endl;//�m�F
	}

	// �񎦃p�^�[���i���ӂW�ӏ��j
	fin >> sbuf; // 1�s���ǂݔ�΂�

	for (tr = 0; tr<TR_NUM; tr++) {
		fin >> itmp1;// ���s�ԍ��ǂݔ�΂�
		for (i = 0; i < 8; i++) {
			fin >> split >> itmp1;
			st[tr][i] = itmp1;
		}
		fin >> split;
	}
	fin.close();

	// �񎦏��̓ǂݍ��݌��ʂ̊m�F
	cout << "���s�ԍ�,�ω��ʒu,���ݎh���ԍ�,�ω���h���ԍ�" << endl;
	for (tr = 0; tr < TR_NUM; tr++) {
		cout << tr + 1 << ":" << chpos[tr] << "," << prst[tr] << "," << chst[tr] << endl;
	}
	cout << "���s�ԍ�,�񎦎h���i�e���ӈʒu)" << endl;
	for (tr = 0; tr < TR_NUM; tr++) {
		cout << tr + 1 << ":";

		for (i = 0; i < 8; i++) {
			cout << st[tr][i] << ",";
		}
		cout << endl;
	}

	// �ڗ̈�ʒu����
	cout << "�ڗ̈�̒������s���܂�" << endl;
	adjust_eye_position(is, js, ie, je);

	cout << "�������n�߂܂����F�����L�[�������Ă�������";
	getchar();
	getchar();


	// ���s�J�n����

	// �񓚏��L�^����
	std::string fname, imname, eyename;
	std::ostringstream ostr;

	fname = "c:\\IEEJ_C_2016\\";
	fname = fname + "exp1" + "\\" + subnm + "\\info_";
	fname = fname + "����1" + "_" + subnm + ".csv"; // �t�@�C�����̐���

												  // �񓚏��o�͗p�t�@�C���̐���
	ofstream fout(fname); // �o�̓t�@�C�����쐬����
	if (!fout) {
		cout << "�t�@�C�����J���܂���" << endl;
		getchar();
		getchar();
		return 0;
	}
	fout << "���s�ԍ�,�ω�����,BL���N,�ω��ʒu" << endl;

	char *window_info = "info";
	//cv::namedWindow(window_info, CV_WINDOW_AUTOSIZE);							      �@// �h���񎦗pwindow�̐���
	//cv::moveWindow(window_info, (DH_SIZE - SW_HSIZE) / 2, (DV_SIZE - SW_VSIZE) / 2);  // ��ʒ����ֈړ�
	cv::Mat background(cv::Size(DH_SIZE, DV_SIZE), CV_8UC3, cv::Scalar(255, 255, 255)); // Window�ɕ\������w�i�̏���

																						//�񎦎h���̏���
	char mark[][10] = { "X", "O", "H", "E" }; // �P�o�C�g������I��


											  //�����񎦈ʒu�̒��S�ʒu�i�����_�ʒu�j
	basei = DH_SIZE / 2;
	basej = DV_SIZE / 2;

	// �h���z�u�̎Z�o
	dtmp = OBDIST*tan(INTDEG*PAI / 180.0);
	dtmp /= PITCH;
	dpix = (int)dtmp;

	// �����J�n!
	for (tr = 0; tr < TR_NUM; tr++) { // TR_NUM��������{
									  //for (tr = 0; tr < 1; tr++){ // TR_NUM��������{
		cv::Mat background(cv::Size(DH_SIZE, DV_SIZE), CV_8UC3, cv::Scalar(255, 255, 255)); // Window�ɕ\������w�i�̏���
		cv::imshow(window_info, background);  // �摜�\��
		cv::waitKey(3 * 1000);                // �w�i��3�b�Ԓ�

											  // �񎦎h���̏���
		int pcnt = 0;
		for (j = -1; j <= 1; j++) {
			for (i = -1; i <= 1; i++) {
				if (i == 0 && j == 0) continue; // ��ʒ����͕ʂ̋L����񎦂���̂ŃX�L�b�v
				px = basei + i*dpix; // ����̒񎦈ʒu
				py = basej + j*dpix; // ����̒񎦈ʒu
				k = st[tr][pcnt];    // �񎦈ʒupcnt�֒񎦂���h���ԍ��̐ݒ�
				cv::putText(background, (const char*)mark[k], cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0, 0, 0), 3, CV_AA);
				pcnt++; // ���̒񎦈ʒu(0,1,2,�E�E�E,7)
			}//for(i=
		}//for(j=

		 // ��ʒ����ւ̋L���i�����_�񎦁j
		px = basei; // �񎦈ʒu(���j
		py = basej; // �񎦈ʒu(�c)
		cv::putText(background, "+", cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(128, 128, 128), 3, CV_AA);

		// �h����
		//startime = (double)cvGetTickCount();  // �J�n�����̎擾
		cv::imshow(window_info, background);    // �摜�\��(���̎��_�ł͂܂��񎦊J�n����Ă��Ȃ��j
		cv::waitKey(30);                        // �h���摜�̒�

												//�h���񎦉�ʂ̕ω�����
		k0 = prst[tr]; // �ω��O�̎h���ԍ�
		k1 = chst[tr]; // �ω���̎h���ԍ�
		get_ij(chpos[tr], &i, &j); // �ω��ʒu��i,j�ԍ��̎擾
		px = basei + i*dpix;       // �ω��ʒu�̐ݒ�
		py = basej + j*dpix;       // �ω��ʒu�̐ݒ�

		int result = -1;
		result = check_volbl_occurence(10 * 1000, &tr_eltime, bl_th_val, is, js, ie, je); // ���Ӑ�BL�̃`�F�b�N(10�b�ԁj 
		if (result == 1) {      // 10�b�Ԃŉ���BL����������
			time_delay(15);    // 15ms�ҋ@���A�ق�������Ԃ̂Ƃ��Ɏh����؂�ւ���(���̂��炢���悳�����j
			cv::putText(background, mark[k0], cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(255, 255, 255), 5, CV_AA); // �Â��L��������
			cv::putText(background, mark[k1], cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0, 0, 0), 3, CV_AA);       // �V�����L����`��
			cv::imshow(window_info, background); // �L���\���X�V
			cv::waitKey(5);                      // �����ɕ`��
			cap >> gframe;                       // �X�V���_�̖ڗ̈�f���̎擾
			time_delay(3 * 1000);                  // 3�b�ҋ@
		}

		// 10�b��,���Ӑ�BL�������Ȃ������ꍇ�́A���s�𖳌��Ƃ���B�^�C���A�E�g
		if (result == 0) {
			fout << tr << "," << tr_eltime << "," << -1 << "," << -1 << endl; // �����Ȏ��s�Ƃ��ċL�^����
			cv::destroyWindow(window_info);  // window�̔j��
			if (tr != 0 && (tr % 10) == 0) {  // ���s10�񖈂ɖڗ̈�ʒu����
				adjust_eye_position(is, js, ie, je);
			}
			continue;
		}

		// �ڗ̈�摜�L�^
		std::ostringstream ostr1;
		ostr1 << "_tr" << tr;

		eyename = "c:\\IEEJ_C_2016\\";
		eyename = eyename + "exp1" + "\\" + subnm + "\\eye_����1_";
		eyename = eyename + subnm + ostr1.str() + ".bmp";

		cv::imwrite(eyename, gframe);   // �L���X�V���̉摜���L�^
		cv::destroyWindow(window_info); // window�̔j��


										// �񓚓��́i�R���\�[���Łj
		cout << endl;
		cout << "���s�ԍ�:" << tr + 1 << "/" << TR_NUM << endl << endl;
		cout << "------------------------------------------" << endl;
		cout << "�ǂ��̈ʒu���ω������������Ă�������" << endl;
		cout << "1:����i�A2:������i�A3:�E��i" << endl;
		cout << "4:�����i�A�@�@�@�@�@�@5:�E���i" << endl;
		cout << "6:�����i�A7:�������i�A8:�E���i" << endl;
		cout << endl;
		cout << "9:�킩��Ȃ�" << endl;
		cin >> answer;

		fout << tr << "," << tr_eltime << "," << 0 << "," << answer << endl;

		if (tr != 0 && (tr % 10) == 0) { // ���s10�񖈂ɖڗ̈�ʒu����
			adjust_eye_position(is, js, ie, je);
		}

	}//for(tr=
	fout.close();
	return 1;
}

// 2016.4.23�`
/*------------------*/
/* BL���N�̃`�F�b�N */
/*------------------*/
//�@
// �߂�l:0 �^�C���A�b�v�ABL���N�Ȃ��B
//        1 BL���N
//
int check_volbl_occurence(
	int maxdur,    // �ő�҂�����(ms)
	int *eltim,    // ���s�J�n��̌o�ߎ���
	int blth,      // BL���o���̂������l
	int is,        // ���o�͈�
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


	// �摜�\���p�E�B���h�E�̐���(���摜�j
	//char *window_input = "input";
	//cv::namedWindow(window_input, CV_WINDOW_AUTOSIZE);

	// �摜�\���p�E�B���h�E�̐���(�G�b�W�摜�j
	//char *window_output = "output";
	//cv::namedWindow(window_output, CV_WINDOW_AUTOSIZE);


	int canny_th1 = CANNY_TH1; // �M�����̍����G�b�W��臒l�i�Œ�l�Ƃ���j
	int canny_th2 = CANNY_TH2; // �M�����ɒႢ�G�b�W��臒l�i�Œ�l�Ƃ���j

	start = cvGetTickCount(); // �J�n����
	fr = 0;
	while (1) {
		eltime = cvGetTickCount() - start; // �o�ߎ���
		if ((int)(eltime / (1000 * cvGetTickFrequency())) > maxdur) {
			*eltim = (int)(eltime / (1000 * cvGetTickFrequency()));
			//fout.close(); 
			return 0; // �^�C���A�E�g
		}
		cap >> frame;
		cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);	// �J���[�摜�����m�N���摜�ɕϊ�
		cv::Canny(gray, gray, canny_th1, canny_th2, 3); // �L���j�[�I�y���[�^�ɂ��G�b�W���o

														//cv::rectangle(frame, cv::Point(is, js), cv::Point(ie, je), cv::Scalar(0, 0, 255), 1, 1);    // �֐S�̈��`�悷��
														//cv::rectangle(gray, cv::Point(is, js), cv::Point(ie, je), cv::Scalar(255, 255, 255), 1, 1); // �֐S�̈��`�悷��
														//cv::imshow(window_input, frame); // ���摜�̕\��
														//cv::imshow(window_output, gray); // �G�b�W���o�摜
														//cv::waitKey(10);                 // �摜�\�� 

		edge_sum[fr] = count_edge_sum(gray, is, js, ie, je); // �G�b�W����
		if (fr < FR_DUR) {
			fr++;
			continue;
		}
		th = 0;
		for (i = fr - 1; i > fr - 1 - FR_DUR; i--) {// �ߋ�FR_DUR�t���[�����i���t���[�����܂܂Ȃ��j�̕��ϒl
			th += edge_sum[i]; // �G�b�W�_���̐ώZ
		}
		th /= FR_DUR;

		//fout << fr << "," << edge_sum[fr] << "," << th << "," << edge_sum[fr] - th << endl;


		if (edge_sum[fr] - th < blth) { // BL���o(�������l�ȏ�̓�������j
			*eltim = (int)(eltime / (1000 * cvGetTickFrequency()));
			break;

		}
		fr++;

		// BL���菈���̑���i�b��F2016.4.23)
		//if (cv::waitKey(30) >= 0){ // �Ȃɂ��L�[�������ꂽ��(BL����������j�I��
		//	*eltim = (int)(eltime / (1000 * cvGetTickFrequency()));
		//	break;
		//}

	}//while(1)
	return 1; // BL���N
}


// 2016.5.4
/*--------------------*/
/* �ڗ̈�̈ʒu�̒��� */
/*--------------------*/
//
// �\�����ꂽ�g���ɖڂ�����悤�ɒ�������
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
	cv::moveWindow(window_input, 0, 0);  // ��ʍ�����ֈړ�
	while (1) {
		cap >> frame;
		msg = "Move your eye-region into red ploted square frame";
		cv::putText(frame, msg, cv::Point(10, 40), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 0, 0), 1, CV_AA); // ���b�Z�[�W�\��
		cv::rectangle(frame, cv::Point(is, js), cv::Point(ie, je), cv::Scalar(0, 0, 255), 1, 1);                    // �֐S�̈��`�悷��
		cv::imshow(window_input, frame);
		if (cv::waitKey(30) > 0) { // �����L�[�������ꂽ��
			break;
		}
	}
	cv::destroyWindow(window_input); // �E�B���h�E�̔j��
	return 1;
}


//2016.2.28�` for SSII2016
/*-------------------------------*/
/* �h���ω����o����3             */
/* CB���ۂ𔺂���Ԃł̌��o����  */
/* ���ӗU����                  */
/*-------------------------------*/
//
//  SSII2016�p
//�@�h�����ω�����ʒu�̕����������i4�����F�㉺���E�j
//  �����SW���͂Ɠ����ɐ��Ӑ���BL���s��
//  SW���͂���100ms��̎��_�Ő؂�ւ���
//�@�؂�ւ����_�̖ڗ̈�f�����L�^���Ă���
//

int cb_exp3_ssii()
{
	static int st[TR_NUM][8]; // �e���s�ɂ�����8�ӏ��̎��ӈʒu�ɒ񎦂���h���ԍ�(0,1,2,3)�@
	int chpos[TR_NUM];        // �e���s�ɂ����Ďh�����ω�������ӈʒu(0�F�����,1,�E�E�E,7�F�E����)
	int prst[TR_NUM];         // �ω��O�̎h���ԍ�
	int chst[TR_NUM];         // �ω����ɒ񎦂���h���ԍ�
	int att[TR_NUM];          // ���ӂ̗U������

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

	std::string subnm; // �����Q���Җ�
	std::string sbuf, sbuf1, sbuf2, sbuf3, sbuf4;  // �o�b�t�@
	cv::Mat frame, gray;
	cv::Mat right, left, down, upper; // ���ӂ̕������������摜

									  // ���摜�̓ǂݍ���
	right = cv::imread("right_dir.bmp", cv::IMREAD_GRAYSCALE);
	if (right.empty()) {
		cout << "�t�@�C��������܂���" << endl;
		return 0;
	}

	left = cv::imread("left_dir.bmp", cv::IMREAD_GRAYSCALE);
	if (left.empty()) {
		cout << "�t�@�C��������܂���" << endl;
		return 0;
	}

	down = cv::imread("down_dir.bmp", cv::IMREAD_GRAYSCALE);
	if (down.empty()) {
		cout << "�t�@�C��������܂���" << endl;
		return 0;
	}

	upper = cv::imread("upper_dir.bmp", cv::IMREAD_GRAYSCALE);
	if (upper.empty()) {
		cout << "�t�@�C��������܂���" << endl;
		return 0;
	}

	// ���摜�̃T�C�Y�擾
	w = upper.cols; // ��
	h = upper.rows; // ����


	cout << "---�h���ω����o����3---" << endl;

	cout << "�����Q����=";
	cin >> subnm;

	// �h���񎦏����f�[�^�ǂݍ���
	std::string sfname;
	sfname = "order_";
	sfname += "����3_" + subnm + ".csv"; // �t�@�C�����̐���

									   // �h���񎦏����f�[�^�t�@�C���̓ǂݍ���
	ifstream fin(sfname);
	if (!fin) {
		cout << "�h���񎦗p�t�@�C��������܂���" << endl;
		return 0;
	}

	// �ω����̓ǂݎ��
	fin >> sbuf; // 1�s���ǂݔ�΂�

				 // ���s�ԍ��A�ω��ʒu�A�ω��O�h���A�ω���h���A�U������
	for (tr = 0; tr < TR_NUM; tr++) {
		fin >> itmp1 >> split >> itmp2 >> split >> itmp3 >> split >> itmp4 >> split >> itmp5;

		chpos[tr] = itmp2;
		prst[tr] = itmp3;
		chst[tr] = itmp4;
		att[tr] = itmp5;
		//cout << tr+1 << ","<< chpos[tr] << "," << prst[tr] << "," << chst[tr]<< endl;//�m�F
	}

	// �񎦃p�^�[���i���ӂW�ӏ��j
	fin >> sbuf; // 1�s���ǂݔ�΂�

	for (tr = 0; tr<TR_NUM; tr++) {
		fin >> itmp1;// ���s�ԍ��ǂݔ�΂�
		for (i = 0; i < 8; i++) {
			fin >> split >> itmp1;
			st[tr][i] = itmp1;
		}
		fin >> split;
	}
	fin.close();

	// �񎦏��̓ǂݍ��݌��ʂ̊m�F
	cout << "���s�ԍ�,�ω��ʒu,���ݎh���ԍ�,�ω���h���ԍ�,���ӗU������" << endl;
	for (tr = 0; tr < TR_NUM; tr++) {
		cout << tr + 1 << ":" << chpos[tr] << "," << prst[tr] << "," << chst[tr] << "," << att[tr] << endl;
	}
	cout << "���s�ԍ�,�񎦎h���i�e���ӈʒu)" << endl;
	for (tr = 0; tr < TR_NUM; tr++) {
		cout << tr + 1 << ":";

		for (i = 0; i < 8; i++) {
			cout << st[tr][i] << ",";
		}
		cout << endl;
	}

	cout << "�������n�߂܂����F�����L�[�������Ă�������";
	getchar();
	getchar();


	// ���s�J�n����

	// �񓚏��L�^����
	std::string fname, imname, eyename;
	std::ostringstream ostr;
	fname = "info_";
	fname += "����3_" + subnm + ".csv"; // �t�@�C�����̐���

									  // �񓚏��o�͗p�t�@�C���̐���
	ofstream fout(fname); // �o�̓t�@�C�����쐬����
	if (!fout) {
		cout << "�t�@�C�����J���܂���" << endl;
		return 0;
	}
	fout << "���s�ԍ�,�ω��ʒu" << endl;

	char *window_info = "info";
	//cv::namedWindow(window_info, CV_WINDOW_AUTOSIZE);							      �@// �h���񎦗pwindow�̐���
	//cv::moveWindow(window_info, (DH_SIZE - SW_HSIZE) / 2, (DV_SIZE - SW_VSIZE) / 2);  // ��ʒ����ֈړ�
	cv::Mat background(cv::Size(DH_SIZE, DV_SIZE), CV_8UC3, cv::Scalar(255, 255, 255)); // Window�ɕ\������w�i�̏���

																						//�񎦎h���̏���
	char mark[][10] = { "X", "O", "H", "E" }; // �P�o�C�g������I��


											  //�����񎦈ʒu�̒��S�ʒu�i�����_�ʒu�j
	basei = DH_SIZE / 2;
	basej = DV_SIZE / 2;

	// �h���z�u�̎Z�o
	dtmp = OBDIST*tan(INTDEG*PAI / 180.0);
	dtmp /= PITCH;
	dpix = (int)dtmp;

	// �����J�n!
	for (tr = 0; tr < TR_NUM; tr++) { // TR_NUM��������{
		cv::Mat background(cv::Size(DH_SIZE, DV_SIZE), CV_8UC3, cv::Scalar(255, 255, 255)); // Window�ɕ\������w�i�̏���
		cv::imshow(window_info, background);  // �摜�\��
		cv::waitKey(3 * 1000);                // �w�i��3�b�Ԓ�
											  // �񎦎h���̏���

		int pcnt = 0;
		for (j = -1; j <= 1; j++) {
			for (i = -1; i <= 1; i++) {
				if (i == 0 && j == 0) continue; // ��ʒ����͕ʂ̋L����񎦂���̂ŃX�L�b�v
				px = basei + i*dpix; // ����̒񎦈ʒu
				py = basej + j*dpix; // ����̒񎦈ʒu
				k = st[tr][pcnt];    // �񎦈ʒupcnt�֒񎦂���h���ԍ��̐ݒ�
				cv::putText(background, (const char*)mark[k], cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0, 0, 0), 3, CV_AA);
				pcnt++; // ���̒񎦈ʒu(0,1,2,�E�E�E,7)
			}//for(i=
		}//for(j=

		 // ��ʒ����֒��ӗU���p�̖��L�����
		int att_dir;
		for (j = 0; j < h; j++) {
			for (i = 0; i < w; i++) {
				if (att[tr] == 0) { // ���
					background.data[(DH_SIZE*(basej - h + j) + basei + i) * 3 + 0] = upper.data[j*w + i];
					background.data[(DH_SIZE*(basej - h + j) + basei + i) * 3 + 1] = upper.data[j*w + i];
					background.data[(DH_SIZE*(basej - h + j) + basei + i) * 3 + 2] = upper.data[j*w + i];
				}

				if (att[tr] == 1) { //������
					background.data[(DH_SIZE*(basej - h + j) + basei + i) * 3 + 0] = left.data[j*w + i];
					background.data[(DH_SIZE*(basej - h + j) + basei + i) * 3 + 1] = left.data[j*w + i];
					background.data[(DH_SIZE*(basej - h + j) + basei + i) * 3 + 2] = left.data[j*w + i];
				}

				if (att[tr] == 2) { //����
					background.data[(DH_SIZE*(basej - h + j) + basei + i) * 3 + 0] = down.data[j*w + i];
					background.data[(DH_SIZE*(basej - h + j) + basei + i) * 3 + 1] = down.data[j*w + i];
					background.data[(DH_SIZE*(basej - h + j) + basei + i) * 3 + 2] = down.data[j*w + i];
				}

				if (att[tr] == 3) { //�E����
					background.data[(DH_SIZE*(basej - h + j) + basei + i) * 3 + 0] = right.data[j*w + i];
					background.data[(DH_SIZE*(basej - h + j) + basei + i) * 3 + 1] = right.data[j*w + i];
					background.data[(DH_SIZE*(basej - h + j) + basei + i) * 3 + 2] = right.data[j*w + i];
				}
			}
		}

		//px = basei; // �񎦈ʒu(���j
		//py = basej; // �񎦈ʒu(�c)
		//cv::putText(background, "+", cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(128, 128, 128), 3, CV_AA);

		// �h����
		//startime = (double)cvGetTickCount();  // �J�n�����̎擾
		cv::imshow(window_info, background);    // �摜�\��
												//cv::waitKey(30);                      // �h���摜�̒�

												//�h���񎦉�ʂ̕ω�����
		k0 = prst[tr]; // �ω��O�̎h���ԍ�
		k1 = chst[tr]; // �ω���̎h���ԍ�
		get_ij(chpos[tr], &i, &j); // �ω��ʒu��i,j�ԍ��̎擾
		px = basei + i*dpix;       // �ω��ʒu�̐ݒ�
		py = basej + j*dpix;       // �ω��ʒu�̐ݒ�

		int result;
		result = cv::waitKey(10 * 1000); // 10�b�ԂȂ��ŃL�[���͂�҂�
		if (result != -1) {     // 10�b�Ԃŉ����L�[�����͂��ꂽ��
			cv::waitKey(100); // 100ms�ҋ@���A�ق�������Ԃ̂Ƃ��Ɏh����؂�ւ���
			cv::putText(background, mark[k0], cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(255, 255, 255), 5, CV_AA); // �Â��L��������
			cv::putText(background, mark[k1], cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0, 0, 0), 3, CV_AA);     // �V�����L����`��
			cv::imshow(window_info, background);	// �L���\���X�V
			cap >> gframe;                          // �X�V���_�̖ڗ̈�f���̎擾
			cv::waitKey(3 * 1000);                  // 3�b�ҋ@
		}

		//10�b�ԃL�[���͂��Ȃ������ꍇ�́A���s�𖳌��Ƃ���B
		if (result == -1) {
			fout << tr << "," << -1 << endl; // �����Ȏ��s�Ƃ��ċL�^����
			cv::destroyWindow(window_info);  // window�̔j��
			continue;
		}

		// �ڗ̈�摜�L�^
		std::ostringstream ostr1;
		ostr1 << "_tr" << tr;
		eyename = "";
		eyename = "eye_����3_" + subnm + ostr1.str() + ".bmp";
		cv::imwrite(eyename, gframe);   // BL���̉摜���L�^

		cv::destroyWindow(window_info); // window�̔j��


										// �񓚓��́i�R���\�[���Łj
		cout << endl;
		cout << "���s�ԍ�:" << tr + 1 << "/" << TR_NUM << endl << endl;
		cout << "------------------------------------------" << endl;
		cout << "�ǂ��̈ʒu���ω������������Ă�������" << endl;
		cout << "1:����i�A2:������i�A3:�E��i" << endl;
		cout << "4:�����i�A�@�@�@�@�@�@5:�E���i" << endl;
		cout << "6:�����i�A7:�������i�A8:�E���i" << endl;
		cout << endl;
		cout << "9:�킩��Ȃ�" << endl;
		cin >> answer;

		fout << tr << "," << answer << endl;
	}//for(tr=
	fout.close();
	return 1;
}




//2016.2.28�` for SSII2016
/*-------------------------------*/
/* �h���ω����o����3(���K�Łj    */
/* CB���ۂ𔺂���Ԃł̌��o����  */
/* ���ӗU����                  */
/*-------------------------------*/
//
//  SSII2016�p
//�@�h�����ω�����ʒu�̕����������i4�����F�㉺���E�j
//  �����SW���͂Ɠ����ɐ��Ӑ���BL���s��
//  SW���͂���100ms��̎��_�Ő؂�ւ���
//�@�؂�ւ����_�̖ڗ̈�f�����L�^���Ă���
//

int cb_exp3_ssii_ren()
{
	static int st[TR_NUM][8]; // �e���s�ɂ�����8�ӏ��̎��ӈʒu�ɒ񎦂���h���ԍ�(0,1,2,3)�@
	int chpos[TR_NUM];        // �e���s�ɂ����Ďh�����ω�������ӈʒu(0�F�����,1,�E�E�E,7�F�E����)
	int prst[TR_NUM];         // �ω��O�̎h���ԍ�
	int chst[TR_NUM];         // �ω����ɒ񎦂���h���ԍ�
	int att[TR_NUM];          // ���ӂ̗U������

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

	std::string subnm; // �����Q���Җ�
	std::string sbuf, sbuf1, sbuf2, sbuf3, sbuf4;  // �o�b�t�@
	cv::Mat frame, gray;
	cv::Mat right, left, down, upper; // ���ӂ̕������������摜

									  // ���摜�̓ǂݍ���
	right = cv::imread("right_dir.bmp", cv::IMREAD_GRAYSCALE);
	if (right.empty()) {
		cout << "�t�@�C��������܂���" << endl;
		return 0;
	}

	left = cv::imread("left_dir.bmp", cv::IMREAD_GRAYSCALE);
	if (left.empty()) {
		cout << "�t�@�C��������܂���" << endl;
		return 0;
	}

	down = cv::imread("down_dir.bmp", cv::IMREAD_GRAYSCALE);
	if (down.empty()) {
		cout << "�t�@�C��������܂���" << endl;
		return 0;
	}

	upper = cv::imread("upper_dir.bmp", cv::IMREAD_GRAYSCALE);
	if (upper.empty()) {
		cout << "�t�@�C��������܂���" << endl;
		return 0;
	}

	// ���摜�̃T�C�Y�擾
	w = upper.cols; // ��
	h = upper.rows; // ����


	cout << "---�h���ω����o����3�i���K�j---" << endl;

	cout << "�����Q����=";
	cin >> subnm;

	// �h���񎦏����f�[�^�ǂݍ���
	std::string sfname;
	sfname = "order_";
	sfname += "����3_���K_" + subnm + ".csv"; // �t�@�C�����̐���

										  // �h���񎦏����f�[�^�t�@�C���̓ǂݍ���
	ifstream fin(sfname);
	if (!fin) {
		cout << "�h���񎦗p�t�@�C��������܂���" << endl;
		return 0;
	}

	// �ω����̓ǂݎ��
	fin >> sbuf; // 1�s���ǂݔ�΂�

				 // ���s�ԍ��A�ω��ʒu�A�ω��O�h���A�ω���h���A�U������
	for (tr = 0; tr < TR_NUM; tr++) { // 
		fin >> itmp1 >> split >> itmp2 >> split >> itmp3 >> split >> itmp4 >> split >> itmp5;

		chpos[tr] = itmp2;
		prst[tr] = itmp3;
		chst[tr] = itmp4;
		att[tr] = itmp5;
		//cout << tr+1 << ","<< chpos[tr] << "," << prst[tr] << "," << chst[tr]<< endl;//�m�F
	}

	// �񎦃p�^�[���i���ӂW�ӏ��j
	fin >> sbuf; // 1�s���ǂݔ�΂�

	for (tr = 0; tr<TR_NUM; tr++) {
		fin >> itmp1;// ���s�ԍ��ǂݔ�΂�
		for (i = 0; i < 8; i++) {
			fin >> split >> itmp1;
			st[tr][i] = itmp1;
		}
		fin >> split;
	}
	fin.close();

	// �񎦏��̓ǂݍ��݌��ʂ̊m�F
	cout << "���s�ԍ�,�ω��ʒu,���ݎh���ԍ�,�ω���h���ԍ�,���ӗU������" << endl;
	for (tr = 0; tr < TR_NUM; tr++) {
		cout << tr + 1 << ":" << chpos[tr] << "," << prst[tr] << "," << chst[tr] << "," << att[tr] << endl;
	}
	cout << "���s�ԍ�,�񎦎h���i�e���ӈʒu)" << endl;
	for (tr = 0; tr < TR_NUM; tr++) {
		cout << tr + 1 << ":";

		for (i = 0; i < 8; i++) {
			cout << st[tr][i] << ",";
		}
		cout << endl;
	}

	cout << "���K���n�߂܂����F�����L�[�������Ă�������";
	getchar();
	getchar();


	// ���s�J�n����

	// �񓚏��L�^����
	std::string fname, imname, eyename;
	std::ostringstream ostr;
	fname = "info_";
	fname += "����3_���K_" + subnm + ".csv"; // �t�@�C�����̐���

										 // �񓚏��o�͗p�t�@�C���̐���
	ofstream fout(fname); // �o�̓t�@�C�����쐬����
	if (!fout) {
		cout << "�t�@�C�����J���܂���" << endl;
		return 0;
	}
	fout << "���s�ԍ�,�ω��ʒu" << endl;

	char *window_info = "info";
	//cv::namedWindow(window_info, CV_WINDOW_AUTOSIZE);							      �@// �h���񎦗pwindow�̐���
	//cv::moveWindow(window_info, (DH_SIZE - SW_HSIZE) / 2, (DV_SIZE - SW_VSIZE) / 2);  // ��ʒ����ֈړ�
	cv::Mat background(cv::Size(DH_SIZE, DV_SIZE), CV_8UC3, cv::Scalar(255, 255, 255)); // Window�ɕ\������w�i�̏���

																						//�񎦎h���̏���
	char mark[][10] = { "X", "O", "H", "E" }; // �P�o�C�g������I��


											  //�����񎦈ʒu�̒��S�ʒu�i�����_�ʒu�j
	basei = DH_SIZE / 2;
	basej = DV_SIZE / 2;

	// �h���z�u�̎Z�o
	dtmp = OBDIST*tan(INTDEG*PAI / 180.0);
	dtmp /= PITCH;
	dpix = (int)dtmp;

	// �����J�n!
	for (tr = 0; tr < TR_NUM / 4; tr++) { // TR_NUM/4��������{
		cv::Mat background(cv::Size(DH_SIZE, DV_SIZE), CV_8UC3, cv::Scalar(255, 255, 255)); // Window�ɕ\������w�i�̏���
		cv::imshow(window_info, background);  // �摜�\��
		cv::waitKey(3 * 1000);                // �w�i��3�b�Ԓ�
											  // �񎦎h���̏���

		int pcnt = 0;
		for (j = -1; j <= 1; j++) {
			for (i = -1; i <= 1; i++) {
				if (i == 0 && j == 0) continue; // ��ʒ����͕ʂ̋L����񎦂���̂ŃX�L�b�v
				px = basei + i*dpix; // ����̒񎦈ʒu
				py = basej + j*dpix; // ����̒񎦈ʒu
				k = st[tr][pcnt];    // �񎦈ʒupcnt�֒񎦂���h���ԍ��̐ݒ�
				cv::putText(background, (const char*)mark[k], cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0, 0, 0), 3, CV_AA);
				pcnt++; // ���̒񎦈ʒu(0,1,2,�E�E�E,7)
			}//for(i=
		}//for(j=

		 // ��ʒ����֒��ӗU���p�̖��L�����
		int att_dir;
		for (j = 0; j < h; j++) {
			for (i = 0; i < w; i++) {
				if (att[tr] == 0) { // ���
					background.data[(DH_SIZE*(basej - h + j) + basei + i) * 3 + 0] = upper.data[j*w + i];
					background.data[(DH_SIZE*(basej - h + j) + basei + i) * 3 + 1] = upper.data[j*w + i];
					background.data[(DH_SIZE*(basej - h + j) + basei + i) * 3 + 2] = upper.data[j*w + i];
				}

				if (att[tr] == 1) { //������
					background.data[(DH_SIZE*(basej - h + j) + basei + i) * 3 + 0] = left.data[j*w + i];
					background.data[(DH_SIZE*(basej - h + j) + basei + i) * 3 + 1] = left.data[j*w + i];
					background.data[(DH_SIZE*(basej - h + j) + basei + i) * 3 + 2] = left.data[j*w + i];
				}

				if (att[tr] == 2) { //����
					background.data[(DH_SIZE*(basej - h + j) + basei + i) * 3 + 0] = down.data[j*w + i];
					background.data[(DH_SIZE*(basej - h + j) + basei + i) * 3 + 1] = down.data[j*w + i];
					background.data[(DH_SIZE*(basej - h + j) + basei + i) * 3 + 2] = down.data[j*w + i];
				}

				if (att[tr] == 3) { //�E����
					background.data[(DH_SIZE*(basej - h + j) + basei + i) * 3 + 0] = right.data[j*w + i];
					background.data[(DH_SIZE*(basej - h + j) + basei + i) * 3 + 1] = right.data[j*w + i];
					background.data[(DH_SIZE*(basej - h + j) + basei + i) * 3 + 2] = right.data[j*w + i];
				}
			}
		}

		//px = basei; // �񎦈ʒu(���j
		//py = basej; // �񎦈ʒu(�c)
		//cv::putText(background, "+", cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(128, 128, 128), 3, CV_AA);

		// �h����
		//startime = (double)cvGetTickCount();  // �J�n�����̎擾
		cv::imshow(window_info, background);    // �摜�\��
												//cv::waitKey(30);                      // �h���摜�̒�

												//�h���񎦉�ʂ̕ω�����
		k0 = prst[tr]; // �ω��O�̎h���ԍ�
		k1 = chst[tr]; // �ω���̎h���ԍ�
		get_ij(chpos[tr], &i, &j); // �ω��ʒu��i,j�ԍ��̎擾
		px = basei + i*dpix;       // �ω��ʒu�̐ݒ�
		py = basej + j*dpix;       // �ω��ʒu�̐ݒ�

		int result;
		result = cv::waitKey(10 * 1000); // 10�b�ԂȂ��ŃL�[���͂�҂�
		if (result != -1) {     // 10�b�Ԃŉ����L�[�����͂��ꂽ��
			cv::waitKey(100); // 100ms�ҋ@���A�ق�������Ԃ̂Ƃ��Ɏh����؂�ւ���
			cv::putText(background, mark[k0], cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(255, 255, 255), 5, CV_AA); // �Â��L��������
			cv::putText(background, mark[k1], cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0, 0, 0), 3, CV_AA);     // �V�����L����`��
			cv::imshow(window_info, background);	// �L���\���X�V
			cap >> gframe;                          // �X�V���_�̖ڗ̈�f���̎擾
			cv::waitKey(3 * 1000);                  // 3�b�ҋ@
		}

		//10�b�ԃL�[���͂��Ȃ������ꍇ�́A���s�𖳌��Ƃ���B
		if (result == -1) {
			fout << tr << "," << -1 << endl; // �����Ȏ��s�Ƃ��ċL�^����
			cv::destroyWindow(window_info);  // window�̔j��
			continue;
		}

		// �ڗ̈�摜�L�^
		std::ostringstream ostr1;
		ostr1 << "_tr" << tr;
		eyename = "";
		eyename = "eye_����3_���K_" + subnm + ostr1.str() + ".bmp";
		cv::imwrite(eyename, gframe);   // BL���̉摜���L�^

		cv::destroyWindow(window_info); // window�̔j��


										// �񓚓��́i�R���\�[���Łj
		cout << endl;
		cout << "���s�ԍ�:" << tr + 1 << "/" << TR_NUM / 4 << endl << endl;
		cout << "------------------------------------------" << endl;
		cout << "�ǂ��̈ʒu���ω������������Ă�������" << endl;
		cout << "1:����i�A2:������i�A3:�E��i" << endl;
		cout << "4:�����i�A�@�@�@�@�@�@5:�E���i" << endl;
		cout << "6:�����i�A7:�������i�A8:�E���i" << endl;
		cout << endl;
		cout << "9:�킩��Ȃ�" << endl;
		cin >> answer;

		fout << tr << "," << answer << endl;
	}//for(tr=
	fout.close();
	return 1;
}












//2016.2.7�` for SSII2016
/*-------------------------------*/
/* �h���ω����o����2             */
/* CB���ۂ𔺂���Ԃł̌��o����  */
/*-------------------------------*/
//
//  SSII2016�p
//�@BL�𔺂����o����(CB���ۂȂ��j
//  �����SW���͂Ɠ����ɐ��Ӑ���BL���s��
//  SW���͂���100ms��̎��_�Ő؂�ւ���
//�@�؂�ւ����_�̖ڗ̈�f�����L�^���Ă���

int cb_exp2_ssii()
{
	static int st[TR_NUM][8]; // �e���s�ɂ�����8�ӏ��̎��ӈʒu�ɒ񎦂���h���ԍ�(0,1,2,3)�@
	int chpos[TR_NUM];        // �e���s�ɂ����Ďh�����ω�������ӈʒu(0�F�����,1,�E�E�E,7�F�E����)
	int prst[TR_NUM];         // �ω��O�̎h���ԍ�
	int chst[TR_NUM];         // �ω����ɒ񎦂���h���ԍ�

	int tr;
	int basei, basej, dpix;
	int px, py;
	int i, j, k;
	int k0, k1;
	int answer;
	int itmp1, itmp2, itmp3, itmp4;
	double dtmp;
	char split;

	std::string subnm; // �����Q���Җ�
	std::string sbuf, sbuf1, sbuf2, sbuf3, sbuf4;  // �o�b�t�@
	cv::Mat frame, gray;


	cout << "---�h���ω����o����2---" << endl;

	cout << "�����Q����=";
	cin >> subnm;

	// �h���񎦏����f�[�^�ǂݍ���
	std::string sfname;
	sfname = "order_";
	sfname += "����2_" + subnm + ".csv"; // �t�@�C�����̐���

									   // �h���񎦏����f�[�^�t�@�C���̓ǂݍ���
	ifstream fin(sfname);
	if (!fin) {
		cout << "�h���񎦗p�t�@�C��������܂���" << endl;
		return 0;
	}

	// �ω����̓ǂݎ��
	fin >> sbuf; // 1�s���ǂݔ�΂�

				 // ���s�ԍ��A�ω��ʒu�A�ω��O�h���A�ω���h��
	for (tr = 0; tr < TR_NUM; tr++) {
		fin >> itmp1 >> split >> itmp2 >> split >> itmp3 >> split >> itmp4;
		chpos[tr] = itmp2;
		prst[tr] = itmp3;
		chst[tr] = itmp4;
		//cout << tr+1 << ","<< chpos[tr] << "," << prst[tr] << "," << chst[tr]<< endl;//�m�F
	}

	// �񎦃p�^�[���i���ӂW�ӏ��j
	fin >> sbuf; // 1�s���ǂݔ�΂�

	for (tr = 0; tr<TR_NUM; tr++) {
		fin >> itmp1;// ���s�ԍ��ǂݔ�΂�
		for (i = 0; i < 8; i++) {
			fin >> split >> itmp1;
			st[tr][i] = itmp1;
		}
		fin >> split;
	}
	fin.close();

	// �񎦏��̓ǂݍ��݌��ʂ̊m�F
	cout << "���s�ԍ�,�ω��ʒu,���ݎh���ԍ�,�ω���h���ԍ�" << endl;
	for (tr = 0; tr < TR_NUM; tr++) {
		cout << tr + 1 << ":" << chpos[tr] << "," << prst[tr] << "," << chst[tr] << endl;
	}
	cout << "���s�ԍ�,�񎦎h���i�e���ӈʒu)" << endl;
	for (tr = 0; tr < TR_NUM; tr++) {
		cout << tr + 1 << ":";

		for (i = 0; i < 8; i++) {
			cout << st[tr][i] << ",";
		}
		cout << endl;
	}

	cout << "�������n�߂܂����F�����L�[�������Ă�������";
	getchar();
	getchar();


	// ���s�J�n����

	// �񓚏��L�^����
	std::string fname, imname, eyename;
	std::ostringstream ostr;
	fname = "info_";
	fname += "����2_" + subnm + ".csv"; // �t�@�C�����̐���

									  // �񓚏��o�͗p�t�@�C���̐���
	ofstream fout(fname); // �o�̓t�@�C�����쐬����
	if (!fout) {
		cout << "�t�@�C�����J���܂���" << endl;
		return 0;
	}
	fout << "���s�ԍ�,�ω��ʒu" << endl;

	char *window_info = "info";
	//cv::namedWindow(window_info, CV_WINDOW_AUTOSIZE);							      �@// �h���񎦗pwindow�̐���
	//cv::moveWindow(window_info, (DH_SIZE - SW_HSIZE) / 2, (DV_SIZE - SW_VSIZE) / 2);  // ��ʒ����ֈړ�
	cv::Mat background(cv::Size(DH_SIZE, DV_SIZE), CV_8UC3, cv::Scalar(255, 255, 255)); // Window�ɕ\������w�i�̏���

																						//�񎦎h���̏���
	char mark[][10] = { "X", "O", "H", "E" }; // �P�o�C�g������I��


											  //�����񎦈ʒu�̒��S�ʒu�i�����_�ʒu�j
	basei = DH_SIZE / 2;
	basej = DV_SIZE / 2;

	// �h���z�u�̎Z�o
	dtmp = OBDIST*tan(INTDEG*PAI / 180.0);
	dtmp /= PITCH;
	dpix = (int)dtmp;

	// �����J�n!
	for (tr = 0; tr < TR_NUM; tr++) { // TR_NUM��������{
		cv::Mat background(cv::Size(DH_SIZE, DV_SIZE), CV_8UC3, cv::Scalar(255, 255, 255)); // Window�ɕ\������w�i�̏���
		cv::imshow(window_info, background);  // �摜�\��
		cv::waitKey(3 * 1000);                // �w�i��3�b�Ԓ�
											  // �񎦎h���̏���

		int pcnt = 0;
		for (j = -1; j <= 1; j++) {
			for (i = -1; i <= 1; i++) {
				if (i == 0 && j == 0) continue; // ��ʒ����͕ʂ̋L����񎦂���̂ŃX�L�b�v
				px = basei + i*dpix; // ����̒񎦈ʒu
				py = basej + j*dpix; // ����̒񎦈ʒu
				k = st[tr][pcnt];    // �񎦈ʒupcnt�֒񎦂���h���ԍ��̐ݒ�
				cv::putText(background, (const char*)mark[k], cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0, 0, 0), 3, CV_AA);
				pcnt++; // ���̒񎦈ʒu(0,1,2,�E�E�E,7)
			}//for(i=
		}//for(j=

		 // ��ʒ����ւ̋L���i�����X�񎦁j
		px = basei; // �񎦈ʒu(���j
		py = basej; // �񎦈ʒu(�c)
		cv::putText(background, "+", cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(128, 128, 128), 3, CV_AA);

		// �h����
		//startime = (double)cvGetTickCount();  // �J�n�����̎擾
		cv::imshow(window_info, background);    // �摜�\��
												//cv::waitKey(30);                      // �h���摜�̒�

												//�h���񎦉�ʂ̕ω�����
		k0 = prst[tr]; // �ω��O�̎h���ԍ�
		k1 = chst[tr]; // �ω���̎h���ԍ�
		get_ij(chpos[tr], &i, &j); // �ω��ʒu��i,j�ԍ��̎擾
		px = basei + i*dpix;       // �ω��ʒu�̐ݒ�
		py = basej + j*dpix;       // �ω��ʒu�̐ݒ�

		int result;
		result = cv::waitKey(10 * 1000); // 10�b�ԂȂ��ŃL�[���͂�҂�
		if (result != -1) {     // 10�b�Ԃŉ����L�[�����͂��ꂽ��
			cv::waitKey(100); // 100ms�ҋ@���A�ق�������Ԃ̂Ƃ��Ɏh����؂�ւ���
			cv::putText(background, mark[k0], cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(255, 255, 255), 5, CV_AA); // �Â��L��������
			cv::putText(background, mark[k1], cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0, 0, 0), 3, CV_AA);     // �V�����L����`��
			cv::imshow(window_info, background);	// �L���\���X�V
			cap >> gframe;                          // �X�V���_�̖ڗ̈�f���̎擾
			cv::waitKey(3 * 1000);                  // 3�b�ҋ@
		}

		//10�b�ԃL�[���͂��Ȃ������ꍇ�́A���s�𖳌��Ƃ���B
		if (result == -1) {
			fout << tr << "," << -1 << endl; // �����Ȏ��s�Ƃ��ċL�^����
			cv::destroyWindow(window_info);  // window�̔j��
			continue;
		}

		// �ڗ̈�摜�L�^
		std::ostringstream ostr1;
		ostr1 << "_tr" << tr;
		eyename = "";
		eyename = "eye_����2_" + subnm + ostr1.str() + ".bmp";
		cv::imwrite(eyename, gframe);   // BL���̉摜���L�^

		cv::destroyWindow(window_info); // window�̔j��


										// �񓚓��́i�R���\�[���Łj
		cout << endl;
		cout << "���s�ԍ�:" << tr + 1 << "/" << TR_NUM << endl << endl;
		cout << "------------------------------------------" << endl;
		cout << "�ǂ��̈ʒu���ω������������Ă�������" << endl;
		cout << "1:����i�A2:������i�A3:�E��i" << endl;
		cout << "4:�����i�A�@�@�@�@�@�@5:�E���i" << endl;
		cout << "6:�����i�A7:�������i�A8:�E���i" << endl;
		cout << endl;
		cout << "9:�킩��Ȃ�" << endl;
		cin >> answer;

		fout << tr << "," << answer << endl;
	}//for(tr=
	fout.close();
	return 1;
}



//2016.2.7�` for SSII2016
/*-------------------------------*/
/* �h���ω����o����2(���K�j      */
/* CB���ۂ𔺂���Ԃł̌��o����  */
/*-------------------------------*/
//
//  ���K���[�h
//
//  SSII2016�p
//�@BL�𔺂����o����(CB���ۂȂ��j
//  �����SW���͂Ɠ����ɐ��Ӑ���BL���s��
//  SW���͂���100ms��̎��_�Ő؂�ւ���
//�@�؂�ւ����_�̖ڗ̈�f�����L�^���Ă���

int cb_exp2_ssii_ren()
{
	static int st[TR_NUM][8]; // �e���s�ɂ�����8�ӏ��̎��ӈʒu�ɒ񎦂���h���ԍ�(0,1,2,3)�@
	int chpos[TR_NUM];        // �e���s�ɂ����Ďh�����ω�������ӈʒu(0�F�����,1,�E�E�E,7�F�E����)
	int prst[TR_NUM];         // �ω��O�̎h���ԍ�
	int chst[TR_NUM];         // �ω����ɒ񎦂���h���ԍ�

	int tr;
	int basei, basej, dpix;
	int px, py;
	int i, j, k;
	int k0, k1;
	int answer;
	int itmp1, itmp2, itmp3, itmp4;
	double dtmp;
	char split;

	std::string subnm; // �����Q���Җ�
	std::string sbuf, sbuf1, sbuf2, sbuf3, sbuf4;  // �o�b�t�@
	cv::Mat frame, gray;


	cout << "---�h���ω����o����2---" << endl;

	cout << "�����Q����=";
	cin >> subnm;

	// �h���񎦏����f�[�^�ǂݍ���
	std::string sfname;
	sfname = "order_";
	sfname += "����2_���K_" + subnm + ".csv"; // �t�@�C�����̐���

										  // �h���񎦏����f�[�^�t�@�C���̓ǂݍ���
	ifstream fin(sfname);
	if (!fin) {
		cout << "�h���񎦗p�t�@�C��������܂���" << endl;
		return 0;
	}

	// �ω����̓ǂݎ��
	fin >> sbuf; // 1�s���ǂݔ�΂�

				 // ���s�ԍ��A�ω��ʒu�A�ω��O�h���A�ω���h��
	for (tr = 0; tr < TR_NUM; tr++) { // 
		fin >> itmp1 >> split >> itmp2 >> split >> itmp3 >> split >> itmp4;
		chpos[tr] = itmp2;
		prst[tr] = itmp3;
		chst[tr] = itmp4;
		//cout << tr+1 << ","<< chpos[tr] << "," << prst[tr] << "," << chst[tr]<< endl;//�m�F
	}

	// �񎦃p�^�[���i���ӂW�ӏ��j
	fin >> sbuf; // 1�s���ǂݔ�΂�

	for (tr = 0; tr<TR_NUM; tr++) {
		fin >> itmp1;// ���s�ԍ��ǂݔ�΂�
		for (i = 0; i < 8; i++) {
			fin >> split >> itmp1;
			st[tr][i] = itmp1;
		}
		fin >> split;
	}
	fin.close();

	// �񎦏��̓ǂݍ��݌��ʂ̊m�F
	cout << "���s�ԍ�,�ω��ʒu,���ݎh���ԍ�,�ω���h���ԍ�" << endl;
	for (tr = 0; tr < TR_NUM; tr++) {
		cout << tr + 1 << ":" << chpos[tr] << "," << prst[tr] << "," << chst[tr] << endl;
	}
	cout << "���s�ԍ�,�񎦎h���i�e���ӈʒu)" << endl;
	for (tr = 0; tr < TR_NUM; tr++) {
		cout << tr + 1 << ":";

		for (i = 0; i < 8; i++) {
			cout << st[tr][i] << ",";
		}
		cout << endl;
	}

	cout << "���K���n�߂܂����F�����L�[�������Ă�������";
	getchar();
	getchar();


	// ���s�J�n����

	// �񓚏��L�^����
	std::string fname, imname, eyename;
	std::ostringstream ostr;
	fname = "info_";
	fname += "����2_���K_" + subnm + ".csv"; // �t�@�C�����̐���

										 // �񓚏��o�͗p�t�@�C���̐���
	ofstream fout(fname); // �o�̓t�@�C�����쐬����
	if (!fout) {
		cout << "�t�@�C�����J���܂���" << endl;
		return 0;
	}
	fout << "���s�ԍ�,�ω��ʒu" << endl;

	char *window_info = "info";
	//cv::namedWindow(window_info, CV_WINDOW_AUTOSIZE);							      �@// �h���񎦗pwindow�̐���
	//cv::moveWindow(window_info, (DH_SIZE - SW_HSIZE) / 2, (DV_SIZE - SW_VSIZE) / 2);  // ��ʒ����ֈړ�
	cv::Mat background(cv::Size(DH_SIZE, DV_SIZE), CV_8UC3, cv::Scalar(255, 255, 255)); // Window�ɕ\������w�i�̏���

																						//�񎦎h���̏���
	char mark[][10] = { "X", "O", "H", "E" }; // �P�o�C�g������I��


											  //�����񎦈ʒu�̒��S�ʒu�i�����_�ʒu�j
	basei = DH_SIZE / 2;
	basej = DV_SIZE / 2;

	// �h���z�u�̎Z�o
	dtmp = OBDIST*tan(INTDEG*PAI / 180.0);
	dtmp /= PITCH;
	dpix = (int)dtmp;

	// �����J�n!
	for (tr = 0; tr < TR_NUM / 4; tr++) { // TR_NUM/4��������{(�{������1/4)
		cv::Mat background(cv::Size(DH_SIZE, DV_SIZE), CV_8UC3, cv::Scalar(255, 255, 255)); // Window�ɕ\������w�i�̏���
		cv::imshow(window_info, background);  // �摜�\��
		cv::waitKey(3 * 1000);                // �w�i��3�b�Ԓ�
											  // �񎦎h���̏���

		int pcnt = 0;
		for (j = -1; j <= 1; j++) {
			for (i = -1; i <= 1; i++) {
				if (i == 0 && j == 0) continue; // ��ʒ����͕ʂ̋L����񎦂���̂ŃX�L�b�v
				px = basei + i*dpix; // ����̒񎦈ʒu
				py = basej + j*dpix; // ����̒񎦈ʒu
				k = st[tr][pcnt];    // �񎦈ʒupcnt�֒񎦂���h���ԍ��̐ݒ�
				cv::putText(background, (const char*)mark[k], cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0, 0, 0), 3, CV_AA);
				pcnt++; // ���̒񎦈ʒu(0,1,2,�E�E�E,7)
			}//for(i=
		}//for(j=

		 // ��ʒ����ւ̋L���i�����X�񎦁j
		px = basei; // �񎦈ʒu(���j
		py = basej; // �񎦈ʒu(�c)
		cv::putText(background, "+", cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(128, 128, 128), 3, CV_AA);

		// �h����
		//startime = (double)cvGetTickCount();  // �J�n�����̎擾
		cv::imshow(window_info, background);    // �摜�\��
												//cv::waitKey(30);                      // �h���摜�̒�

												//�h���񎦉�ʂ̕ω�����
		k0 = prst[tr]; // �ω��O�̎h���ԍ�
		k1 = chst[tr]; // �ω���̎h���ԍ�
		get_ij(chpos[tr], &i, &j); // �ω��ʒu��i,j�ԍ��̎擾
		px = basei + i*dpix;       // �ω��ʒu�̐ݒ�
		py = basej + j*dpix;       // �ω��ʒu�̐ݒ�

		int result;
		result = cv::waitKey(10 * 1000); // 10�b�ԂȂ��ŃL�[���͂�҂�
		if (result != -1) {     // 10�b�Ԃŉ����L�[�����͂��ꂽ��
			cv::waitKey(100); // 100ms�ҋ@���A�ق�������Ԃ̂Ƃ��Ɏh����؂�ւ���
			cv::putText(background, mark[k0], cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(255, 255, 255), 5, CV_AA); // �Â��L��������
			cv::putText(background, mark[k1], cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0, 0, 0), 3, CV_AA);     // �V�����L����`��
			cv::imshow(window_info, background);	// �L���\���X�V
			cap >> gframe;                          // �X�V���_�̖ڗ̈�f���̎擾
			cv::waitKey(3 * 1000);                  // 3�b�ҋ@
		}

		//10�b�ԃL�[���͂��Ȃ������ꍇ�́A���s�𖳌��Ƃ���B
		if (result == -1) {
			fout << tr << "," << -1 << endl; // �����Ȏ��s�Ƃ��ċL�^����
			cv::destroyWindow(window_info);  // window�̔j��
			continue;
		}

		// �ڗ̈�摜�L�^
		std::ostringstream ostr1;
		ostr1 << "_tr" << tr;
		eyename = "";
		eyename = "eye_����2_���K_" + subnm + ostr1.str() + ".bmp";
		cv::imwrite(eyename, gframe);   // BL���̉摜���L�^

		cv::destroyWindow(window_info); // window�̔j��


										// �񓚓��́i�R���\�[���Łj
		cout << endl;
		cout << "���s�ԍ�:" << tr + 1 << "/" << TR_NUM / 4 << endl << endl;
		cout << "------------------------------------------" << endl;
		cout << "�ǂ��̈ʒu���ω������������Ă�������" << endl;
		cout << "1:����i�A2:������i�A3:�E��i" << endl;
		cout << "4:�����i�A�@�@�@�@�@�@5:�E���i" << endl;
		cout << "6:�����i�A7:�������i�A8:�E���i" << endl;
		cout << endl;
		cout << "9:�킩��Ȃ�" << endl;
		cin >> answer;

		fout << tr << "," << answer << endl;
	}//for(tr=
	fout.close();
	return 1;
}









//2016.2.6�` for SSII2016
/*-------------------------------*/
/* �h���ω����o�����P            */
/* CB���ۂ̂Ȃ���Ԃł̌��o����  */
/*-------------------------------*/
//
//  SSII2016�p
//�@BL�Ȃ��ł̌��o����(CB���ۂȂ��j
//  �^�����m����
//
int cb_exp1_ssii()
{
	static int st[TR_NUM][8]; // �e���s�ɂ�����8�ӏ��̎��ӈʒu�ɒ񎦂���h���ԍ�(0,1,2,3)�@
	int chpos[TR_NUM];        // �e���s�ɂ����Ďh�����ω�������ӈʒu(0�F�����,1,�E�E�E,7�F�E����)
	int prst[TR_NUM];         // �ω��O�̎h���ԍ�
	int chst[TR_NUM];         // �ω����ɒ񎦂���h���ԍ�

	int tr;
	int basei, basej, dpix;
	int px, py;
	int i, j, k;
	int k0, k1;
	int answer;
	int itmp1, itmp2, itmp3, itmp4;
	double dtmp;
	char split;

	std::string subnm; // �����Q���Җ�
	std::string sbuf, sbuf1, sbuf2, sbuf3, sbuf4;  // �o�b�t�@
	cv::Mat frame, gray;


	cout << "---�h���ω����o�����P---" << endl;

	cout << "�����Q����=";
	cin >> subnm;

	// �h���񎦏����f�[�^�ǂݍ���
	std::string sfname;
	sfname = "order_";
	sfname += "����1_" + subnm + ".csv"; // �t�@�C�����̐���

									   // �h���񎦏����f�[�^�t�@�C���̓ǂݍ���
	ifstream fin(sfname);
	if (!fin) {
		cout << "�h���񎦗p�t�@�C��������܂���" << endl;
		return 0;
	}

	// �ω����̓ǂݎ��
	fin >> sbuf; // 1�s���ǂݔ�΂�

				 // ���s�ԍ��A�ω��ʒu�A�ω��O�h���A�ω���h��
	for (tr = 0; tr < TR_NUM; tr++) {
		fin >> itmp1 >> split >> itmp2 >> split >> itmp3 >> split >> itmp4;
		chpos[tr] = itmp2;
		prst[tr] = itmp3;
		chst[tr] = itmp4;
		//cout << tr+1 << ","<< chpos[tr] << "," << prst[tr] << "," << chst[tr]<< endl;//�m�F
	}

	// �񎦃p�^�[���i���ӂW�ӏ��j
	fin >> sbuf; // 1�s���ǂݔ�΂�

	for (tr = 0; tr<TR_NUM; tr++) {
		fin >> itmp1;// ���s�ԍ��ǂݔ�΂�
		for (i = 0; i < 8; i++) {
			fin >> split >> itmp1;
			st[tr][i] = itmp1;
		}
		fin >> split;
	}
	fin.close();

	// �񎦏��̓ǂݍ��݌��ʂ̊m�F
	cout << "���s�ԍ�,�ω��ʒu,���ݎh���ԍ�,�ω���h���ԍ�" << endl;
	for (tr = 0; tr < TR_NUM; tr++) {
		cout << tr + 1 << ":" << chpos[tr] << "," << prst[tr] << "," << chst[tr] << endl;
	}
	cout << "���s�ԍ�,�񎦎h���i�e���ӈʒu)" << endl;
	for (tr = 0; tr < TR_NUM; tr++) {
		cout << tr + 1 << ":";
		for (i = 0; i < 8; i++) {
			cout << st[tr][i] << ",";
		}
		cout << endl;
	}

	cout << "�������n�߂܂����F�����L�[�������Ă�������";
	getchar();


	// ���s�J�n����

	// �񓚏��L�^����
	std::string fname, imname;
	std::ostringstream ostr;
	fname = "info_";
	fname += "����1_" + subnm + ".csv"; // �t�@�C�����̐���

									  // �񓚏��o�͗p�t�@�C���̐���
	ofstream fout(fname); // �o�̓t�@�C�����쐬����
	if (!fout) {
		cout << "�t�@�C�����J���܂���" << endl;
		return 0;
	}
	fout << "���s�ԍ�,�ω��ʒu" << endl;

	char *window_info = "info";
	//cv::namedWindow(window_info, CV_WINDOW_AUTOSIZE);							      �@// �h���񎦗pwindow�̐���
	//cv::moveWindow(window_info, (DH_SIZE - SW_HSIZE) / 2, (DV_SIZE - SW_VSIZE) / 2);  // ��ʒ����ֈړ�
	cv::Mat background(cv::Size(DH_SIZE, DV_SIZE), CV_8UC3, cv::Scalar(255, 255, 255)); // Window�ɕ\������w�i�̏���

																						//�񎦎h���̏���
	char mark[][10] = { "X", "O", "H", "E" }; // �P�o�C�g������I��


											  //�����񎦈ʒu�̒��S�ʒu�i�����_�ʒu�j
	basei = DH_SIZE / 2;
	basej = DV_SIZE / 2;

	// �h���z�u�̎Z�o
	dtmp = OBDIST*tan(INTDEG*PAI / 180.0);
	dtmp /= PITCH;
	dpix = (int)dtmp;

	// �����J�n!
	for (tr = 0; tr < TR_NUM; tr++) { // TR_NUM��������{
		cv::Mat background(cv::Size(DH_SIZE, DV_SIZE), CV_8UC3, cv::Scalar(255, 255, 255)); // Window�ɕ\������w�i�̏���
		cv::imshow(window_info, background);  // �摜�\��
		cv::waitKey(3 * 1000);                  // �w�i��3�b�Ԓ�
												// �񎦎h���̏���

		int pcnt = 0;
		for (j = -1; j <= 1; j++) {
			for (i = -1; i <= 1; i++) {
				if (i == 0 && j == 0) continue; // ��ʒ����͕ʂ̋L����񎦂���̂ŃX�L�b�v
				px = basei + i*dpix; // ����̒񎦈ʒu
				py = basej + j*dpix; // ����̒񎦈ʒu
				k = st[tr][pcnt];    // �񎦈ʒupcnt�֒񎦂���h���ԍ��̐ݒ�
				cv::putText(background, (const char*)mark[k], cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0, 0, 0), 3, CV_AA);
				pcnt++; // ���̒񎦈ʒu(0,1,2,�E�E�E,7)
			}//for(i=
		}//for(j=

		 // ��ʒ����ւ̋L���i�����X�񎦁j
		px = basei; // �񎦈ʒu(���j
		py = basej; // �񎦈ʒu(�c)
		cv::putText(background, "+", cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(128, 128, 128), 3, CV_AA);

		// �h����
		startime = (double)cvGetTickCount();  // �J�n�����̎擾
		cv::imshow(window_info, background);  // �摜�\��
		cv::waitKey(3 * 1000);                  // �h���摜��3�b�Ԓ�

												//�h���񎦉�ʂ̕ω�
		k0 = prst[tr]; // �ω��O�̎h���ԍ�
		k1 = chst[tr]; // �ω���̎h���ԍ�
		get_ij(chpos[tr], &i, &j); // �ω��ʒu��i,j�ԍ��̎擾
		px = basei + i*dpix;       // �ω��ʒu�̐ݒ�
		py = basej + j*dpix;       // �ω��ʒu�̐ݒ�

		cv::putText(background, mark[k0], cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(255, 255, 255), 5, CV_AA); // �Â��L��������
		cv::putText(background, mark[k1], cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0, 0, 0), 3, CV_AA);       // �V�����L����`��
		cv::imshow(window_info, background);	// �L���\���X�V
		cv::waitKey(3 * 1000);                  // 3�b�҂�
		cv::destroyWindow(window_info);        // window�̔j��


											   // �񓚓��́i�R���\�[���Łj
		cout << endl;
		cout << "���s�ԍ�:" << tr + 1 << "/" << TR_NUM << endl << endl;
		cout << "------------------------------------------" << endl;
		cout << "�ǂ��̈ʒu���ω������������Ă�������" << endl;
		cout << "1:����i�A2:������i�A3:�E��i" << endl;
		cout << "4:�����i�A�@�@�@�@�@�@5:�E���i" << endl;
		cout << "6:�����i�A7:�������i�A8:�E���i" << endl;
		cout << endl;
		cout << "9:�킩��Ȃ�" << endl;
		cin >> answer;

		fout << tr << "," << answer << endl;
	}//for(tr=
	fout.close();
	return 1;
}



//2016.2.6�` for SSII2016
/*-------------------------------*/
/* �h���ω����o�����P(���K�j     */
/* CB���ۂ̂Ȃ���Ԃł̌��o����  */
/*-------------------------------*/
//
// ���K
//
//  SSII2016�p
//�@BL�Ȃ��ł̌��o����(CB���ۂȂ��j
//  �^�����m����
//
int cb_exp1_ssii_ren()
{
	static int st[TR_NUM][8]; // �e���s�ɂ�����8�ӏ��̎��ӈʒu�ɒ񎦂���h���ԍ�(0,1,2,3)�@
	int chpos[TR_NUM];        // �e���s�ɂ����Ďh�����ω�������ӈʒu(0�F�����,1,�E�E�E,7�F�E����)
	int prst[TR_NUM];         // �ω��O�̎h���ԍ�
	int chst[TR_NUM];         // �ω����ɒ񎦂���h���ԍ�

	int tr;
	int basei, basej, dpix;
	int px, py;
	int i, j, k;
	int k0, k1;
	int answer;
	int itmp1, itmp2, itmp3, itmp4;
	double dtmp;
	char split;

	std::string subnm; // �����Q���Җ�
	std::string sbuf, sbuf1, sbuf2, sbuf3, sbuf4;  // �o�b�t�@
	cv::Mat frame, gray;


	cout << "---�h���ω����o�����P---" << endl;

	cout << "�����Q����=";
	cin >> subnm;

	// �h���񎦏����f�[�^�ǂݍ���
	std::string sfname;
	sfname = "order_";
	sfname += "����1_���K_" + subnm + ".csv"; // �t�@�C�����̐���

										  // �h���񎦏����f�[�^�t�@�C���̓ǂݍ���
	ifstream fin(sfname);
	if (!fin) {
		cout << "�h���񎦗p�t�@�C��������܂���" << endl;
		return 0;
	}

	// �ω����̓ǂݎ��
	fin >> sbuf; // 1�s���ǂݔ�΂�

				 // ���s�ԍ��A�ω��ʒu�A�ω��O�h���A�ω���h��
	for (tr = 0; tr < TR_NUM; tr++) {
		fin >> itmp1 >> split >> itmp2 >> split >> itmp3 >> split >> itmp4;
		chpos[tr] = itmp2;
		prst[tr] = itmp3;
		chst[tr] = itmp4;
		//cout << tr+1 << ","<< chpos[tr] << "," << prst[tr] << "," << chst[tr]<< endl;//�m�F
	}

	// �񎦃p�^�[���i���ӂW�ӏ��j
	fin >> sbuf; // 1�s���ǂݔ�΂�

	for (tr = 0; tr<TR_NUM; tr++) {
		fin >> itmp1;// ���s�ԍ��ǂݔ�΂�
		for (i = 0; i < 8; i++) {
			fin >> split >> itmp1;
			st[tr][i] = itmp1;
		}
		fin >> split;
	}
	fin.close();

	// �񎦏��̓ǂݍ��݌��ʂ̊m�F
	cout << "���s�ԍ�,�ω��ʒu,���ݎh���ԍ�,�ω���h���ԍ�" << endl;
	for (tr = 0; tr < TR_NUM; tr++) {
		cout << tr + 1 << ":" << chpos[tr] << "," << prst[tr] << "," << chst[tr] << endl;
	}
	cout << "���s�ԍ�,�񎦎h���i�e���ӈʒu)" << endl;
	for (tr = 0; tr < TR_NUM; tr++) {
		cout << tr + 1 << ":";
		for (i = 0; i < 8; i++) {
			cout << st[tr][i] << ",";
		}
		cout << endl;
	}

	cout << "���K���n�߂܂����F�����L�[�������Ă�������";
	getchar();


	// ���s�J�n����

	// �񓚏��L�^����
	std::string fname, imname;
	std::ostringstream ostr;
	fname = "info_";
	fname += "����1_���K_" + subnm + ".csv"; // �t�@�C�����̐���

										 // �񓚏��o�͗p�t�@�C���̐���
	ofstream fout(fname); // �o�̓t�@�C�����쐬����
	if (!fout) {
		cout << "�t�@�C�����J���܂���" << endl;
		return 0;
	}
	fout << "���s�ԍ�,�ω��ʒu" << endl;

	char *window_info = "info";
	//cv::namedWindow(window_info, CV_WINDOW_AUTOSIZE);							      �@// �h���񎦗pwindow�̐���
	//cv::moveWindow(window_info, (DH_SIZE - SW_HSIZE) / 2, (DV_SIZE - SW_VSIZE) / 2);  // ��ʒ����ֈړ�
	cv::Mat background(cv::Size(DH_SIZE, DV_SIZE), CV_8UC3, cv::Scalar(255, 255, 255)); // Window�ɕ\������w�i�̏���

																						//�񎦎h���̏���
	char mark[][10] = { "X", "O", "H", "E" }; // �P�o�C�g������I��


											  //�����񎦈ʒu�̒��S�ʒu�i�����_�ʒu�j
	basei = DH_SIZE / 2;
	basej = DV_SIZE / 2;

	// �h���z�u�̎Z�o
	dtmp = OBDIST*tan(INTDEG*PAI / 180.0);
	dtmp /= PITCH;
	dpix = (int)dtmp;

	// �����J�n!
	for (tr = 0; tr < TR_NUM / 9; tr++) { // TR_NUM/9��������{
		cv::Mat background(cv::Size(DH_SIZE, DV_SIZE), CV_8UC3, cv::Scalar(255, 255, 255)); // Window�ɕ\������w�i�̏���
		cv::imshow(window_info, background);  // �摜�\��
		cv::waitKey(3 * 1000);                  // �w�i��3�b�Ԓ�
												// �񎦎h���̏���

		int pcnt = 0;
		for (j = -1; j <= 1; j++) {
			for (i = -1; i <= 1; i++) {
				if (i == 0 && j == 0) continue; // ��ʒ����͕ʂ̋L����񎦂���̂ŃX�L�b�v
				px = basei + i*dpix; // ����̒񎦈ʒu
				py = basej + j*dpix; // ����̒񎦈ʒu
				k = st[tr][pcnt];    // �񎦈ʒupcnt�֒񎦂���h���ԍ��̐ݒ�
				cv::putText(background, (const char*)mark[k], cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0, 0, 0), 3, CV_AA);
				pcnt++; // ���̒񎦈ʒu(0,1,2,�E�E�E,7)
			}//for(i=
		}//for(j=

		 // ��ʒ����ւ̋L���i�����X�񎦁j
		px = basei; // �񎦈ʒu(���j
		py = basej; // �񎦈ʒu(�c)
		cv::putText(background, "+", cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(128, 128, 128), 3, CV_AA);

		// �h����
		startime = (double)cvGetTickCount();  // �J�n�����̎擾
		cv::imshow(window_info, background);  // �摜�\��
		cv::waitKey(3 * 1000);                  // �h���摜��3�b�Ԓ�

												//�h���񎦉�ʂ̕ω�
		k0 = prst[tr]; // �ω��O�̎h���ԍ�
		k1 = chst[tr]; // �ω���̎h���ԍ�
		get_ij(chpos[tr], &i, &j); // �ω��ʒu��i,j�ԍ��̎擾
		px = basei + i*dpix;       // �ω��ʒu�̐ݒ�
		py = basej + j*dpix;       // �ω��ʒu�̐ݒ�

		cv::putText(background, mark[k0], cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(255, 255, 255), 5, CV_AA); // �Â��L��������
		cv::putText(background, mark[k1], cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0, 0, 0), 3, CV_AA);       // �V�����L����`��
		cv::imshow(window_info, background);	// �L���\���X�V
		cv::waitKey(3 * 1000);                  // 3�b�҂�
		cv::destroyWindow(window_info);        // window�̔j��


											   // �񓚓��́i�R���\�[���Łj
		cout << endl;
		cout << "���s�ԍ�:" << tr + 1 << "/" << TR_NUM / 9 << endl << endl;
		cout << "------------------------------------------" << endl;
		cout << "�ǂ��̈ʒu���ω������������Ă�������" << endl;
		cout << "1:����i�A2:������i�A3:�E��i" << endl;
		cout << "4:�����i�A�@�@�@�@�@�@5:�E���i" << endl;
		cout << "6:�����i�A7:�������i�A8:�E���i" << endl;
		cout << endl;
		cout << "9:�킩��Ȃ�" << endl;
		cin >> answer;

		fout << tr << "," << answer << endl;
	}//for(tr=
	fout.close();
	return 1;
}


/*------------------------------------*/
/* ���ӈʒu�ԍ��ɑ΂���i,j�l���擾����*/
/*------------------------------------*/
//�@���ӈʒu:0,1,�E�E�E,7
//
int get_ij(
	int pos,  // �񎦈ʒu�ԍ�(0,1,2,�E�E�E7)
	int *i0,  // �Ή�����i�ԍ�
	int *j0   // �Ή�����j�ԍ�
)
{
	int i, j;
	int cnt = -1;
	for (j = -1; j <= 1; j++) {
		for (i = -1; i <= 1; i++) {
			if (i == 0 && j == 0) continue; // ���S�ʒu�̓J�E���g���Ȃ�
			cnt++;
			if (cnt == pos) {
				*i0 = i;
				*j0 = j;
				return 1;
			}
		}
	}
	cout << "�v�Z�ł��܂���" << endl;
	return 0;
}




//2016.1.11�`
/*----------------*/
/* CB���ۊm�F���� */
/*----------------*/
//
// �h�����������̉���(2016.1.16�ǉ��j
//
int cb_exp()
{

	static int st[8][TR_NUM]; // �e���s�ɂ�����8�ӏ��̎��ӈʒu�ɒ񎦂���h���ԍ�(0,1,2,3)
	int chpos[TR_NUM];        // �e���s�ɂ����Ďh�����ω�������ӈʒu(0,1,�E�E�E,7)
	int chst[TR_NUM];         // �ω����ɒ񎦂���h���ԍ�
	int fch[4][4];            // �ω��p�^�[���̏o���񐔂�c�����邽�߂̏o���L�^�\�i��Ɨp�j

	int tr, fr, fr_bl;
	int basei, basej, dpix;
	int px, py;
	int canny_th1, canny_th2, bl_th;
	int i, j, k, is, js, ie, je;
	int i0, j0, k0, k1;
	int cnt, th;
	int answer;
	double dtmp;

	std::string subnm; // �����Q���Җ�
	std::string sbuf;  // �o�b�t�@
	cv::Mat frame, gray;

	cout << "---CB���ۊm�F����---" << endl;


	//double startime, stoptime; �O���[�o���ϐ��Œ�`���Ă���B

	is = box.x;
	js = box.y;
	ie = box.x + box.width;
	je = box.y + box.height;

	if (is == -1) {
		cout << "���o�͈͂��ݒ肳��Ă��܂���" << endl;
		return 0;
	}

	cout << "is=" << is << "ie=" << ie << endl;
	cout << "js=" << js << "je=" << je << endl;


	cout << "�����Q����=";
	cin >> subnm;

	cout << "canny臒l1(�M�����̍����G�b�W�j=";
	cin >> canny_th1;

	cout << "canny臒l2(�M�����̒Ⴂ�G�b�W�j=";
	cin >> canny_th2;

	cout << "BL���o�̂��߂�臒l=";
	cin >> bl_th;

	/*�@���Ƃō��(2.6)
	// �h���񎦗p�f�[�^�ǂݍ���
	std::string sfname;
	sfname = "order_";
	//sfname = fname + subnm + ".csv"; // �t�@�C�����̐���


	// �h���񎦃f�[�^�̓ǂݍ��ݗp�t�@�C���̐���
	ifstream fin(sfname); // ���̓t�@�C�����쐬����
	if (!fin){
	cout << "�h���񎦗p�t�@�C��������܂���" << endl;
	return 0;
	}
	//sfname >> sbuf; // 1�s�ǂݔ�΂�,�����v����
	for (tr = 0; tr < TR_NUM; tr++){
	//sfname >>
	}

	*/

	//�񎦗pWindow�̏���(SW_HSIZE�~SW_VSIZE��f�̑傫���ɌŒ�j
	char *window_info = "info";
	//cv::namedWindow(window_info, CV_WINDOW_AUTOSIZE);							      // �h���񎦗pwindow�̐���
	//cv::moveWindow(window_info, (DH_SIZE - SW_HSIZE) / 2, (DV_SIZE - SW_VSIZE) / 2);  // ��ʒ����ֈړ�
	cv::Mat background(cv::Size(DH_SIZE, DV_SIZE), CV_8UC3, cv::Scalar(255, 255, 255)); // Window�ɕ\������w�i�̏���

																						//�񎦎h���̏���
	char mark[][10] = { "X", "O", "I", "E" }; // �P�o�C�g������I��
	int mark_num[3][3]; // �񎦋L���̋L���p�z��


						//�����񎦈ʒu�̃x�[�X�ʒu�i�����_�j
	basei = DH_SIZE / 2;
	basej = DV_SIZE / 2;

	// �h���z�u�̎Z�o
	dtmp = OBDIST*tan(INTDEG*PAI / 180.0);
	dtmp /= PITCH;
	dpix = (int)dtmp;
	//cout << dpix;

	// �����J�n!
	//for (tr = 0; tr < TR_NUM; tr++){ // TR_NUM��������{
	for (tr = 0; tr < 1; tr++) {
		// ��ʏ��8�ӏ��ɕ�����񎦂���i�e�����������񎦂����悤�ɉ��ǂ���K�v����:2016.1.11)
		srand((unsigned)time(NULL));
		for (j = -1; j <= 1; j++) {
			for (i = -1; i <= 1; i++) {
				if (i == 0 && j == 0) continue; // ��ʒ����͕ʂ̋L����񎦂���
				px = basei + i*dpix; // ����̒񎦈ʒu
				py = basej + j*dpix; // ����̒񎦈ʒu
				k = 4 * rand() / RAND_MAX;
				if (k <= 3) { // k=4�ȏ�͖����Ȉʒu
					mark_num[i + 1][j + 1] = k; // �e�񎦈ʒu�̋L���̋L��
					cv::putText(background, (const char*)mark[k], cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0, 0, 0), 3, CV_AA);
				}
			}//for(i=
		}//for(j=

		 // ��ʒ����ւ̋L����
		px = basei; // �񎦈ʒu(���j
		py = basej; // �񎦈ʒu(�c)
		cv::putText(background, "+", cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0, 0, 0), 3, CV_AA);

		// �񎦐؂�ւ��ʒu�I��(���Fi0�A�c�Fj0)
		while (1) {
			i0 = 3 * rand() / RAND_MAX;
			j0 = 3 * rand() / RAND_MAX;
			if (i0 == 1 && j0 == 1) continue; // �����I���Ȃ̂Ŗ���
			if (i0 >= 3) continue;            // �����Ȓ񎦈ʒu
			if (j0 >= 3) continue;            // �����Ȓ񎦈ʒu
			k0 = mark_num[i0][j0];            // �؂�ւ��Ώۈʒu�ɒ񎦂���Ă���L���̔ԍ��̎擾
			break;
		}
		// �؂�ւ���L���̑I��(k1)
		while (1) {
			k1 = 4 * rand() / RAND_MAX; //
			if (k1 == k0) continue;     // �؂�ւ���O�Ɠ����L��
			if (k1 == 4) continue;      // �I���ł��Ȃ��L��
			break;
		}

		// �h����
		startime = (double)cvGetTickCount();  // �J�n�����̎擾
		cv::imshow(window_info, background);  // �摜�\��
		cv::waitKey(30);                      // ��ʕ\���̂��߂�30ms�҂�(�L�[���̓C�x���g�փt�H�[�J�X���ړ����Ȃ��Ɖ�ʂ��\������Ȃ��j

											  // �ڗ̈�f���L���v�`���J�n
											  // 5�b�܂ł�BL���N�͖�������
		fr = 0; // �t���[���ԍ��̏�����
		startime = (double)cvGetTickCount(); // �J�n����
		while (1) {
			fr++;
			if (fr >= 30 * 60) { // 1���ȏ�(30fps)�Ȃ�^�C���A�E�g
								 //save_basic_data(edge_sum, fr); // ��{�f�[�^�̏�������
				break;
			}

			cap >> frame; // 1�t���[���ǂݍ���

			cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);	// �J���[�摜�����m�N���摜�ɕϊ�
			cv::Canny(gray, gray, canny_th1, canny_th2, 3); // �L���j�[�I�y���[�^�ɂ��G�b�W���o

															//cv::rectangle(frame, cv::Point(is, js), cv::Point(ie, je), cv::Scalar(0, 0, 255), 1, 1);    // �֐S�̈��`�悷��
															//cv::rectangle(gray, cv::Point(is, js), cv::Point(ie, je), cv::Scalar(255, 255, 255), 1, 1); // �֐S�̈��`�悷��

															//cv::imshow(window_input, frame); // ���摜�̕\��
															//cv::imshow(window_output, gray); // �G�b�W���o�摜

															// �G�b�W�_���̃J�E���g
			cnt = 0;
			for (j = js; j < je; j++) {
				for (i = is; i < ie; i++) {
					if (gray.data[j * H_SIZE + i] == 255)
						cnt++;
				}
			}
			edge_sum[fr] = cnt;

			// BL���菈��
			if (fr < 30 * 5) { // ���s�ŏ���5�b�Ԃ͔��菈�����Ȃ�
				cv::waitKey(15);
				continue;
			}

			th = 0;
			for (i = fr - 1; i > fr - 1 - FR_DUR; i--) {// �ߋ�5�t���[�����̕��ϒl
				th += edge_sum[i]; // �G�b�W�_���̐ώZ
			}
			th /= FR_DUR;
			itmp[fr] = th; // �m�F�p
			itmp2[fr] = edge_sum[fr] - th; // �m�F�p
			if (edge_sum[fr] - th < -bl_th) {		// ����ق̉��~���_�̌��o����
				fr_bl = fr; // BL���o�t���[��:fr_bl
				cout << "BL frame=" << fr_bl;
				//save_basic_data(edge_sum, fr);      // ��{�f�[�^�̊i�[����
				//cv::imwrite("closebl.bmp", frame);  // BL���̉摜���L�^
				break; // �񎦏I��
			}
			stoptime = (double)cvGetTickCount() - startime; // �I������
			if (cv::waitKey(15) >= 0) { // 15ms���ŁA�����L�[�������ꂽ��I��
				break;
			}
		}//while(1)

		 //�h���񎦉�ʂ̕ω�
		if (fr != 30 * 60) { // �^�C���A�E�g�łȂ���Β񎦉�ʂ�ω�������
			px = basei + dpix * (i0 - 1); // ����̒񎦈ʒu
			py = basej + dpix * (j0 - 1); //
			cv::putText(background, mark[k0], cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(255, 255, 255), 5, CV_AA); // �Â��L��������
			cv::putText(background, mark[k1], cv::Point(px, py), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0, 0, 255), 3, CV_AA);     // �V�����L��������(�킩��悤�ɍ��͐ԂŁj
			cv::imshow(window_info, background);	// �L���\���X�V
			cv::waitKey(5 * 1000); // 5s�҂�
		}

		cv::destroyAllWindows(); // ���ׂĂ�window��j��

								 // �񓚂����߂�i�R���\�[���Łj
		cout << endl;
		cout << "------------------------------------------" << endl;
		cout << "�ǂ��̈ʒu���ω������������Ă�������" << endl;
		cout << "1:����i�A2:������i�A3:�E��i" << endl;
		cout << "4:�����i�A�@�@�@�@�@�@5:�E���i" << endl;
		cout << "6:�����i�A7:�������i�A8:�E���i" << endl;
		cout << endl;
		cout << "9:�킩��Ȃ�" << endl;
		cin >> answer;

		// ����̎��s�̏��L�^
		std::string fname, imname;
		std::ostringstream ostr;
		fname = "info_";
		ostr << "_tr" << tr;
		fname = fname + subnm + ostr.str() + ".csv"; // �t�@�C�����̐���
		imname = subnm + ostr.str() + ".bmp";        // BL���摜�i�[�p�t�@�C�����̐���

													 // �摜���L�^����
		cv::imwrite(imname, frame);  // BL���̉摜���L�^

									 // ���o�͗p�t�@�C���̐���
		ofstream fout(fname); // �o�̓t�@�C�����쐬����
		if (!fout) {
			cout << "�t�@�C�����J���܂���" << endl;
			return 0;
		}
		fout << "�񎦕�����" << endl;
		for (j = 0; j <3; j++) {
			for (i = 0; i < 3; i++) {
				if (i == 1 && j == 1) continue;
				k = mark_num[i][j];
				fout << mark[k] << ",";
			}
		}
		fout << endl;

		fout << "�ω��ʒu(��),�ω��ʒu(�c�j,�ω�����" << endl;
		fout << i0 << "," << j0 << "," << mark[k1] << endl;
		fout << "�񓚕ω��ʒu(���j,�񓚕ω��ʒu�i�c�j" << endl;
		if (answer == 1) fout << 0 << "," << 0 << endl;
		if (answer == 2) fout << 1 << "," << 0 << endl;
		if (answer == 3) fout << 2 << "," << 0 << endl;
		if (answer == 4) fout << 0 << "," << 1 << endl;
		if (answer == 5) fout << 2 << "," << 1 << endl;
		if (answer == 6) fout << 0 << "," << 2 << endl;
		if (answer == 7) fout << 1 << "," << 2 << endl;
		if (answer == 8) fout << 2 << "," << 2 << endl;
		if (answer == 9) fout << -1 << "," << -1 << endl;

		fout << "canny�I�y���[�^�̃p�����[�^" << endl;
		fout << "臒l1=" << canny_th1 << endl;
		fout << "臒l2=" << canny_th2 << endl;
		fout << "�G�b�W�_����臒l=" << bl_th << endl;

		fout << "BL���N�t���[���ԍ�=" << fr_bl << endl;

		fout << "--- ��b�f�[�^---" << endl;
		fout << "fr,�G�b�W��,臒l,�����l" << endl;

		for (fr = 1; fr <= fr_bl; fr++) {
			fout << fr << "," << edge_sum[fr] << "," << itmp[fr] << "," << itmp2[fr] << endl;
		}
		fout << "���s���� =";
		fout << stoptime / ((double)cvGetTickFrequency()*1000.) << endl;
		fout.close();

	}//for(tr=

	return 1;
}



//2016.1.16
/*-----------------------------------*/
/* CB�����̏����i�񎦎h���n��̐����j*/
/*  ����3�p�i���ӗU������)           */
/*-----------------------------------*/
//
// ���s�񐔁F8�̔{��(�ω��ʒu,�ω��p�^�[���𓯂��񐔂ɐݒ肷�邽�߁j,24�̔{�����]�܂���
// �񎦈ʒu      �F��ʒ������������ӂ�8�ӏ�
// �񎦎h���̎�ށF4���
// ���ǁF2016.2.6 :�񎦃p�^�[�����ꎩ�̂̋L�^��Y��Ă����B
// ���K�p�̌n�񐶐��@�\�ǉ�
//
//

int prep_cb_exp3()
{
	static int st[TR_NUM][8]; // �e���s�ɂ�����8�ӏ��̎��ӈʒu�ɒ񎦂���h���ԍ�(0,1,2,3)
	int chpos[TR_NUM];        // �e���s�ɂ����Ďh�����ω�������ӈʒu(0,1,�E�E�E,7)
	int chst[TR_NUM];         // �ω����ɒ񎦂���h���ԍ�
	int att[TR_NUM];          // ���ӗU���̕���(0:���,1:����,2:����,3:�E��)
	int fch[4][4];            // �ω��p�^�[���̏o���񐔂�c�����邽�߂̏o���L�^�\�i��Ɨp�j

	int tr, i, j, p0, p1, itmp;
	char ren;
	std::string subnm;      // �����Q���Җ�
	std::string expnum = "3"; // �����ԍ�3�ŌŒ�

							  //cout << "�h���n��͐����ς݂ł�" << endl; //2016.2.9 �h���͐����ς�
							  //return 1;

	cout << "�����Q����=";
	cin >> subnm;

	//cout << "�����ԍ�=";
	//cin >> expnum;

	cout << "���K�p(y / n)";
	cin >> ren;

	cout << "---�h���n��̐�����---" << endl;

	// �����l�̐ݒ�:�h���̎��(1��̎��s�ŁA�����h����2�ӏ��ɒ񎦂���j
	for (tr = 0; tr < TR_NUM; tr++) {
		for (i = 0; i < 8; i++) { // ���Ӓ񎦈ʒu
			st[tr][i] = i % 4;
		}
	}

	// �����l�̐ݒ�F�ω��ʒu(�S���s��ʂ��đI���������ӈʒu�͓����񐔂ɐݒ肷��j
	for (tr = 0; tr < TR_NUM; tr++) {
		chpos[tr] = tr % 8;
	}

	// �����_���� 
	srand((unsigned)time(NULL));

	// �e���s�ɂ�����h���񎦈ʒu�̃����_����
	for (tr = 0; tr < TR_NUM; tr++) {
		for (i = 0; i < 20; i++) {
			p0 = rand() % 8;
			p1 = rand() % 8;
			if (p0 < 8 && p1 < 8) { // �s�v��������Ȃ����O�̂���
				itmp = st[tr][p0];
				st[tr][p0] = st[tr][p1];
				st[tr][p1] = itmp;
			}
		}//for(i=
	}//for(tr=


	 // �S���s��ʂ��Ă̎h���ω��ʒu�̃����_����
	while (1) {
		for (i = 0; i < 200; i++) {
			p0 = rand() % TR_NUM;
			p1 = rand() % TR_NUM;
			if (p0 < TR_NUM && p1 < TR_NUM) { // �s�v��������Ȃ���
				itmp = chpos[p0];
				chpos[p0] = chpos[p1];
				chpos[p1] = itmp;
			}
		}//for(i=
		 // �����ω��ʒu��3�񑱂�����Ē�������
		int cnt = 0;
		p0 = chpos[0];
		for (j = 0; j < TR_NUM; j++) {
			if (p0 == chpos[j]) {
				cnt++;
				if (cnt == 3) break;
			}
			else {
				p0 = chpos[j]; // �ʒu�̍X�V
				cnt = 1;
			}
		}
		if (cnt == 3) continue; // 3�񑱂����������x�����_����
		break; // 3�񖢖��ōŌ�܂œ��B
	}//while(1);


	 // �ω��p�^�[���̐���
	int ovlp;
	ovlp = TR_NUM / 6; // ���ݒ�i����̕ω��p�^�[���������񐔁F�o���I�ɐݒ�A�l���������Ə������I�������j

	for (i = 0; i < 4; i++) { // �o���L�^�\�̏�����
		for (j = 0; j < 4; j++) {
			fch[i][j] = 0;
		}
	}

	int chp, chs;
	for (tr = 0; tr < TR_NUM; tr++) {
		while (1) {
			chp = chpos[tr];   // �{���s�̕ω��ʒu
			chs = st[tr][chp]; // �h���񎦈ʒu�ɕω��O�ɒ񎦂����h���ԍ�
			p0 = rand() % 4;
			if (chs == p0) continue; // �����ԍ��ł͕ω����Ȃ�
			if (fch[chs][p0] < ovlp) {
				chst[tr] = p0; // �ω�������̎h���ԍ��̓o�^
				fch[chs][p0]++;
				break;
			}
		}//while(1)
	}//for(

	 // ���ӗU�������̐ݒ�
	 //
	 // 
	int pos, val;

	int dirptn[TR_NUM];


	// ���ӗU�������̐U�蕪��
	for (pos = 0; pos < 8; pos++) {
		if (pos == 1 || pos == 3 || pos == 4 || pos == 6)
			continue; // ���ӗU���̕����͊m�肵�Ă���̂Ŗ���
		val = 0;// 0,1�̒l���Ƃ�
		for (tr = 0; tr < TR_NUM; tr++) { // �U�������̂Q�����ւ̐U�蕪��(0 or 1�Łj
			if (chpos[tr] == pos) {
				dirptn[tr] = val;
				val = (val + 1) % 2; // �U�蕪������Q�������݂Ƃ���B
			}
		}
	}

	// �U�������̃Z�b�g
	for (tr = 0; tr < TR_NUM; tr++) {
		if (chpos[tr] == 0) {  // 0:�����, 1:������
			if (dirptn[tr] == 0)
				att[tr] = 0; // �����
			else
				att[tr] = 1; // ������
		}

		if (chpos[tr] == 1) { // 0:�����
			att[tr] = 0;
		}

		if (chpos[tr] == 2) {  // 0:�����, 3:�E����
			if (dirptn[tr] == 0)
				att[tr] = 0; // �����
			else
				att[tr] = 3; // �E����
		}

		if (chpos[tr] == 3) { // 1:������
			att[tr] = 1;
		}

		if (chpos[tr] == 4) { // 3:�E����
			att[tr] = 3;
		}

		if (chpos[tr] == 5) {  // 2:������, 1:������
			if (dirptn[tr] == 0)
				att[tr] = 2; // ������
			else
				att[tr] = 1; // ������
		}

		if (chpos[tr] == 6) { // 2:������
			att[tr] = 2;
		}

		if (chpos[tr] == 7) {  // 2:������, 3:�E����
			if (dirptn[tr] == 0)
				att[tr] = 2; // ������
			else
				att[tr] = 3; // �E����
		}
	} //for(tr=


	  /*
	  int dcnt[4] = { 0 };
	  for(tr = 0; tr < TR_NUM; tr++){
	  dcnt[att[tr]]++;
	  }
	  cout << "0:" << dcnt[0] << "  1:" << dcnt[1] << "  2:" << dcnt[2] << "  3:" << dcnt[3] << endl;
	  */


	  // �h���������ʂ̊m�F
	cout << "���s�ԍ�, �ω��ʒu, �ω��O�h�� �� �ω���h�� " << endl;
	for (tr = 0; tr < TR_NUM; tr++) {
		cout << "Tr-" << tr + 1 << " " << chpos[tr] << " " << st[tr][chpos[tr]] << "��" << chst[tr] << endl;
	}

	getchar();
	getchar();

	//�h���n��ւ̃t�@�C���ւ̊i�[
	//
	//
	// ����̎��s�̏��L�^
	std::string fname, imname;
	std::ostringstream ostr;
	fname = "order_";
	if (ren == 'n')
		fname = fname + "����" + expnum + "_" + subnm + ".csv"; // �t�@�C�����̐���
	else // ���K�Ȃ�
		fname = fname + "����" + expnum + "_���K_" + subnm + ".csv"; // �t�@�C�����̐���

																 // ���o�͗p�t�@�C���̐���
	ofstream fout(fname); // �o�̓t�@�C�����쐬����
	if (!fout) {
		cout << "�t�@�C�����J���܂���" << endl;
		return 0;
	}
	fout << "���s�ԍ�,�ω��ʒu,�ω��O�h��,�ω���h��,�U������ " << endl; // �U��������������(2016.2.28)
	for (tr = 0; tr < TR_NUM; tr++) {
		fout << tr + 1 << "," << chpos[tr] << "," << st[tr][chpos[tr]] << "," << chst[tr] << "," << att[tr] << endl;
	}

	fout << "���s�ԍ�,�񎦎h���n��" << endl;
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
/* CB�����̏����i�񎦎h���n��̐����j*/
/*-----------------------------------*/
//
// ���s�񐔁F8�̔{��(�ω��ʒu,�ω��p�^�[���𓯂��񐔂ɐݒ肷�邽�߁j,24�̔{�����]�܂���
// �񎦈ʒu      �F��ʒ������������ӂ�8�ӏ�
// �񎦎h���̎�ށF4���
// ���ǁF2016.2.6 :�񎦃p�^�[�����ꎩ�̂̋L�^��Y��Ă����B
// ���K�p�̌n�񐶐��@�\�ǉ�
//
// C����p�ɕۑ�����t�H���_��ύX(2016.4.10�`�j
// ����2�p

int prep_cb_exp2()
{
	static int st[TR_NUM][8]; // �e���s�ɂ�����8�ӏ��̎��ӈʒu�ɒ񎦂���h���ԍ�(0,1,2,3)
	int chpos[TR_NUM];        // �e���s�ɂ����Ďh�����ω�������ӈʒu(0,1,�E�E�E,7)
	int chst[TR_NUM];         // �ω����ɒ񎦂���h���ԍ�
	int fch[4][4];            // �ω��p�^�[���̏o���񐔂�c�����邽�߂̏o���L�^�\�i��Ɨp�j
	int intvl[TR_NUM];        // �i�ő�j�X�V�Ԋu�i6,8,10,12s)


	int tr, i, j, p0, p1, itmp;
	char ren;
	std::string subnm;  // �����Q���Җ�
	std::string expnum; // �����ԍ�(2:�s���Ӑ��u�ڎ���)


						//cout << "�h���n��͐����ς݂ł�" << endl; //2016.4.3 �h���͐����ς�
						//return 1;

	cout << "�����Q����=";
	cin >> subnm;

	cout << "�����ԍ�=(2�̂݁j";
	cin >> expnum;

	// expnum��2�ȊO�̂Ƃ��߂�R�[�h��ǉ�����

	cout << "���K�p(y / n)";
	cin >> ren;

	cout << "---�h���n��̐�����---" << endl;

	// �����l�̐ݒ�:�h���̎��(1��̎��s�ŁA�����h����2�ӏ��ɒ񎦂���j
	for (tr = 0; tr < TR_NUM; tr++) {
		for (i = 0; i < 8; i++) { // ���Ӓ񎦈ʒu
			st[tr][i] = i % 4;
		}
	}

	// �����l�̐ݒ�F�ω��ʒu(�S���s��ʂ��đI���������ӈʒu�͓����񐔂ɐݒ肷��j
	for (tr = 0; tr < TR_NUM; tr++) {
		chpos[tr] = tr % 8;
	}

	// �����l�̐ݒ�F�X�V�Ԋu(6,8,10,12)
	tr = 0;
	while (1) {
		for (i = 6; i <= 12; i += 2) {
			intvl[tr] = i;
			tr++;
		}
		if (tr == TR_NUM)
			break;
	}

	// �����_���� 
	srand((unsigned)time(NULL));

	// �e���s�ɂ�����h���񎦈ʒu�̃����_����
	for (tr = 0; tr < TR_NUM; tr++) {
		for (i = 0; i < 20; i++) {
			p0 = rand() % 8;
			p1 = rand() % 8;
			if (p0 < 8 && p1 < 8) { // �s�v��������Ȃ����O�̂���
				itmp = st[tr][p0];
				st[tr][p0] = st[tr][p1];
				st[tr][p1] = itmp;
			}
		}//for(i=
	}//for(tr=


	 // �S���s��ʂ��Ă̎h���ω��ʒu�̃����_����
	while (1) {
		for (i = 0; i < 200; i++) {
			p0 = rand() % TR_NUM;
			p1 = rand() % TR_NUM;
			if (p0 < TR_NUM && p1 < TR_NUM) { // �s�v��������Ȃ���
				itmp = chpos[p0];
				chpos[p0] = chpos[p1];
				chpos[p1] = itmp;
			}
		}//for(i=
		 // �����ω��ʒu��3�񑱂�����Ē�������
		int cnt = 0;
		p0 = chpos[0];
		for (j = 0; j < TR_NUM; j++) {
			if (p0 == chpos[j]) {
				cnt++;
				if (cnt == 3) break;
			}
			else {
				p0 = chpos[j]; // �ʒu�̍X�V
				cnt = 1;
			}
		}
		if (cnt == 3) continue; // 3�񑱂����������x�����_����
		break; // 3�񖢖��ōŌ�܂œ��B
	}//while(1);


	 // �ω��p�^�[���̐���
	int ovlp;
	ovlp = TR_NUM / 6; // ���ݒ�i����̕ω��p�^�[���������񐔁F�o���I�ɐݒ�A�l���������Ə������I�������j

	for (i = 0; i < 4; i++) { // �o���L�^�\�̏�����
		for (j = 0; j < 4; j++) {
			fch[i][j] = 0;
		}
	}

	int chp, chs;
	for (tr = 0; tr < TR_NUM; tr++) {
		while (1) {
			chp = chpos[tr];   // �{���s�̕ω��ʒu
			chs = st[tr][chp]; // �h���񎦈ʒu�ɕω��O�ɒ񎦂����h���ԍ�
			p0 = rand() % 4;
			if (chs == p0) continue; // �����ԍ��ł͕ω����Ȃ�
			if (fch[chs][p0] < ovlp) {
				chst[tr] = p0; // �ω�������̎h���ԍ��̓o�^
				fch[chs][p0]++;
				break;
			}
		}//while(1)
	}//for(



	 // �e���s�ɂ�����X�V�Ԋu�̃����_����
	while (1) {
		for (i = 0; i < 30; i++) {
			p0 = rand() % (TR_NUM + 1);
			p1 = rand() % (TR_NUM + 1);
			if (p0 < TR_NUM && p1 < TR_NUM) { // �s�v��������Ȃ����O�̂���
				itmp = intvl[p0];
				intvl[p0] = intvl[p1];
				intvl[p1] = itmp;
			}
		}//for(i=

		 // �����ω��ʒu��3�񑱂�����Ē�������
		int cnt = 0;
		p0 = intvl[0];
		for (j = 0; j < TR_NUM; j++) {
			if (p0 == intvl[j]) {
				cnt++;
				if (cnt == 3) break;
			}
			else {
				p0 = intvl[j]; // �ʒu�̍X�V
				cnt = 1;
			}
		}
		if (cnt == 3) continue; // 3�񑱂����������x�����_����
		break; // 3�񖢖��ōŌ�܂œ��B
	}

	// �h���������ʂ̊m�F
	cout << "���s�ԍ�, �ω��ʒu, �ω��O�h�� �� �ω���h��,�X�V�Ԋu " << endl;
	for (tr = 0; tr < TR_NUM; tr++) {
		cout << "Tr-" << tr + 1 << " " << chpos[tr] << " " << st[tr][chpos[tr]] << "��" << chst[tr] << " " << intvl[tr] << endl;
	}

	getchar();
	getchar();

	//�h���n��ւ̃t�@�C���ւ̊i�[
	//
	// �e���ӈʒu�ɒ񎦂���h���ԍ���g�ݓ���Ă��Ȃ��i�v���ǁj2016.1.16
	//
	// ����̎��s�̏��L�^
	std::string fname, imname;
	std::ostringstream ostr;

	fname = "c:\\IEEJ_C_2016\\";
	fname = fname + "exp" + expnum + "\\" + subnm + "\\order_";
	if (ren == 'n')
		fname = fname + "����" + expnum + "_" + subnm + ".csv"; // �t�@�C�����̐���
	else // ���K�Ȃ�
		fname = fname + "����" + expnum + "_���K_" + subnm + ".csv"; // �t�@�C�����̐���

																 /*
																 fname = "order_";
																 if (ren == 'n')
																 fname = fname + "����"+expnum+"_"+subnm + ".csv"; // �t�@�C�����̐���
																 else // ���K�Ȃ�
																 fname = fname + "����" + expnum + "_���K_" + subnm + ".csv"; // �t�@�C�����̐���
																 */



																 // ���o�͗p�t�@�C���̐���
	ofstream fout(fname); // �o�̓t�@�C�����쐬����
	if (!fout) {
		cout << fname << endl;
		cout << "�t�@�C�����J���܂���" << endl;
		getchar();
		getchar();
		return 0;
	}
	fout << "���s�ԍ�,�ω��ʒu,�ω��O�h��,�ω���h��,�X�V�Ԋu " << endl;
	for (tr = 0; tr < TR_NUM; tr++) {
		fout << tr + 1 << "," << chpos[tr] << "," << st[tr][chpos[tr]] << "," << chst[tr] << "," << intvl[tr] << endl;
	}

	fout << "���s�ԍ�,�񎦎h���n��" << endl;
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
/* CB�����̏����i�񎦎h���n��̐����j*/
/*-----------------------------------*/
//
// ���s�񐔁F8�̔{��(�ω��ʒu,�ω��p�^�[���𓯂��񐔂ɐݒ肷�邽�߁j,24�̔{�����]�܂���
// �񎦈ʒu      �F��ʒ������������ӂ�8�ӏ�
// �񎦎h���̎�ށF4���
// ���ǁF2016.2.6 :�񎦃p�^�[�����ꎩ�̂̋L�^��Y��Ă����B
// ���K�p�̌n�񐶐��@�\�ǉ�
//
// C����2016�p�ɕۑ�����t�H���_��ύX(2016.4.3�`�j
// ����1�p

int prep_cb_exp()
{
	static int st[TR_NUM][8]; // �e���s�ɂ�����8�ӏ��̎��ӈʒu�ɒ񎦂���h���ԍ�(0,1,2,3)
	int chpos[TR_NUM];        // �e���s�ɂ����Ďh�����ω�������ӈʒu(0,1,�E�E�E,7)
	int chst[TR_NUM];         // �ω����ɒ񎦂���h���ԍ�
	int fch[4][4];            // �ω��p�^�[���̏o���񐔂�c�����邽�߂̏o���L�^�\�i��Ɨp�j

	int tr, i, j, p0, p1, itmp;
	char ren;
	std::string subnm;  // �����Q���Җ�
	std::string expnum; // �����ԍ�(1:���Ӑ��u�ڎ����A2:�s���Ӑ��u�ڎ���)


	cout << "�h���n��͐����ς݂ł�" << endl; //2016.4.3 �h���͐����ς�
	return 1;

	cout << "�����Q����=";
	cin >> subnm;

	cout << "�����ԍ�=";
	cin >> expnum;

	// expnum��3�ȏ�̂Ƃ��߂�R�[�h��ǉ�����

	cout << "���K�p(y / n)";
	cin >> ren;

	cout << "---�h���n��̐�����---" << endl;

	// �����l�̐ݒ�:�h���̎��(1��̎��s�ŁA�����h����2�ӏ��ɒ񎦂���j
	for (tr = 0; tr < TR_NUM; tr++) {
		for (i = 0; i < 8; i++) { // ���Ӓ񎦈ʒu
			st[tr][i] = i % 4;
		}
	}

	// �����l�̐ݒ�F�ω��ʒu(�S���s��ʂ��đI���������ӈʒu�͓����񐔂ɐݒ肷��j
	for (tr = 0; tr < TR_NUM; tr++) {
		chpos[tr] = tr % 8;
	}

	// �����_���� 
	srand((unsigned)time(NULL));

	// �e���s�ɂ�����h���񎦈ʒu�̃����_����
	for (tr = 0; tr < TR_NUM; tr++) {
		for (i = 0; i < 20; i++) {
			p0 = rand() % 8;
			p1 = rand() % 8;
			if (p0 < 8 && p1 < 8) { // �s�v��������Ȃ����O�̂���
				itmp = st[tr][p0];
				st[tr][p0] = st[tr][p1];
				st[tr][p1] = itmp;
			}
		}//for(i=
	}//for(tr=


	 // �S���s��ʂ��Ă̎h���ω��ʒu�̃����_����
	while (1) {
		for (i = 0; i < 200; i++) {
			p0 = rand() % TR_NUM;
			p1 = rand() % TR_NUM;
			if (p0 < TR_NUM && p1 < TR_NUM) { // �s�v��������Ȃ���
				itmp = chpos[p0];
				chpos[p0] = chpos[p1];
				chpos[p1] = itmp;
			}
		}//for(i=
		 // �����ω��ʒu��3�񑱂�����Ē�������
		int cnt = 0;
		p0 = chpos[0];
		for (j = 0; j < TR_NUM; j++) {
			if (p0 == chpos[j]) {
				cnt++;
				if (cnt == 3) break;
			}
			else {
				p0 = chpos[j]; // �ʒu�̍X�V
				cnt = 1;
			}
		}
		if (cnt == 3) continue; // 3�񑱂����������x�����_����
		break; // 3�񖢖��ōŌ�܂œ��B
	}//while(1);


	 // �ω��p�^�[���̐���
	int ovlp;
	ovlp = TR_NUM / 6; // ���ݒ�i����̕ω��p�^�[���������񐔁F�o���I�ɐݒ�A�l���������Ə������I�������j

	for (i = 0; i < 4; i++) { // �o���L�^�\�̏�����
		for (j = 0; j < 4; j++) {
			fch[i][j] = 0;
		}
	}

	int chp, chs;
	for (tr = 0; tr < TR_NUM; tr++) {
		while (1) {
			chp = chpos[tr];   // �{���s�̕ω��ʒu
			chs = st[tr][chp]; // �h���񎦈ʒu�ɕω��O�ɒ񎦂����h���ԍ�
			p0 = rand() % 4;
			if (chs == p0) continue; // �����ԍ��ł͕ω����Ȃ�
			if (fch[chs][p0] < ovlp) {
				chst[tr] = p0; // �ω�������̎h���ԍ��̓o�^
				fch[chs][p0]++;
				break;
			}
		}//while(1)
	}//for(


	 // �h���������ʂ̊m�F
	cout << "���s�ԍ�, �ω��ʒu, �ω��O�h�� �� �ω���h�� " << endl;
	for (tr = 0; tr < TR_NUM; tr++) {
		cout << "Tr-" << tr + 1 << " " << chpos[tr] << " " << st[tr][chpos[tr]] << "��" << chst[tr] << endl;
	}

	getchar();
	getchar();

	//�h���n��ւ̃t�@�C���ւ̊i�[
	//
	// �e���ӈʒu�ɒ񎦂���h���ԍ���g�ݓ���Ă��Ȃ��i�v���ǁj2016.1.16
	//
	// ����̎��s�̏��L�^
	std::string fname, imname;
	std::ostringstream ostr;

	fname = "c:\\IEEJ_C_2016\\";
	fname = fname + "exp" + expnum + "\\" + subnm + "\\order_";
	if (ren == 'n')
		fname = fname + "����" + expnum + "_" + subnm + ".csv"; // �t�@�C�����̐���
	else // ���K�Ȃ�
		fname = fname + "����" + expnum + "_���K_" + subnm + ".csv"; // �t�@�C�����̐���

																 /*
																 fname = "order_";
																 if (ren == 'n')
																 fname = fname + "����"+expnum+"_"+subnm + ".csv"; // �t�@�C�����̐���
																 else // ���K�Ȃ�
																 fname = fname + "����" + expnum + "_���K_" + subnm + ".csv"; // �t�@�C�����̐���
																 */



																 // ���o�͗p�t�@�C���̐���
	ofstream fout(fname); // �o�̓t�@�C�����쐬����
	if (!fout) {
		cout << fname << endl;
		cout << "�t�@�C�����J���܂���" << endl;
		return 0;
	}
	fout << "���s�ԍ�,�ω��ʒu,�ω��O�h��,�ω���h�� " << endl;
	for (tr = 0; tr < TR_NUM; tr++) {
		fout << tr + 1 << "," << chpos[tr] << "," << st[tr][chpos[tr]] << "," << chst[tr] << endl;
	}

	fout << "���s�ԍ�,�񎦎h���n��" << endl;
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
/* �����x�^�C�}�[*/
/*---------------*/
//
//
int time_delay(
	int tim // �x������(ms)
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




/*----- �f�[�^����------*/


// 2016.5.21
/*--------------------*/
/* �������Z�o(����2)  */
/*--------------------*/
//
// BL���N�����̎��s�͂Ƃ肠������������(2016.5.21) 
//
//
int analysis2()
{
	int chpos[TR_NUM4];        // �e���s�ɂ����Ďh�����ω�������ӈʒu(0,1,�E�E�E,7)
	int chst[TR_NUM4];         // �ω����ɒ񎦂���h���ԍ�
	int prst[TR_NUM4];         // �ω��O�̎h���ԍ�
	int intvl[TR_NUM4];        // �񎦊Ԋu

	int anspos[TR_NUM4];       // �h���ω��ʒu�i�񓚁j(1,2,3,�E�E�E8, 9:�킩��Ȃ��j
	int blon[TR_NUM4];         // BL���N�̗L��(-1:���C0:�L�j

	int sn, fn, tr, itmp1, itmp2, itmp3, itmp4, itmp5;
	int trnum;
	char sbuf[100];
	char split;
	std::string sfname, fns;
	std::string subnm[] = { "subA", "subB", "subC", "subD", "subE", "subF", "subG" };
	ifstream fin;
	ofstream fo;


	int exp = 1;


	cout << "---�������Z�o�i����2�j---" << endl;

	for (sn = 0; sn < 6; sn++) { // �����Q����6����
		if (sn != 1) continue;  // subB������������(2016.5.27)

								// �h���񎦏����f�[�^�ǂݍ���
		std::string sfname;

		for (fn = 0; fn <= 3; fn++) { // �t�H���_�ԍ�
			fns = std::to_string(fn + 1);
			sfname = "c:\\IEEJ_C_2016\\";
			sfname = sfname + "exp2" + "\\" + subnm[sn] + "\\" + subnm[sn] + fns + "\\order_";
			sfname = sfname + "����2" + "_" + subnm[sn] + fns + ".csv";     // �t�@�C�����̐���

																		  // �h���񎦏����f�[�^�t�@�C���̓ǂݍ���
			ifstream fin(sfname);
			if (!fin) {
				cout << "�h���񎦗p�t�@�C��������܂���" << sfname << endl;
				getchar();
				getchar();
				return 0;
			}

			// �ω����̓ǂݎ��
			fin >> sbuf; // 1�s���ǂݔ�΂�

						 // ���s�ԍ��A�ω��ʒu�A�ω��O�h���A�ω���h��,�񎦊Ԋu
			for (tr = 0; tr < TR_NUM; tr++) {
				fin >> itmp1 >> split >> itmp2 >> split >> itmp3 >> split >> itmp4 >> split >> itmp5;
				chpos[tr + fn*TR_NUM] = itmp2;
				prst[tr + fn*TR_NUM] = itmp3;
				chst[tr + fn*TR_NUM] = itmp4;
				intvl[tr + fn*TR_NUM] = itmp5;
				//cout << tr+1 << ","<< chpos[tr] << "," << prst[tr] << "," << chst[tr]<< endl;//�m�F
			}
			fin.close();
		}//for(fn=


		 // �񎦏��̓ǂݍ��݌��ʂ̊m�F
		cout << "���s�ԍ�,�ω��ʒu,���ݎh���ԍ�,�ω���h���ԍ�,�񎦊Ԋu" << endl;
		for (tr = 0; tr < TR_NUM4; tr++) {
			cout << tr + 1 << ":" << chpos[tr] << "," << prst[tr] << "," << chst[tr] << "," << intvl[tr] << endl;
		}


		// �񓚃f�[�^�̓ǂݍ���
		sfname = "c:\\IEEJ_C_2016\\exp2\\";
		sfname += subnm[sn] + "\\";
		sfname += "info_����2_" + subnm[sn] + ".csv"; // �t�@�C�����̐���

													// �񓚃f�[�^�t�@�C���̓ǂݍ���
		fin.open(sfname);
		if (!fin) {
			cout << "�񓚃f�[�^�t�@�C��" << sfname << "������܂���" << endl;
			return 0;
		}

		// �ω����̓ǂݎ��
		fin >> sbuf; // 1�s���ǂݔ�΂�
					 // ���s�ԍ��A�ω��ʒu
		for (tr = 0; tr < TR_NUM4; tr++) {
			if (fin.eof()) { // ���s�񐔂����ł͂Ȃ��̂ŁC�t�@�C���̍Ō��T���D
				break;
			}
			fin >> itmp1 >> split >> itmp2 >> split >> itmp3 >> split >> itmp4;
			anspos[tr] = itmp4;
			blon[tr] = itmp3;
			cout << tr + 1 << "," << anspos[tr] << endl;//�m�F
		}
		fin.close(); // �t�@�C���N���[�Y

		trnum = tr; // ���s�񐔂̐ݒ�

					// ���𗦎Z�o����
					// ���͌��ʂ̏�������
		sfname = "c:\\IEEJ_C_2016\\exp2\\";
		sfname += subnm[sn] + "\\";
		sfname += "anal1_����2_" + subnm[sn] + ".csv"; // �t�@�C�����̐���
		fo.open(sfname);
		if (!fo) {
			cout << "�t�@�C�����J���܂���" << endl;
			return 0;
		}

		std::string judge;
		int mukou = 0; // BL�񐶋N���s��
		int seikai = 0;  //����
		int num9 = 0;    // 9�i�킩��Ȃ��j�Ɠ�������
		fo << "�ω��ʒu,�񓚈ʒu,����" << endl;
		for (tr = 0; tr < trnum; tr++) {
			if (blon[tr] == -1) { //BL�񐶋N���s�j
				judge = "�|";
				mukou++;
			}

			else if (chpos[tr] + 1 == anspos[tr]) {
				judge = "��";
				seikai++;
			}
			else {
				judge = "�~";
				if (anspos[tr] == 9)
					num9++;
			}
			fo << chpos[tr] + 1 << "," << anspos[tr] << "," << judge << endl;
		}
		fo << "-----------------------------" << endl;
		fo << "����,����(%)" << endl;
		fo << seikai << "," << 100.0*seikai / (trnum - mukou) << endl;
		fo << "�킩��Ȃ��񓚐�" << endl;
		fo << num9 << endl;
		fo.close();
	}//for(sn=
}








// 2016.5.5
/*--------------------*/
/* �������Z�o(����1)  */
/*--------------------*/
//
// �������s(BL���N�����j�𖳎�����(2016.5.6) 
//
int analysis1()
{
	int chpos[TR_NUM];        // �e���s�ɂ����Ďh�����ω�������ӈʒu(0,1,�E�E�E,7)
	int chst[TR_NUM];         // �ω����ɒ񎦂���h���ԍ�
	int prst[TR_NUM];         // �ω��O�̎h���ԍ�

	int anspos[TR_NUM];       // �h���ω��ʒu�i�񓚁j(1,2,3,�E�E�E8, 9:�킩��Ȃ��j


	int sn, tr, itmp1, itmp2, itmp3, itmp4, itmp5;
	char sbuf[100];
	char split;
	std::string sfname;
	std::string subnm[] = { "subA", "subB", "subC", "subD","subE", "subF", "subG" };
	ifstream fin;
	ofstream fo;


	int exp = 1;


	cout << "---�������Z�o�i����1�j---" << endl;

	for (sn = 1; sn < 6; sn++) { // �����Q����5����
								 // �h���񎦏����f�[�^�ǂݍ���
		sfname = "c:\\IEEJ_C_2016\\exp1\\";
		sfname += subnm[sn] + "\\";
		sfname += "order_����1_" + subnm[sn] + ".csv"; // �t�@�C�����̐���

													 // �h���񎦏����f�[�^�t�@�C���̓ǂݍ���
		fin.open(sfname);
		if (!fin) {
			cout << "�h���񎦗p�t�@�C��" << sfname << "������܂���" << endl;
			return 0;
		}

		// �ω����̓ǂݎ��
		fin >> sbuf; // 1�s���ǂݔ�΂�

					 // ���s�ԍ��A�ω��ʒu�A�ω��O�h���A�ω���h��
		for (tr = 0; tr < TR_NUM; tr++) {
			if (exp == 3) { // ����3
				fin >> itmp1 >> split >> itmp2 >> split >> itmp3 >> split >> itmp4 >> split >> itmp5;
			}
			else {
				fin >> itmp1 >> split >> itmp2 >> split >> itmp3 >> split >> itmp4;
			}
			chpos[tr] = itmp2;
			prst[tr] = itmp3;
			chst[tr] = itmp4;
			cout << tr + 1 << "," << chpos[tr] << "," << prst[tr] << "," << chst[tr] << endl;//�m�F
		}
		fin.close(); // �t�@�C���N���[�Y


					 // �񓚃f�[�^�̓ǂݍ���
		sfname = "c:\\IEEJ_C_2016\\exp1\\";
		sfname += subnm[sn] + "\\";
		sfname += "info_����1_" + subnm[sn] + ".csv"; // �t�@�C�����̐���

													// �񓚃f�[�^�t�@�C���̓ǂݍ���
		fin.open(sfname);
		if (!fin) {
			cout << "�񓚃f�[�^�t�@�C��" << sfname << "������܂���" << endl;
			return 0;
		}

		// �ω����̓ǂݎ��
		fin >> sbuf; // 1�s���ǂݔ�΂�
					 // ���s�ԍ��A�ω��ʒu
		for (tr = 0; tr < TR_NUM; tr++) {
			fin >> itmp1 >> split >> itmp2 >> split >> itmp3 >> split >> itmp4;
			anspos[tr] = itmp4;
			cout << tr + 1 << "," << anspos[tr] << endl;//�m�F
		}
		fin.close(); // �t�@�C���N���[�Y


					 // ���𗦎Z�o����
					 // �񓚃f�[�^�̓ǂݍ���
		sfname = "c:\\IEEJ_C_2016\\exp1\\";
		sfname += subnm[sn] + "\\";
		sfname += "anal1_����1_" + subnm[sn] + ".csv"; // �t�@�C�����̐���
		fo.open(sfname);
		if (!fo) {
			cout << "�t�@�C�����J���܂���" << endl;
			return 0;
		}

		std::string judge;
		int mukou = 0; // �������s��
		int seikai = 0;  //����
		int num9 = 0;    // 9�i�킩��Ȃ��j�Ɠ�������
		fo << "�ω��ʒu,�񓚈ʒu,����" << endl;
		for (tr = 0; tr < TR_NUM; tr++) {
			if (anspos[tr] == -1) { //�������s�iBL���o�ł����j
				judge = "�|";
				mukou++;
			}

			else if (chpos[tr] + 1 == anspos[tr]) {
				judge = "��";
				seikai++;
			}
			else {
				judge = "�~";
				if (anspos[tr] == 9)
					num9++;
			}
			fo << chpos[tr] + 1 << "," << anspos[tr] << "," << judge << endl;
		}
		fo << "-----------------------------" << endl;
		fo << "����,����(%)" << endl;
		fo << seikai << "," << 100.0*seikai / (TR_NUM - mukou) << endl;
		fo << "�킩��Ȃ��񓚐�" << endl;
		fo << num9 << endl;
		fo.close();
	}//for(sn=
}