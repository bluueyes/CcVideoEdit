#include "CcVideoThread.h"
#include <opencv2\imgcodecs.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>
#include <iostream>
#include "CcFilter.h"
#include <thread>

using namespace cv;

//一号视频源
static VideoCapture cap1;
static VideoCapture cap2;

static bool isexit = false;

//保存视频
static VideoWriter vw;

bool CcVideoThread::Open(const std::string& file)
{
	std::cout << "open: " << file << std::endl;
	_mutex.lock();
	bool re=cap1.open(file);
	_mutex.unlock();
	Seek(0);
	_cond.notify_all();
	std::cout << re << std::endl;
	if (!re) {
		return re;
	}
	fps = cap1.get(CAP_PROP_FPS);
	width = cap1.get(CAP_PROP_FRAME_WIDTH);
	height = cap1.get(CAP_PROP_FRAME_HEIGHT);
	if (fps <= 0) fps = 25;
	src1file = file;

	double count = cap1.get(CAP_PROP_FRAME_COUNT);
	totalMs = (count /(double)fps) * 1000;
	
	return true;
}

bool CcVideoThread::Open2(const std::string& file)
{

	std::cout << "open: " << file << std::endl;
	_mutex.lock();
	bool re = cap2.open(file);
	_mutex.unlock();
	_cond.notify_all();
	Seek(0);

	std::cout << re << std::endl;
	if (!re) {
		return re;
	}
	//fps = cap1.get(CAP_PROP_FPS);
	width2 = cap2.get(CAP_PROP_FRAME_WIDTH);
	height2 = cap2.get(CAP_PROP_FRAME_HEIGHT);
	//if (fps <= 0) fps = 25;
	return true;
}

//返回当前播放位置
double CcVideoThread::GetPos()
{
	double pos = 0;
	std::lock_guard<std::mutex> lock(_mutex);
	if (!cap1.isOpened()) {
		return pos;
	}
	double count = cap1.get(CAP_PROP_FRAME_COUNT);
	double cur = cap1.get(CAP_PROP_POS_FRAMES);
	if (count > 0.0001) {
		pos = cur / count;
	}
	return pos;
}

void CcVideoThread::SetBegin(double pos) {
	std::lock_guard<std::mutex> lock(_mutex);
	double count = cap1.get(CAP_PROP_FRAME_COUNT);
	int frame = pos * count;
	begin = frame;
}
void CcVideoThread::SetEnd(double pos)
{
	std::lock_guard<std::mutex> lock(_mutex);
	double count = cap1.get(CAP_PROP_FRAME_COUNT);
	int frame = pos * count;
	end = frame;
}


CcVideoThread::CcVideoThread()
{
	//setenv("OPENCV_FFMPEG_CAPTURE_OPTIONS", "threads;1", 1);
	start();
}

bool CcVideoThread::Seek(int frame)
{
	std::lock_guard<std::mutex> lock(_mutex);
	if (!cap1.isOpened()) {
		return false;
	}
	int re = cap1.set(CAP_PROP_POS_FRAMES,frame);
	if (cap2.isOpened()) {

		cap2.set(CAP_PROP_POS_FRAMES, frame);
	}
	return true;
}

bool CcVideoThread::Seek(double pos)
{

	double count = cap1.get(CAP_PROP_FRAME_COUNT);
	int frame=pos * count;
	
	return Seek(frame);
}

bool CcVideoThread::StartSave(const std::string filename, int width, int height,bool isColor)
{
	Seek(begin);

	if (!cap1.isOpened()) return false;
	if (width <= 0) width = cap1.get(CAP_PROP_FRAME_WIDTH);
	if (height <= 0) height = cap1.get(CAP_PROP_FRAME_HEIGHT);
	vw.open(filename, VideoWriter::fourcc('X', '2', '6', '4'),
			this->fps,Size(width,height),isColor);
	if (!vw.isOpened()) {
		std::cout << "start save filed " << std::endl;
	}
	this->isWrite = true;
	dstfile = filename;
	return true;

}

void CcVideoThread::StopSave()
{

	std::cout << " Stop save"<<std::endl;
	vw.release();
	SaveEnd();
	isWrite = false;
}

void CcVideoThread::SetMark(Mat mark)
{
	std::unique_lock<std::mutex> lock(_mutex);
	this->_mark = mark;
}

CcVideoThread::~CcVideoThread()
{
	{
		std::lock_guard<std::mutex> lock(_mutex);

		isexit = true;
	}
	_cond.notify_all();
	wait();
	cap1.release();
}

void CcVideoThread::run()
{
	Mat mat1;
	std::thread t;
	for (; !isexit;) {
		{
			std::unique_lock lock(_mutex);
			_cond.wait(lock, [&]() {
				if (isexit) return true;
				return cap1.isOpened() && isPlay;
				});	//判断视频是否打开开



			_cond.wait(lock, [&]() {
				if (isexit) return true;
				double cur = cap1.get(CAP_PROP_POS_FRAMES);
				bool ret=(!(cur>=end) && cap1.read(mat1)) && !(mat1.empty());	//读取一帧视频，解码并颜色转换
				//导出到结尾位置，停止导出
				if (!ret&&isWrite) {
					StopSave();
				}
				return ret;
				});

			if (isexit) {
				lock.unlock();
				break;
			}

		}
		int s = 1;
		Mat mat2 = _mark;
		if (cap2.isOpened()) {
			cap2.read(mat2);
		}
		//通过过滤器生成视频
		if (!isWrite) {
			t = std::thread([&]() {
				Mat dst = CcFilter::Get()->Filter(mat1, mat2);
				//显示生成后的图像
					ViewDst(dst);
				});
			//显示图像1
			ViewImage1(mat1);
			//显示图像2
			if (!mat2.empty()) {
				ViewImage2(mat2);
			}
			s = 1000 / fps;
			msleep(s);
			t.join();
		}
		else {
			s = 1;
			Mat dst = CcFilter::Get()->Filter(mat1, mat2);
			vw.write(dst);
			msleep(s);
		}
		
		

	}

}
