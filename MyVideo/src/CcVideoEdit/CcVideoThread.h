#pragma once
#include <string>
#include <QThread>
#include <QMutex>
#include <opencv2\core.hpp>
#include <mutex>;
#include <condition_variable>
class CcVideoThread:public QThread
{
	Q_OBJECT
public:
	CcVideoThread& operator=(const CcVideoThread&) = delete;
	CcVideoThread(const CcVideoThread&) = delete;

	int fps = 0;
	static CcVideoThread* GetInstance() {
		static CcVideoThread cc;
		return &cc;
	}
	//打开一号视频资源
	bool Open(const std::string& file);

	//打开二号视频源
	bool Open2(const std::string& file);

	//返回当前播放位置
	double GetPos();

	//线程入口函数
	void run() override;

	//跳转视频 frame帧位置
	bool Seek(int frame);
	bool Seek(double pos);

	//开始保存视频
	bool StartSave(const std::string filename, int width=0, int height=0,bool isColor=true);
	//停止保存视频，写入视频帧的索引
	void StopSave();

	//添加水印
	void SetMark(cv::Mat mark);

	~CcVideoThread();

	void Play() {
		std::unique_lock<std::mutex> lock(_mutex);
		isPlay = true;
		_cond.notify_all();
	}
	void Pause() {
		std::unique_lock<std::mutex> lock(_mutex);
		isPlay = false;
	}

	int width = 0;
	int height = 0;
signals:
	//显示原视频1图像
	void ViewImage1(cv::Mat mat);
	//显示原视频2图像
	void ViewImage2(cv::Mat mat);
	//显示输出图像
	void ViewDst(cv::Mat mat);
	//保存结束
	void SaveEnd();
	

private:

	//打开一号视频源文件
	CcVideoThread();
	//是否开始写视频
	bool isWrite = false;
	//是否播放
	bool isPlay = true;
	cv::Mat _mark;
	std::mutex _mutex;
	std::condition_variable _cond;


};

