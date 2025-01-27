#pragma once
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
class CcImagePro
{
public:
	//����ԭͼ������������
	void Set(const cv::Mat& mat1,const cv::Mat& mat2);
	//��ȡ�����Ľ��
	cv::Mat Get() { return dst; }
	//�������ȺͶԱȶ� 
	//���� 0~100 �Աȶ� 1.0~3.0
	void Gain(double bright, double contrast);

	//��ת
	void Rotate90();
	void Rotate180();
	void Rotate270();

	//����
	void FlipX();
	void FlipY();
	void FlipXY();

	//ͼ��ߴ�
	void Resize(int width, int height);

	//ͼ�������
	void PyDown(int count);
	void PyUp(int count);

	//��Ƶ����ü�
	void Clip(int x, int y, int width, int height);

	//תΪ�Ҷ�ͼ
	void Gray();

	//ˮӡ
	void Mark(int x, int y, double a);

	//�ں�
	void Blend(double a);

	CcImagePro();
	~CcImagePro();

private:
	cv::Mat src1, src2;

	cv::Mat dst;
};

