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
	//��һ����Ƶ��Դ
	bool Open(const std::string& file);

	//�򿪶�����ƵԴ
	bool Open2(const std::string& file);

	//���ص�ǰ����λ��
	double GetPos();

	//�߳���ں���
	void run() override;

	//��ת��Ƶ frame֡λ��
	bool Seek(int frame);
	bool Seek(double pos);

	//��ʼ������Ƶ
	bool StartSave(const std::string filename, int width=0, int height=0,bool isColor=true);
	//ֹͣ������Ƶ��д����Ƶ֡������
	void StopSave();

	//���ˮӡ
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
	//��ʾԭ��Ƶ1ͼ��
	void ViewImage1(cv::Mat mat);
	//��ʾԭ��Ƶ2ͼ��
	void ViewImage2(cv::Mat mat);
	//��ʾ���ͼ��
	void ViewDst(cv::Mat mat);
	//�������
	void SaveEnd();
	

private:

	//��һ����ƵԴ�ļ�
	CcVideoThread();
	//�Ƿ�ʼд��Ƶ
	bool isWrite = false;
	//�Ƿ񲥷�
	bool isPlay = true;
	cv::Mat _mark;
	std::mutex _mutex;
	std::condition_variable _cond;


};

