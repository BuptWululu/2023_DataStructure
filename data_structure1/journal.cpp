#include "journal.h"
#include "login.h"
#include "setlabeltext.h"
#include "adddatatime.h"
#include <QString>
#include <QDebug>
#include <QLabel>
#include <QWidget>
#include <QScrollArea>
#include <QTimer>
extern QString JournalPath;
extern QTimer *Tim;
Journal::Journal(QWidget *parent) : QWidget(parent)
{
    this->setWindowTitle("日志");
    this->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);
    this->setWindowModality(Qt::ApplicationModal);
    Tim->stop();
    this->setFixedSize(500,700);
    this->setWindowFlags(Qt::WindowStaysOnTopHint);
    QScrollArea *ScrollLine = new QScrollArea(this);
    ScrollLine->setGeometry(0,0,this->width(),this->height());
    //ScrollLine->widgetResizable(true);

    QWidget *w = new QWidget(this);

    QString data = GetInformation(JournalPath);
    QLabel *LabelText = new QLabel(w);
    SetLabelText(LabelText,data);
    LabelText->adjustSize();

    ScrollLine->setWidget(w);
    w->setGeometry(0,0,LabelText->width(),LabelText->height());

    //w->adjustSize();

}

void Journal::closeEvent(QCloseEvent *event)
{
    Tim->start();
}
