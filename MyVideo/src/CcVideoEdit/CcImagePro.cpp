#include "CcImagePro.h"
using namespace cv;

void CcImagePro::Set(const cv::Mat& mat1,const cv::Mat& mat2)
{
	if (mat1.empty()) return;
	this->src1 = mat1;
	this->src2 = mat2;
	this->src1.copyTo(dst);

}

void CcImagePro::Gain(double bright, double contrast)
{
	if (dst.empty()) return;
	dst.convertTo(dst, -1, contrast, bright);
}

void CcImagePro::Rotate90()
{
	if (dst.empty()) return;
	rotate(dst, dst, ROTATE_90_CLOCKWISE);

}

void CcImagePro::Rotate180()
{
	if (dst.empty()) return;
	rotate(dst, dst, ROTATE_180);
}

void CcImagePro::Rotate270()
{
	if (dst.empty()) return;
	rotate(dst, dst, ROTATE_90_COUNTERCLOCKWISE);
}

void CcImagePro::FlipX()
{
	if (dst.empty()) return;
	flip(dst, dst, 1);

}

void CcImagePro::FlipY()
{
	if (dst.empty()) return;
	flip(dst, dst, 0);

}

void CcImagePro::FlipXY()
{
	if (dst.empty()) return;
	flip(dst, dst, -1);


}
 
void CcImagePro::Resize(int width, int height)
{
	if (dst.empty()) return;
	resize(dst, dst, Size(width, height));
}

void CcImagePro::PyDown(int count)
{
	if (dst.empty()) return;
	for (int i = 0; i < count; i++) {
		pyrDown(dst, dst);
	}
}

void CcImagePro::PyUp(int count)
{
	if (dst.empty()) return;
	for (int i = 0; i < count; i++) {
		pyrUp(dst, dst);
	}
}

void CcImagePro::Clip(int x, int y, int width, int height)
{
	if (dst.empty()) return;
	if (x < 0 || y < 0 || width <= 0 || height <= 0) return;
	if (x > dst.cols || y > dst.rows) return;

	dst = dst(Rect(x, y, width, height));
}

void CcImagePro::Gray()
{
	if (dst.empty()) return;
	cvtColor(dst, dst, COLOR_BGR2GRAY);

}

void CcImagePro::Mark(int x, int y, double a)
{
	if (dst.empty()) return;
	if (src2.empty()) return;
	Mat rol = dst(Rect(x, y, this->src2.cols, this->src2.rows));
	addWeighted(src2, a, rol, 1 - a,0,rol);
}

void CcImagePro::Blend(double a)
{
	if (dst.empty()) return;
	if (src2.empty()) return;
	if (src2.size() != dst.size()) {
		resize(src2, src2, dst.size());
	}
	addWeighted(src2, a, dst, 1 - a, 0, dst);
}

void CcImagePro::Merge()
{
	if (dst.empty()) return;
	if (src2.empty()) return;
	if (src2.size() != dst.size()) {
		int w = src2.cols * (double)src2.rows / (double)dst.rows;
		resize(src2, src2, Size(w,dst.rows));
	}
	int dw = dst.cols + src2.cols;
	int dh = dst.rows;
	dst = Mat(Size(dw, dh), dst.type());
	Mat r1 = dst(Rect(0, 0, src1.cols, dh));
	Mat r2 = dst(Rect(src1.cols, 0, src2.cols, dh));
	src1.copyTo(r1);
	src2.copyTo(r2);

}

CcImagePro::CcImagePro()
{
}

CcImagePro::~CcImagePro()
{
}

