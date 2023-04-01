#include "navigation.h"
#include "dijsktra.h"
#include "setlabeltext.h"
#include "getpath.h"
#include "adddatatime.h"
#include "txtadd.h"
#include <QPixmap>
#include <QPushButton>
#include <QPainter>
#include <QDebug>
#include <QPen>
#include <QMessageBox>
#include <QLabel>
#include <QBrush>
#include <QLineEdit>
#include <QCompleter>
#include <QStringList>
#include <bits/stdc++.h>
#define N 200
#define WordSize 15
#define Size 1000000
#define PushButtonWidth 100
#define PushButtonHeight 50
extern QString MapPath;
extern QString EdgePath;
extern QString JournalPath;
extern QString GetInformation(QString Path);
extern QString MapFirstLine;
extern QString EdgeFirstLine;
extern QString User;
extern int PrePosition[N];
extern double MinDist;
int TotArea=0;
int WayFlag=0;
int StringHash[Size];
const int Mod = 999983;
struct Position Area[N];
struct Edge *Node[N];
QLineEdit *LineEditFrom;
QLineEdit *LineEditEnd;
QLabel *LabelInformation;
void SetPositionInformation(Position *now,QString data,int flag)
{
    if(flag==0)
        now->Name=data;
    if(flag==1)
        now->x=data.toInt();
    if(flag==2)
        now->y=data.toInt();
    return ;
}
void SetPosition(QString data)
{
    Position now;
    int flag=0;
    QString s="";
    for(int pos=0;pos<data.size();pos++)
    {
        if(data[pos]=='\t'||data[pos]=='\n')
        {
            SetPositionInformation(&now,s,flag);
            flag++;
            s="";
        }
        else
            s=s+data[pos];
    }
    Area[++TotArea]=now;
    return ;
}
int GetHash(QString S)
{
    long long sco=1;
    for(int i=0;i<S.size();i++)
        sco=(sco * S[i].unicode()*13) % Mod;
    return sco;
}
inline int GetId(QString S)
{
    return StringHash[GetHash(S)];
}

navigation::navigation(QWidget *parent) : QWidget(parent)
{
    QLabel *LabelStart = new QLabel(this);
    LabelStart->setFont(QFont("宋体",WordSize));
    LabelStart->setText("起点：");
    LabelStart->move(540,155);

    QLabel *LabelEnd = new QLabel(this);
    LabelEnd->setFont(QFont("宋体",WordSize));
    LabelEnd->setText("终点：");
    LabelEnd->move(540,255);

    LabelInformation = new QLabel(this);
    LabelInformation->resize(500,300);
    LabelInformation->move(500,400);
    LabelInformation->setStyleSheet("QLabel{border:2px solid rgb(0, 0, 0);}");
    QString Data=GetInformation(MapPath);
    QString line="";
    TotArea = 0;
    for(int pos=0;pos<Data.size();pos++)
    {
        line=line+Data[pos];
        if(Data[pos]=='\n')
        {
            if(line!=MapFirstLine)
            {
                SetPosition(line);
            }
            line="";
        }
    }
    this->setFixedSize(1000,700);
    QPushButton *PushButtonShort = new QPushButton("最短距离",this);
    PushButtonShort->resize(PushButtonWidth,PushButtonHeight);
    PushButtonShort->move(700,350);

    connect(PushButtonShort,&QPushButton::clicked,[=](){
        QString from = LineEditFrom->text();
        QString end = LineEditEnd->text();
        if(from == end)
        {
            QMessageBox::critical(this, tr("错误！"),  tr("起点与终点不能相同！"));
            WayFlag = 0;
        }
        else
            if(!GetId(from)||!GetId(end))
            {
                QMessageBox::critical(this, tr("错误！"),  tr("起点或终点不存在！"));
                WayFlag = 0;
            }
        else
            WayFlag = 1;
        this->update();
    });

    LineEditFrom = new QLineEdit(this);
    LineEditFrom->resize(200,30);
    LineEditFrom->move(600,150);
    LineEditEnd = new QLineEdit(this);
    LineEditEnd->resize(200,30);
    LineEditEnd->move(600,250);

    QStringList PositionList;
    for(int i=1;i<=TotArea;i++)
    {
        PositionList<<Area[i].Name;
    }
    QCompleter *PositionCompleter = new QCompleter(PositionList);
    LineEditFrom->setCompleter(PositionCompleter);
    LineEditEnd->setCompleter(PositionCompleter);


}




void Connect(QString Start,QString End,int Congestion)
{
    int St=GetId(Start),Ed=GetId(End);
    //qDebug()<<St<<" "<<Ed;
    Edge *temp = (Edge *)malloc(sizeof(Edge));
    temp->Congestion=Congestion;
    temp->next=Node[St]->next;
    temp->Next_id=Ed;
    temp->From=St;
    Node[St]->next=temp;
    return ;
}
void SetEdge(QString data)
{
    QString Start="";
    QString End="";
    QString Congestion="";
    int flag=0;
    for(int pos=0;pos<data.size();pos++)
    {
        if(data[pos]=='\t'||data[pos]=='\n')
        {
            flag++;
        }
        else
        {
            switch (flag) {
            case 0:
                Start=Start+data[pos];
                break;
            case 1:
                End=End+data[pos];
                break;
            case 2:
                Congestion=Congestion+data[pos];
                break;
            }
        }
    }
    //qDebug()<<Start<<" "<<End<<"}}";
    Connect(Start,End,Congestion.toInt());
    Connect(End,Start,Congestion.toInt());
    return ;
}
void navigation::paintEvent(QPaintEvent *event)
{
    QPainter *painter = new QPainter(this);
    QPixmap pix;
    pix.load(":/image/地图.png");
    pix.scaled(this->width(),this->height());
    painter->drawPixmap(0,0,500,this->height(),pix);


    //painter->setPen(QPen(Qt::red,4)); //取消外围黑圈
    painter->setBrush(QColor(255,0,0));
    for(int i=1;i<=TotArea;i++)
    {
        painter->drawEllipse(Area[i].x-5,Area[i].y-5,10,10);
        StringHash[GetHash(Area[i].Name)]=i;
        if(Node[i]==NULL)
            Node[i] = (Edge *)malloc(sizeof(Edge));
        Node[i]->next=NULL;
        //PositionList<<Area[i].Name;
        //qDebug()<<Area[i].Name<<" "<<Area[i].x<<" "<<Area[i].y<<endl;
    }
    QPen Pen(Qt::red,3,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin);
    painter->setPen(Pen);
    if(WayFlag==0)
        return ;
    QString Data=GetInformation(EdgePath);
    QString line="";
    for(int pos=0;pos<Data.size();pos++)
    {
        line=line+Data[pos];
        if(Data[pos]=='\n')
        {
            if(line!=EdgeFirstLine)
            {
                SetEdge(line);
            }
            line="";
        }
    }
    if(WayFlag==1)
    {
        QString from = LineEditFrom->text();
        QString end = LineEditEnd->text();
        FindShortest(GetId(from),GetId(end));
        int PaintNow=GetId(end);
        QString Pass = GetPath(PrePosition,PaintNow);
        TxtAdd(JournalPath,AddDataTime("用户 "+User+" 进行了从 "+from+" 到 "+end+" 的最短距离导航\n"
                                       +Pass));
        SetLabelText(LabelInformation,"导航开始！\n"+Pass);
        while (PrePosition[PaintNow]) {
            painter->drawLine(Area[PaintNow].x,Area[PaintNow].y,
                              Area[PrePosition[PaintNow]].x,Area[PrePosition[PaintNow]].y);
            PaintNow = PrePosition[PaintNow];
        }
        WayFlag = 0;
    }
}
