#include "CcVideoUI.h"
#include <QFileDialog>
#include <string>
#include <iostream>
#include <QMessageBox>
#include "CcVideoThread.h"
#include <opencv2\imgcodecs.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\opencv.hpp>
CcVideoUI::CcVideoUI(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    setWindowFlag (Qt::FramelessWindowHint);

}

CcVideoUI::~CcVideoUI()
{}


void CcVideoUI::Open() {
    
    QString name=QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("请选择视频文件"));
    if (name.isEmpty()) return;
    std::string file=name.toLocal8Bit().data();
    
    if (!CcVideoThread::GetInstance()->Open(file)) {
        QMessageBox::information(this, "error", name+" open filed");
    }
    //QMessageBox::information(this, "", name);
}