#include "CcVideoThread.h"
#include <opencv2\imgcodecs.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>
#include <iostream>

using namespace cv;

//一号视频源
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
		if (!cap1.isOpened()) {	//判断视频是否打开开
			mutex.unlock();
			msleep(5);
			continue;
		}
		if (!cap1.read(mat1) || mat1.empty()) {	//读取一帧视频，解码并颜色转换
			mutex.unlock();
			msleep(5);
			continue;
		}
		//显示图像

		mutex.unlock();
	}
}
