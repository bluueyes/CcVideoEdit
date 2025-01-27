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
    //�������϶�
    void SetPos(int);
    //���ù�����
    void Set();
    //������Ƶ
    void Export();
    //��������
    void ExportEnd();
    //����
    void Play();
    //��ͣ
    void Pause();
    //ˮӡ
    void Mark();    
    //�ں�
    void Blend();

private:
    Ui::CcVideoUIClass ui;
};
