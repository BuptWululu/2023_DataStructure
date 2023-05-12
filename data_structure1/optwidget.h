#ifndef OPTWIDGET_H
#define OPTWIDGET_H

#include <QWidget>
#include <QPushButton>

class OptWidget : public QWidget
{
    Q_OBJECT
public:
    QPushButton *Delete = new QPushButton(this),*Alarm = new QPushButton(this);
    int ChosenRow,ChosenColumn;
    OptWidget(QWidget *parent = nullptr);
    void PushButtonInit(QPushButton *P,QString Text,int MoveWidth,int MoveHeigth,int SizeWidth = 100,int SizeHeigth = 40);
signals:
    void ItemDelete();
    void SetAlarm();
public slots:
};

#endif // OPTWIDGET_H
