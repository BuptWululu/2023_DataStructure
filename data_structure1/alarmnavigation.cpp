#include "alarmnavigation.h"
#include "login.h"
#include "adddatatime.h"
#include <QDebug>
#include <QLineEdit>
#include <QLabel>
#include <QMessageBox>
#include <QPainter>
#include <QScrollArea>
#include <QPixmap>
#include <vector>
#include <QTimer>
#define N 200
#define AlarmSize 1000
#define Size 1000000
extern QTimer *Tim;
extern struct Position Area[N];
extern QString EdgeFirstLine;
extern QString UserAlarmPath;
extern QString EdgePath;
extern QString JournalPath;
extern QString User;
extern int StringHash[Size];
extern int PrePosition[N];
extern int TotArea;
extern struct Edge *Node[N];
extern bool CurriculumNavigation;
extern QString CurriculumNavigationPosition ;
bool VisRow[AlarmSize];
AlarmNavigation::AlarmNavigation(int preRow, int nowRow)
{
    PreRow = preRow;
    NowRow = nowRow;
    this->setWindowTitle("导航");
    Init();
    LineEditInit(LineEditStart,600,150);
}

void AlarmNavigation::Init()
{
    LabelStart->setText("起点：");
    LineEditInformation->hide();
    LineEditBegin->hide();
    LabelBegin->hide();
    PushButtonShort->setText("开始导航");
    PushButtonShort->disconnect();
    connect(PushButtonShort,&QPushButton::clicked,[=](){
        QString Start = LineEditStart->text();
        if(!GetId(Start))
        {
            QMessageBox::critical(this, tr("错误！"),  tr("起点不存在！"));
            WayFlag = 0;
        }
        else
            WayFlag = 1;
        this->update();
    });
}

void AlarmNavigation::LineEditInit(QLineEdit *L, int MoveWidth, int MoveHeight, int SizeWidth, int SizeHeight)
{
    L->move(MoveWidth,MoveHeight);
    L->resize(SizeWidth,SizeHeight);
    L->setCompleter(PositionCompleter);
}

void AlarmNavigation::closeEvent(QCloseEvent *event)
{
    return ;
}

void AlarmNavigation::paintEvent(QPaintEvent *event)
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
    }
    QPen Pen(Qt::red,3,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin);
    painter->setPen(Pen);
    if(WayFlag==0)
    {
        painter->end();
        return ;
    }
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
        WayFlag = 0;
        qDebug()<<"开始绘图";
        if(CurriculumNavigation)
        {
            FindShortest(GetId(LineEditStart->text()),GetId(CurriculumNavigationPosition));
            int PaintNow=GetId(CurriculumNavigationPosition);
            QString Pass = GetPath(PrePosition,PaintNow);
            while (PrePosition[PaintNow]) {
                painter->drawLine(Area[PaintNow].x,Area[PaintNow].y,
                                  Area[PrePosition[PaintNow]].x,Area[PrePosition[PaintNow]].y);
                PaintNow = PrePosition[PaintNow];
            }
            painter->end();

            QWidget *BigW = new QWidget(this);
            BigW->resize(500,300);
            BigW->move(500,400);
            QScrollArea *ScrollLine = new QScrollArea(BigW);
            ScrollLine->setGeometry(0,0,BigW->width(),BigW->height());
            QWidget *SmallW = new QWidget(BigW);
            QLabel *LabelText = new QLabel(SmallW);
            SetLabelText(LabelText,"导航开始！\n"+Pass + "\n到达目的地: " + CurriculumNavigationPosition);
            LabelText->adjustSize();
            ScrollLine->setWidget(SmallW);
            SmallW->setGeometry(0,0,LabelText->width(),LabelText->height());
            BigW->show();
            return ;
        }
        PassPositionList.clear();
        QStringList EmptyList;
        FindAllCombination(EmptyList,PreRow);
        FindAimPositionList();
        if(AimPositionList.size() == 1)
        {
            QMessageBox::information(this, tr("提示"),  tr("起点即可完成所有事务"));
            return ;
        }
        FindPassPosition();
        for(int i = 1;i < PassPositionId.size();i++)
        {
            painter->drawLine(Area[PassPositionId[i]].x,Area[PassPositionId[i]].y,
                              Area[PassPositionId[i - 1]].x,Area[PassPositionId[i - 1]].y);
        }
        painter->end();
        QWidget *BigW = new QWidget(this);
        BigW->resize(500,300);
        BigW->move(500,400);
        QScrollArea *ScrollLine = new QScrollArea(BigW);
        ScrollLine->setGeometry(0,0,BigW->width(),BigW->height());
        QWidget *SmallW = new QWidget(BigW);
        QLabel *LabelText = new QLabel(SmallW);
        SetLabelText(LabelText,"导航开始！\n"+Tip + "\n完成了所有事务并返回目的地");
        LabelText->adjustSize();

        ScrollLine->setWidget(SmallW);
        SmallW->setGeometry(0,0,LabelText->width(),LabelText->height());
        BigW->show();
    }
}

void AlarmNavigation::FindAllCombination(QStringList PositionList, int Row)
{
    if(Row == NowRow + 1)
    {
        for(int i = 0;i < PositionList.size();i++)
        {
            if(!GetId(PositionList[i]))
            {
                PositionList.removeAt(i);
                i--;
                continue ;
            }
            if(i != 0)
            {
                if(PositionList[i] == PositionList[i-1])
                {
                    PositionList.removeAt(i);
                    i--;
                }
            }
        }
        if(PositionList[PositionList.size() - 1] != LineEditStart->text())
            PositionList<<LineEditStart->text();
        if(PositionList[0] != LineEditStart->text())
            PositionList.insert(0,LineEditStart->text());
        PassPositionList.push_back(PositionList);
        return ;
    }
    QString Data = GetInformation(UserAlarmPath);
    QStringList DataList = Data.split("\n");
    for(int i = PreRow; i <= NowRow ;i++)
    {
        if(VisRow[i] == 1) continue;
        QStringList Line = DataList[i].split(" ");
        VisRow[i] = 1;
        QStringList NextList = PositionList;
        NextList<<Line[Line.size() - 1];
        FindAllCombination(NextList , Row + 1);
        VisRow[i] = 0;
    }

}

void AlarmNavigation::FindAimPositionList()
{
    double MinDist = 1e9;
    for(int i = 0;i < (int)PassPositionList.size();i++)
    {
        QStringList Line = PassPositionList[i];
        double NowDist = 0;
        for(int j = 1;j < Line.size(); j++)
        {
            NowDist += FindShortest(GetId(Line[j - 1]),GetId(Line[j]));
        }
        if(MinDist > NowDist)
        {
            MinDist = NowDist;
            AimPositionList = Line ;
        }
    }
    return ;
}
void AlarmNavigation::FindPassPosition()
{
    PassPositionId.clear();
    Tip = "";
    for(int i = 1;i < (int)AimPositionList.size();i++)
    {
        FindShortest(GetId(AimPositionList[i - 1]),GetId(AimPositionList[i]));
        Recursion(GetId(AimPositionList[i]));
        int BeginPara = 0;
        bool EndlFlag = 0;
        for(int j = 0;j < Tip.size();j++)
            if(Tip[j] == '\n'&&EndlFlag == 0)
                BeginPara ++,EndlFlag = 1;
            else
                EndlFlag = 0;
        Tip = Tip + GetPath(PrePosition,GetId(AimPositionList[i]),BeginPara) + '\n';
    }

}

void AlarmNavigation::Recursion(int Now)
{
    if(PrePosition[Now] == 0)
    {
        PassPositionId.push_back(Now);
        return ;
    }
    Recursion(PrePosition[Now]);
    PassPositionId.push_back(Now);
    return ;
}
