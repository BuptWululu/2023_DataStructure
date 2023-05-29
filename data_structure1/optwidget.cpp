#include "optwidget.h"
#include "adddatatime.h"
#include <QPushButton>
#include <QDebug>
#include <QTimer>
extern QTimer *Tim;
extern QString JournalPath;
extern QString User;
OptWidget::OptWidget(QWidget *parent)
{
    this->resize(270,150);
    this->setWindowTitle("请选择");
    this->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);
    this->setWindowModality(Qt::ApplicationModal);
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

void OptWidget::closeEvent(QCloseEvent *event)
{
    Tim->start();
}
