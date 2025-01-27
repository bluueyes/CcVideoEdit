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
#include "CcFilter.h"

using namespace cv;
static bool pressSlider = false;
static bool isExport = false;
static bool isColor = true;
static bool isMark = false;
static bool isBlend = false;

CcVideoUI::CcVideoUI(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    setWindowFlag (Qt::FramelessWindowHint);
    qRegisterMetaType<cv::Mat>("cv::Mat");
    //源视频显示信号
    QObject::connect(CcVideoThread::GetInstance(), &CcVideoThread::ViewImage1,
        ui.src1, &CcVideoWidget::SetImage);
    //输出视频显示信号
    QObject::connect(CcVideoThread::GetInstance(), &CcVideoThread::ViewDst,
        ui.dst, &CcVideoWidget::SetImage);
    //导出视频结束
    QObject::connect(CcVideoThread::GetInstance(), &CcVideoThread::SaveEnd,
        this, &CcVideoUI::ExportEnd);
    //源视频w2显示信号
    QObject::connect(CcVideoThread::GetInstance(), &CcVideoThread::ViewImage2,
        ui.src2, &CcVideoWidget::SetImage);
    startTimer(40);
    Pause();
}

CcVideoUI::~CcVideoUI()
{
}

void CcVideoUI::timerEvent(QTimerEvent * e)
{
    if (pressSlider) return;
    double pos=CcVideoThread::GetInstance()->GetPos();
    ui.playSlide->setValue(pos * 1000);
}

void CcVideoUI::SliderPress()
{
    pressSlider = true;
}

void CcVideoUI::SliderRelease()
{
    pressSlider = false;
}

void CcVideoUI::SetPos(int pos)
{
    if (pos < 10) return;
    CcVideoThread::GetInstance()->Seek((double)pos / 1000.);

}

void CcVideoUI::Set()
{
    CcFilter::Get()->Clear();
    isColor = true;

    //视频图像的裁剪
    bool isClip = false;
    double cx = ui.clip_x->value();
    double cy = ui.clip_y->value();
    double ch = ui.clip_h->value();
    double cw = ui.clip_w->value();
    if (cx + cy + cw + ch > 0.001) {
        isClip = true;
        CcFilter::Get()->Add(Task{ TASK_CLIP,{cx,cy,cw,ch} });
        double w = CcVideoThread::GetInstance()->width;
        double h = CcVideoThread::GetInstance()->height;
        CcFilter::Get()->Add(Task{ TASK_RESIZE,{w,h} });

    }

    //图像金字塔
    bool isPy = false;
    int down = ui.pydown_box->value();
    int up = ui.pyup_box->value();
    if (down > 0) {
        isPy = true;
        CcFilter::Get()->Add(Task{ TASK_PYDOWN,{(double)down} });
        int w=CcVideoThread::GetInstance()->width;
        int h = CcVideoThread::GetInstance()->height;
        for (int i = 0; i < down; i++) {
            w /= 2;
            h /= 2;
        }
        ui.width_box->setValue(w);
        ui.height_box->setValue(h);
    }

    if (up > 0) {
        isPy = true;
        CcFilter::Get()->Add(Task{ TASK_PYUP,{(double)up} });
        int w = CcVideoThread::GetInstance()->width;
        int h = CcVideoThread::GetInstance()->height;
        for (int i = 0; i < up; i++) {
            w *= 2;
            h *= 2;
        }
        ui.width_box->setValue(w);
        ui.height_box->setValue(h);
    }


    //调整图像尺寸大小
    double w = ui.width_box->value();
    double h = ui.height_box->value();
    if (!isClip && !isPy && ui.width_box->value() > 0 && ui.height_box->value() > 0) {
        CcFilter::Get()->Add(Task{ TASK_RESIZE,{w,h} });
    }

    //对比度和亮度
    if (ui.bright->value() > 0 || ui.contrast->value() > 1) {
        CcFilter::Get()->Add(Task{ TASK_GAIN,{(double)ui.bright->value(),ui.contrast->value()} });
    }

    //灰度图
    if (ui.color_btn->currentIndex() == 1) {
        CcFilter::Get()->Add(Task{ TASK_GRAY });
        isColor = false;
    }

    //图像旋转 1:90 2:180 3:270
    if (ui.rotate_box->currentIndex() == 1) {
        CcFilter::Get()->Add(Task{ TASK_ROTATE90 });
    }
    else if (ui.rotate_box->currentIndex() == 2) {
        CcFilter::Get()->Add(Task{ TASK_ROTATE180 });
    }
    else if (ui.rotate_box->currentIndex() == 3) {
        CcFilter::Get()->Add(Task{ TASK_ROTATE270 });
    }
    
    //图像镜像
    if (ui.flip_box->currentIndex() == 1) {
        CcFilter::Get()->Add(Task{ TASK_FLIPX });
    }
    else if (ui.flip_box->currentIndex() == 2) {
        CcFilter::Get()->Add(Task{ TASK_FLIPY });
    }
    else if (ui.flip_box->currentIndex() == 3) {
        CcFilter::Get()->Add(Task{ TASK_FLIPXY });
    }

    //水印
    if (isMark) {
        double x = ui.mark_x->value();
        double y = ui.mark_y->value();
        double a = ui.mark_a->value();
        CcFilter::Get()->Add(Task{ TASK_MARK ,{x,y,a} });
        
    }

    if (isBlend) {
        double a = ui.blend_box->value();
        CcFilter::Get()->Add(Task{ TASK_BLEND ,{a} });

    }

}

void CcVideoUI::Export()
{

    if (isExport) {
        //停止导出
        CcVideoThread::GetInstance()->StopSave();
        isExport = false;
        ui.Export_btn->setText("Export");
        return;
    }
    QString name = QFileDialog::getSaveFileName(this, "Save", "out1.avi");
    if (name.isEmpty()) return;
    std::string filename = name.toLocal8Bit().data();
    int w = ui.width_box->value();
    int h = ui.height_box->value();
    if (CcVideoThread::GetInstance()->StartSave(filename,w,h,isColor)) {
        isExport = true;
        ui.Export_btn->setText("Stop");

    }
}

void CcVideoUI::ExportEnd()
{
    isExport = false;
    ui.Export_btn->setText("Export");
}

void CcVideoUI::Play()
{
    ui.Stop_btn->show();
    ui.Stop_btn->setGeometry(ui.Play_btn->geometry());
   CcVideoThread::GetInstance()->Play();
    ui.Play_btn->hide();
}

void CcVideoUI::Pause()
{
    ui.Play_btn->show();
    //ui.Stop_btn->setGeometry(ui.Play_btn->geometry());
    ui.Stop_btn->hide();
    CcVideoThread::GetInstance()->Pause();
}

void CcVideoUI::Mark()
{
    isMark = false;
    QString name = QFileDialog::getOpenFileName(this, "select picture");
    if (name.isEmpty()) {
        std::cout << "name empty" << std::endl;
        return;
    }
    std::string file = name.toLocal8Bit().data();
    cv::Mat mark = cv::imread(file);
    if (mark.empty()) {
        std::cout << "mark empty" << std::endl;
        return;
    }
    CcVideoThread::GetInstance()->SetMark(mark);
    isMark = true;
}

void CcVideoUI::Blend()
{
    isMark = false;
    isBlend = false;
    QString name = QFileDialog::getOpenFileName(this, "select video");
    if (name.isEmpty()) {
        std::cout << "name empty" << std::endl;
        return;
    }
    std::string file = name.toLocal8Bit().data();
    isBlend=CcVideoThread::GetInstance()->Open2(file);
}


void CcVideoUI::Open() {
    
    QString name=QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("请选择视频文件"));
    if (name.isEmpty()) return;
    std::string file=name.toLocal8Bit().data();
    
    if (!CcVideoThread::GetInstance()->Open(file)) {
        QMessageBox::information(this, "error", name+" open filed");
    }
    Play();
    //QMessageBox::information(this, "", name);
}