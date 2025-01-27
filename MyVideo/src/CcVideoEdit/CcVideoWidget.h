#pragma once
#include <QOpenGLWidget>
#include <opencv2\core.hpp>
#include <QImage>

class CcVideoWidget:public QOpenGLWidget
{
	Q_OBJECT
public:
	CcVideoWidget(QWidget* parent);
	void paintEvent(QPaintEvent* e);
	virtual ~CcVideoWidget();

public slots:
	void SetImage(cv::Mat mat);
protected:
	QImage img;

};

