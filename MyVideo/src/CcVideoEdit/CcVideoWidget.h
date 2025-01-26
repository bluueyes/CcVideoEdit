#pragma once
#include <QOpenGLWidget>

class CcVideoWidget:public QOpenGLWidget
{
public:
	CcVideoWidget(QWidget* parent);
	void paintEvent(QPaintEvent* e);
	virtual ~CcVideoWidget();

};

