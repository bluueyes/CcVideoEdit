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

	//�߳���ں���
	void run() override;
	~CcVideoThread();

signals:
	void ViewImage1(cv::Mat mat);

private:
	//��һ����ƵԴ�ļ�
	CcVideoThread();
	QMutex mutex;
};

