#pragma once
#include <string>
#include <QThread>
#include <QMutex>
#include <opencv2\core.hpp>
class CcVideoThread:public QThread
{
	Q_OBJECT
public:
	static CcVideoThread* GetInstance() {
		static CcVideoThread cc;
		return &cc;
	}
	bool Open(const std::string& file);

	CcVideoThread(const CcVideoThread&) = delete;
	CcVideoThread& operator=(const CcVideoThread&) = delete;

	//线程入口函数
	void run() override;
	~CcVideoThread();

signals:
	void ViewImage1(cv::Mat mat);

private:
	//打开一号视频源文件
	CcVideoThread();
	QMutex mutex;
};

