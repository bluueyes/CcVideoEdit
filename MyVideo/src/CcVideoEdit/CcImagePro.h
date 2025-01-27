#pragma once
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
class CcImagePro
{
public:
	//设置原图，会清理处理结果
	void Set(const cv::Mat& mat1,const cv::Mat& mat2);
	//获取处理后的结果
	cv::Mat Get() { return dst; }
	//设置亮度和对比度 
	//亮度 0~100 对比度 1.0~3.0
	void Gain(double bright, double contrast);

	//旋转
	void Rotate90();
	void Rotate180();
	void Rotate270();

	//镜像
	void FlipX();
	void FlipY();
	void FlipXY();

	//图像尺寸
	void Resize(int width, int height);

	//图像金字塔
	void PyDown(int count);
	void PyUp(int count);

	//视频画面裁剪
	void Clip(int x, int y, int width, int height);

	//转为灰度图
	void Gray();

	//水印
	void Mark(int x, int y, double a);

	//融合
	void Blend(double a);

	CcImagePro();
	~CcImagePro();

private:
	cv::Mat src1, src2;

	cv::Mat dst;
};

