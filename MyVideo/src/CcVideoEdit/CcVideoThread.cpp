#include "CcVideoThread.h"
#include <opencv2\imgcodecs.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>
#include <iostream>

using namespace cv;

//һ����ƵԴ
static VideoCapture cap1;

bool CcVideoThread::Open(const std::string& file)
{
	std::cout << "open: " << file << std::endl;
	mutex.lock();
	bool re=cap1.open(file);
	mutex.unlock();
	std::cout << re << std::endl;
	return re;
}

CcVideoThread::CcVideoThread()
{
}

CcVideoThread::~CcVideoThread()
{
}

void CcVideoThread::run()
{
	Mat mat1;
	for (;;) {
		mutex.lock();
		if (!cap1.isOpened()) {	//�ж���Ƶ�Ƿ�򿪿�
			mutex.unlock();
			msleep(5);
			continue;
		}
		if (!cap1.read(mat1) || mat1.empty()) {	//��ȡһ֡��Ƶ�����벢��ɫת��
			mutex.unlock();
			msleep(5);
			continue;
		}
		//��ʾͼ��

		mutex.unlock();
	}
}
