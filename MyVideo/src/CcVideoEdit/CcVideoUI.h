#pragma once

#include <QtWidgets/QWidget>
#include "ui_CcVideoUI.h"

class CcVideoUI : public QWidget
{
    Q_OBJECT

public:
    CcVideoUI(QWidget *parent = nullptr);
    ~CcVideoUI();
    void timerEvent(QTimerEvent* e) override;

public slots:
    void Open();
    void SliderPress();
    void SliderRelease();
    //滑动条拖动
    void SetPos(int);
    //设置过滤器
    void Set();
    //导出视频
    void Export();
    //导出结束
    void ExportEnd();
    //播放
    void Play();
    //暂停
    void Pause();
    //水印
    void Mark();    
    //融合
    void Blend();

private:
    Ui::CcVideoUIClass ui;
};
