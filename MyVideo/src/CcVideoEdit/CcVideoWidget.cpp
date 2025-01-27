#include "CcVideoWidget.h"
#include <QPainter>
#include <opencv2\imgproc.hpp>
using namespace cv;
CcVideoWidget::CcVideoWidget(QWidget* parent):QOpenGLWidget(parent)
{

}

void CcVideoWidget::paintEvent(QPaintEvent* e)
{
	QPainter p;
	p.begin(this);
	p.drawImage(QPoint(0, 0), img);
	p.end();
}

CcVideoWidget::~CcVideoWidget()
{
}

void CcVideoWidget::SetImage(cv::Mat mat) {
	QImage::Format fmt = QImage::Format_RGB888;
	int pixSize = 3;
	if (mat.type() == CV_8UC1) {
		fmt = QImage::Format_Grayscale8;
		pixSize = 1;
	}
	if (img.isNull() || img.format()!=fmt) {
		delete img.bits();
		uchar* buf = new uchar[width() * height() * pixSize];
		img = QImage(buf, width(), height(), fmt);
	}
	Mat dst;
	cv::resize(mat, dst, Size(img.size().width(),img.size().height()));
	if (pixSize > 1) {
		cv::cvtColor(dst, dst, COLOR_BGR2RGB);
	}
	memcpy(img.bits(), dst.data, dst.cols * dst.rows * dst.elemSize());
	update();
}