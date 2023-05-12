#include "optwidget.h"
#include <QPushButton>
#include <QDebug>

OptWidget::OptWidget(QWidget *parent)
{
    this->resize(270,150);

    PushButtonInit(Delete,"删除该项",20,60);
    PushButtonInit(Alarm,"设置闹钟",150,60);
    connect(Delete,&QPushButton::clicked,[=](){
        emit this->ItemDelete();
        this->hide();
    });
    connect(Alarm,&QPushButton::clicked,[=](){
        emit this->SetAlarm();
        this->hide();
    });
}

void OptWidget::PushButtonInit(QPushButton *P,QString Text, int MoveWidth, int MoveHeigth,int SizeWidth, int SizeHeigth)
{
    P->setText(Text);
    P->setStyleSheet("font-size: 18px");
    P->move(MoveWidth,MoveHeigth);
    P->resize(SizeWidth,SizeHeigth);
}
