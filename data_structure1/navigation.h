#ifndef NAVIGATION_H
#define NAVIGATION_H

#include "navigation.h"
#include "dijsktra.h"
#include "setlabeltext.h"
#include "getpath.h"
#include "adddatatime.h"
#include "txtadd.h"
#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QCompleter>

struct Position{
    QString Name;
    int x,y;
};
struct Edge
{
    Edge *next;
    int From,Next_id,Congestion;
};
class navigation : public QWidget
{
    Q_OBJECT
public:
    explicit navigation(QWidget *parent = nullptr);
    QLineEdit *LineEditFrom;
    QLineEdit *LineEditEnd;
    QPushButton *PushButtonShort ;
    QLabel *LabelInformation;
    QLabel *LabelEnd;
    QLabel *LabelStart;
    QCompleter *PositionCompleter;
    int WayFlag=0;
    void paintEvent(QPaintEvent *event);
    void SetPositionInformation(Position *now,QString data,int flag);
    void SetPosition(QString data);
    int GetHash(QString S);
    int GetId(QString S);
    void Connect(QString Start,QString End,int Congestion);
    void SetEdge(QString data);
signals:
    void back_mainwindow();

public slots:
};

#endif // NAVIGATION_H
