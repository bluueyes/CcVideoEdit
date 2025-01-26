#pragma once

#include <QtWidgets/QWidget>
#include "ui_CcVideoUI.h"

class CcVideoUI : public QWidget
{
    Q_OBJECT

public:
    CcVideoUI(QWidget *parent = nullptr);
    ~CcVideoUI();

public slots:
    void Open();

private:
    Ui::CcVideoUIClass ui;
};
