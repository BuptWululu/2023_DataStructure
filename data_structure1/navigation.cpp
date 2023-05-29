#include "navigation.h"
#include "dijsktra.h"
#include "setlabeltext.h"
#include "getpath.h"
#include "adddatatime.h"
#include "txtadd.h"
#include "adddatatime.h"
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
#include <QTimer>
#define N 200
#define WordSize 15
#define Size 1000000
#define PushButtonWidth 100
#define PushButtonHeight 50
extern QTimer *Tim;
extern QString MapPath;
extern QString EdgePath;
extern QString JournalPath;
extern QString GetInformation(QString Path);
extern QString MapFirstLine;
extern QString EdgeFirstLine;
extern QString User;
extern QString UserPath,UserCurriculumPath,UserExtracurricularPath,UserTemporaryPath,UserAlarmPath;
extern int PrePosition[N];
extern double MinDist;
struct WeekToDay{
    int Year,Month,Day;
};
extern WeekToDay GetWeekToDay(int Week,int WeekDay);
extern int GetDateDist(WeekToDay X,WeekToDay Y);
extern struct SerTime{
    int year=2023,month=2,day=20,hour,minute,second,millisecond;
}*ServerTime;
int TotArea=0;
int StringHash[Size];
const int Mod = 999983;
struct Position Area[N];
struct Edge *Node[N];
void navigation::SetPositionInformation(Position *now,QString data,int flag)
{
    if(flag==0)
        now->Name=data;
    if(flag==1)
        now->x=data.toInt();
    if(flag==2)
        now->y=data.toInt();
    return ;
}
void navigation::SetPosition(QString data)
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
int navigation::GetHash(QString S)
{
    long long sco=1;
    for(int i=0;i<S.size();i++)
        sco=(sco * S[i].unicode()*13) % Mod;
    return sco;
}
inline int navigation::GetId(QString S)
{
    return StringHash[GetHash(S)];
}

bool navigation::TimeOut(QStringList DataList)
{
    int Tem[5]={ServerTime->year,ServerTime->month,ServerTime->day,ServerTime->hour,ServerTime->minute};
    for(int i = 0;i < 3;i++)
    {
        if(Tem[i] < DataList[i].toInt())
            return 0;
        if(Tem[i] > DataList[i].toInt())
            return 1;

    }
    QStringList Time = DataList[3].split(":");
    for(int i = 3;i < 5;i++)
    {
        if(Tem[i] < Time[i - 3].toInt())
            return 0;
        if(Tem[i] > Time[i - 3].toInt())
            return 1;
    }
    return 0;
}

void navigation::FindAimPosition(int *T, QStringList *Data,QString position)
{
    int MinFlag = 0;
    for(int i = 0;i < 3;i++)
    {
        if(MinFlag) break;
        if(T[i] > (*Data)[i].toInt())
            MinFlag = 1;
        if(T[i] < (*Data)[i].toInt())
            MinFlag = -1;
    }
    QStringList Time = (*Data)[3].split(":");
    for(int i = 3;i < 5;i++)
    {
        if(MinFlag) break;
        if(T[i] > Time[i - 3].toInt())
            MinFlag = 1;
        if(T[i] < Time[i - 3].toInt())
            MinFlag = -1;
    }
    if(MinFlag != 1)
        return;
    for(int i = 0;i < 3;i++)
        T[i] = (*Data)[i].toInt();
    for(int i = 3;i < 5;i++)
        T[i] = Time[i - 3].toInt();
    AimPosition = position;
}

void navigation::GetInformationList()
{
    GetOneInformationList(GetInformation(UserCurriculumPath));
    GetOneInformationList(GetInformation(UserExtracurricularPath));
    GetOneInformationList(GetInformation(UserTemporaryPath));
}

void navigation::GetOneInformationList(QString Data)
{
    QStringList DataList = Data.split("\n");
    for(int i = 0;i < DataList.size();i++)
    {
        if(DataList[i] == "") continue;
        bool Added = 0;
        QStringList Line = DataList[i].split(" ");
        for(int j = 0;j < InformationList.size();j++)
            if(InformationList[j] == Line[1])
            {
                Added = 1;
                break;
            }
        if(Added) continue;
        InformationList<<Line[1];
    }
}

navigation::navigation(QWidget *parent) : QWidget(parent)
{
    this->setWindowTitle("日程导航");
    this->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);
    this->setWindowModality(Qt::ApplicationModal);
    Tim->stop();
    LabelStart = new QLabel(this);
    LabelStart->setFont(QFont("宋体",WordSize));
    LabelStart->setText("信息：");
    LabelStart->move(540,155);

    LabelBegin = new QLabel(this);
    LabelBegin->setFont(QFont("宋体",WordSize));
    LabelBegin->setText("起点：");
    LabelBegin->move(540,205);

    LineEditBegin = new QLineEdit(this);
    LineEditBegin->move(600,200);
    LineEditBegin->resize(200,30);

    LineEditInformation = new QLineEdit(this);
    LineEditInformation->move(600,150);
    LineEditInformation->resize(200,30);

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
    PushButtonShort = new QPushButton("最短距离",this);
    PushButtonShort->resize(PushButtonWidth,PushButtonHeight);
    PushButtonShort->move(700,350);

    connect(PushButtonShort,SIGNAL(clicked(bool)),this,SLOT(InformationNavigation()));

    QStringList PositionList;
    for(int i=1;i<=TotArea;i++)
    {
        PositionList<<Area[i].Name;
    }
    PositionCompleter = new QCompleter(PositionList);
    LineEditBegin->setCompleter(PositionCompleter);
    GetInformationList();
    QCompleter *InformationCompleter = new QCompleter(InformationList);
    LineEditInformation->setCompleter(InformationCompleter);
}




void navigation::Connect(QString Start,QString End,int Congestion)
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
void navigation::SetEdge(QString data)
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
    Connect(Start,End,Congestion.toInt());
    Connect(End,Start,Congestion.toInt());
    return ;
}

void navigation::closeEvent(QCloseEvent *)
{
    Tim->start();
}

void navigation::InformationNavigation()
{
    if(!GetId(LineEditBegin->text()))
    {
        QMessageBox::critical(this,tr("错误！"),tr("起点不存在"));
        return ;
    }
    QString Information = LineEditInformation->text();
    bool In = 0;
    for(int i = 0;i < InformationList.size();i++)
    {
        if(Information == InformationList[i])
        {
            In = 1;
            break;
        }
    }
    if(!In)
    {
        QMessageBox::critical(this,tr("错误！"),tr("该日程信息不存在"));
        return ;
    }
    int MinTime[5] = {10000};
    QString Data = GetInformation(UserCurriculumPath) + GetInformation(UserExtracurricularPath) + GetInformation(UserTemporaryPath);
    QStringList DataList = Data.split("\n");
    AimPosition = "";
    int minDist = 1e9;
    for(int i = 0;i < DataList.size();i++)
    {
        if(DataList[i] == "") continue;
        QStringList Line = DataList[i].split(" ");
        if(Line[1] != LineEditInformation->text()) continue;
        QStringList SwitchControl;
        SwitchControl<<"Curriculum"<<"Extracurricular"<<"Temporary";
        QStringList TemList ;
        switch (SwitchControl.indexOf(Line[0])) {
        case 0:{
            WeekToDay X,Y;
            X.Year = ServerTime->year;
            X.Month = ServerTime->month;
            X.Day = ServerTime->day;
            for(int j = Line[2].toInt();j <= Line[3].toInt();j++)
            {
                Y = GetWeekToDay(j,Line[4].toInt());
                if(Y.Year < X.Year ||(Y.Year == X.Year&&Y.Month < X.Month)||(Y.Year == X.Year&&Y.Month == X.Month&&Y.Day < X.Day))
                    continue;
                int Dist = GetDateDist(X,Y);
                if(Dist > minDist) continue;
                minDist = Dist;
                TemList<<QString::number(Y.Year)<<QString::number(Y.Month)<<QString::number(Y.Day)<<Line[5];
                FindAimPosition(MinTime,&TemList,Line[Line.size() - 1]);
            }
            break;
        }
        case 1:
            for(int j = 3;j <= 6;j++)
                TemList<<Line[j];
            if(TimeOut(TemList)&&Line[7] != "每天"&&Line[7] != "每周")
                break;
            FindAimPosition(MinTime,&TemList,Line[Line.size() - 1]);
            break;
        case 2:
            for(int j = 2;j <= 5;j++)
                TemList<<Line[j];
            if(TimeOut(TemList))
                break;
            FindAimPosition(MinTime,&TemList,Line[Line.size() - 1]);
            break;
        }
    }
    if(minDist&&minDist != 1e9)
    {
        QString FindTip = "当天没有该日程信息，最近的该日程信息是在" +QString::number(minDist) + "天后，是否导航？";
        QMessageBox::StandardButton result = QMessageBox::question(this,tr("提示"),FindTip);
        switch (result) {
        case QMessageBox::No:
                AimPosition = "";
            break;
        default:
            break;
        }
    }
    if(LineEditBegin->text() == AimPosition)
    {
        QMessageBox::information(this,tr("提示"),tr("起点与最近的日程信息地点相同！"));
        return ;
    }
    if(AimPosition == ""&&minDist == 1e9)
    {
        QMessageBox::critical(this,tr("错误！"),tr("该日程信息已结束"));
        return ;
    }
    if(!GetId(AimPosition))
    {
        QMessageBox::critical(this,tr("错误！"),tr("终点不存在"));
        return ;
    }
    WayFlag = 1;
    this->update();
}
void navigation::paintEvent(QPaintEvent *)
{
    QPainter *painter = new QPainter(this);
    QPixmap pix;
    pix.load(":/image/地图.png");
    pix.scaled(this->width(),this->height());
    painter->drawPixmap(0,0,500,this->height(),pix);
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
    if(WayFlag == 0)
    {
        painter->end();
        return ;
    }
    FindShortest(GetId(LineEditBegin->text()),GetId(AimPosition));
    int PaintNow=GetId(AimPosition);
    QString Pass = GetPath(PrePosition,PaintNow);
    TxtAdd(JournalPath,AddDataTime("用户 "+User+" 进行了从 "+LineEditBegin->text()+" 到 "+AimPosition+" 的最短距离导航\n"),1);
    SetLabelText(LabelInformation,"导航开始！\n"+Pass + "\n到达目的地: " + AimPosition);
    while (PrePosition[PaintNow]) {
        painter->drawLine(Area[PaintNow].x,Area[PaintNow].y,
                          Area[PrePosition[PaintNow]].x,Area[PrePosition[PaintNow]].y);
        PaintNow = PrePosition[PaintNow];
    }
    painter->end();
    WayFlag = 0;
}
