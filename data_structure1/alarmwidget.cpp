#include "alarmwidget.h"
#include "alarmnavigation.h"
#include "txtadd.h"
#include "login.h"
#include "navigation.h"
#include "adddatatime.h"
#include <QDebug>
#include <QTableWidget>
#include <QToolTip>
#include <QMessageBox>
#include <QHeaderView>
#include <map>
#include <QTimer>
extern QTimer *Tim;
extern QString UserCurriculumPath,UserAlarmPath;
extern QString JournalPath;
extern QString User;
extern struct SerTime{
    int year=2023,month=2,day=20,hour,minute,second,millisecond;
}*ServerTime;
extern int BeginYear,BeginMonth,BeginDay;
extern int Month_Day[13];
int PreAlarmHour = -1;
struct WeekToDay{
    int Year,Month,Day;
};
bool CurriculumNavigation = 0;
QString CurriculumNavigationPosition ,CurriculumNavigationType;
extern WeekToDay GetWeekToDay(int Week,int WeekDay);
std::map<QTableWidgetItem *,QString> Tip;
AlarmWidget::AlarmWidget(int Width, int Height, QWidget *parent)
{ 
    if(ServerTime->hour == PreAlarmHour || ServerTime->minute != 0)
    {
        delete this;
        return ;
    }
    ClearEndl();
    this->setWindowTitle("闹钟提醒");
    this->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);
    this->setWindowModality(Qt::ApplicationModal);
    this->resize(Width,Height);
    QString Data = GetInformation(UserAlarmPath);
    DataList = Data.split("\n");
    NowItem = DataList[1].toInt();
    bool FindFlag = 0;
    if (NowItem <= AlarmRow()) {
        CurriculumNavigationPosition = "";
        CurriculumNavigation = 0;
        InsertNextPeriod();
        Table = new QTableWidget(this);
        Table->setRowCount(AlarmRow() - NowItem + 1);
        Table->setColumnCount(1);
        TableInit();
        FindFlag = 1;
        for(int i = NowItem; i <= AlarmRow() ;i++)
        {
            QStringList Line = DataList[i].split(" ");
            QTableWidgetItem *Item = new QTableWidgetItem;
            Item->setText(Line[1]);
            Table->setItem(i-NowItem,0,Item);
            Table->item(i-NowItem,0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            Tip[Item] = GetTip(Line);
            TxtAdd(JournalPath,AddDataTime("用户 " + User + " 设置的 " + Line[1] + " 闹钟响铃\n"),1);
        }
        Tim->stop();
        this->show();
    }
    if(FindFlag == 0)
    {
        QStringList LineList = GetInformation(UserCurriculumPath).split("\n");
        for(int i = 0;i < LineList.size();i++)
        {
            if(LineList[i] == "") continue;
            QStringList Line = LineList[i].split(" ");
            for(int j = Line[2].toInt();j <= Line[3].toInt();j++)
            {
                WeekToDay X = GetWeekToDay(j,Line[4].toInt());
                if(X.Year == ServerTime->year&&X.Month == ServerTime->month&&X.Day == ServerTime->day)
                {

                    QStringList Temp = Line[5].split(":");
                    if(Temp[0].toInt() - 1 == ServerTime->hour)
                    {
                        PreAlarmHour = ServerTime->hour;
                        Table = new QTableWidget(this);
                        Table->setRowCount(1);
                        Table->setColumnCount(1);
                        TableInit();
                        QTableWidgetItem *Item = new QTableWidgetItem;
                        Item->setText(Line[1]);
                        Table->setItem(0,0,Item);
                        Table->item(0,0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                        Tip[Item] = GetTip(Line);
                        FindFlag = 1;
                        CurriculumNavigation = 1;
                        CurriculumNavigationPosition = Line[8];
                        CurriculumNavigationType = Line[7];
                        this->show();
                        Tim->stop();
                        TxtAdd(JournalPath,AddDataTime("用户 " + User + " 的课程 " + Line[1] + " 临近提醒\n"),1);
                        break ;
                    }
                }
            }
            if(FindFlag) break;
        }
        if(!FindFlag)
        {
            delete this;
            return ;
        }
    }
    connect(Table,SIGNAL(itemClicked(QTableWidgetItem*)),this, SLOT(slotCellEnter(QTableWidgetItem *)));
    PushButtonInit(NavigationPushButton,"开始导航");
    connect(NavigationPushButton,SIGNAL(clicked(bool)),this,SLOT(slotNavigation()));
    TxtAdd(UserAlarmPath,QString::number(AlarmRow() + 1),1,1);
}

void AlarmWidget::TableInit()
{
    Table->verticalHeader()->setHidden(true);
    Table->resize(350,500);
    Table->setStyleSheet("font-size: 18px");
    Table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    Table->setColumnWidth(0,Table->width());
}

void AlarmWidget::InsertNextPeriod()
{
    QStringList Line = DataList[NowItem].split(" ");
    QString NewLine;
    if(Line[Line.size() - 3] == "每周")
    {
        NewLine = AddDay(Line,7);
    }
    if(Line[Line.size() - 3] == "每天")
    {
        NewLine = AddDay(Line,1);
    }
    for(int i = NowItem;i;i++)
    {
        if(DataList[i] == "")
        {
            TxtAdd(UserAlarmPath,NewLine,2,i);
            break ;
        }
        QStringList data1 = NewLine.split(" ");
        QStringList data2 = DataList[i].split(" ");
        if(CompareTime(data1,data2))
        {
            TxtAdd(UserAlarmPath,NewLine,2,i);
            break;
        }
    }
    return ;
}

void AlarmWidget::ClearEndl()
{
    QString Data = GetInformation(UserAlarmPath);
    QString ret = "";
    for(int i = 1;i < Data.size() - 1;i ++)
    {
        if(Data[i] == '\n'&&Data[i + 1] == '\n')
            continue;
        ret = ret + Data[i];
    }
    TxtAdd(UserAlarmPath,"\n" + ret + "\n",0);
}

int AlarmWidget::CompareTime(QStringList a, QStringList b)
{
    int T1[5],T2[5];
    QStringList HM;
    if(a[0] == "Extracurricular")
    {
        HM = a[6].split(":");
        for(int i = 3;i <= 5;i++)
            T1[i - 3] = a[i].toInt();
        T1[3] = HM[0].toInt();
        T1[4] = HM[1].toInt();
    }
    if(a[0] == "Temporary")
    {
        HM = a[5].split(":");
        for(int i = 2;i <= 4;i++)
            T1[i - 2] = a[i].toInt();
        T1[3] = HM[0].toInt();
        T1[4] = HM[1].toInt();
    }
    if(b[0] == "Extracurricular")
    {
        HM = b[6].split(":");
        for(int i = 3;i <= 5;i++)
            T2[i - 3] = b[i].toInt();
        T2[3] = HM[0].toInt();
        T2[4] = HM[1].toInt();
    }
    if(b[0] == "Temporary")
    {
        HM = b[5].split(":");
        for(int i = 2;i <= 4;i++)
            T2[i - 2] = b[i].toInt();
        T2[3] = HM[0].toInt();
        T2[4] = HM[1].toInt();
    }
    for(int i = 0;i < 5;i++)
    {
        if(T1[i] < T2[i])
            return 1;
        if(T1[i] > T2[i])
            return 0;
    }
    return 0;
}

QString AlarmWidget::AddDay(QStringList Now,int AddNumber)
{
    int Y,M,D,DatePos;
    if(Now[0] == "Extracurricular")
        Y = Now[3].toInt(),M = Now[4].toInt(),D = Now[5].toInt(),DatePos = 3;
    if(Now[0] == "Temporary")
        Y = Now[2].toInt(),M = Now[3].toInt(),D = Now[4].toInt(),DatePos = 2;
    while(AddNumber --)
    {
        if(M == 2&&D == 29)
        {
            M++;
            D = 1;
            continue;
        }
        if(Month_Day[M] == D)
        {
            if(M == 2 &&((Y%400 == 0)||(Y%4 == 0&&Y%100 != 0)))
            {
                D++;
                continue;
            }
            if(M == 12)
            {
                Y++,M = 1,D = 1;
                continue;
            }
            M++;
            D = 1;
            continue;
        }
        D++;
    }
    QString ret;
    for(int i = 0;i < Now.size();i++)
    {
        if(i >= DatePos && i <= DatePos + 2)
        {
            if(i == DatePos)
                ret = ret + QString::number(Y) + " ";
            if(i == DatePos + 1)
                ret = ret + QString::number(M) + " ";
            if(i == DatePos + 2)
                ret = ret + QString::number(D) + " ";
            continue;
        }
        if(i == Now.size() - 1)
            ret = ret + Now[i];
        else
            ret = ret + Now[i] + " ";
    }
    return ret;
}

void AlarmWidget::PushButtonInit(QPushButton *P, QString Text, int MoveWidth, int MoveHeight)
{
    P->setStyleSheet("font-size:20px");
    P->setText(Text);
    P->adjustSize();
    P->move(MoveWidth,MoveHeight);
}

bool AlarmWidget::FindAlarm(int row)
{
    QStringList Line = DataList[row].split(" ");
    QStringList SwitchControl;
    SwitchControl<<"Curriculum"<<"Extracurricular"<<"Temporary";
    switch (SwitchControl.indexOf(Line[0])) {
    case 1:{
        QStringList Time = Line[6].split(":");
        if(ServerTime->year > Line[3].toInt())
            return 1;
        if(ServerTime->year == Line[3].toInt()&&ServerTime->month > Line[4].toInt())
            return 1;
        if(ServerTime->year == Line[3].toInt()&&ServerTime->month == Line[4].toInt()&&ServerTime->day > Line[5].toInt())
            return 1;
        if(ServerTime->year == Line[3].toInt()&&ServerTime->month == Line[4].toInt()&&ServerTime->day == Line[5].toInt()&&ServerTime->hour > Time[0].toInt())
            return 1;
        if(ServerTime->year == Line[3].toInt()&&ServerTime->month == Line[4].toInt()&&ServerTime->day == Line[5].toInt()&&ServerTime->hour == Time[0].toInt()&&ServerTime->minute >= Time[1].toInt())
            return 1;
        break;}
    case 0:
    case 2:{
        QStringList Time = Line[5].split(":");
        if(ServerTime->year > Line[2].toInt())
            return 1;
        if(ServerTime->year == Line[2].toInt()&&ServerTime->month > Line[3].toInt())
            return 1;
        if(ServerTime->year == Line[2].toInt()&&ServerTime->month == Line[3].toInt()&&ServerTime->day > Line[4].toInt())
            return 1;
        if(ServerTime->year == Line[2].toInt()&&ServerTime->month == Line[3].toInt()&&ServerTime->day == Line[4].toInt()&&ServerTime->hour > Time[0].toInt())
            return 1;
        if(ServerTime->year == Line[2].toInt()&&ServerTime->month == Line[3].toInt()&&ServerTime->day == Line[4].toInt()&&ServerTime->hour == Time[0].toInt()&&ServerTime->minute >= Time[1].toInt())
            return 1;
        break;}
    default:
        break;
    }

    return 0;
}

QString AlarmWidget::GetTip(QStringList Line)
{
    QString ret ;
    if(Line[0] == "Extracurricular")
    {
        ret = "活动名称:" + Line[1] + "\n";
        ret = ret + "活动类型:" + Line[2] + "\n";
        ret = ret + "活动日期:" + Line[3] + "年 " + Line[4] + "月 " + Line[5] + "日\n";
        ret = ret + "开始时间:" + Line[6] + "\n";
        ret = ret + "活动频率:" + Line[7] + "\n";
        ret = ret + "活动方式:" + Line[8] + "\n";
        ret = ret + "活动地点/链接:" + Line[9] ;
    }
    if(Line[0] == "Temporary")
    {
        ret = "活动名称:" + Line[1] + "\n";
        ret = ret + "活动日期:" + Line[2] + "年 " + Line[3] + "月 " + Line[4] + "日\n";
        ret = ret + "开始时间:" + Line[5] + "\n";
        ret = ret + "活动地点:" + Line[6] ;
    }
    if(Line[0] == "Curriculum")
    {
        ret = "课程名称:" + Line[1] + "\n";
        ret = ret + "上课周次:" + Line[2] + "-" + Line[3] + "周\n";
        ret = ret + "上课时间:" + Line[5] + "-" + Line[6] + "\n";
        ret = ret + "上课方式:" + Line[7] + "\n";
        ret = ret + "上课地点/链接:" + Line[8] ;
    }
    return ret;
}

int AlarmWidget::AlarmRow()
{
    for(int i = NowItem;i;i++)
    {
        if(DataList[i] == "") return i - 1;
        if(FindAlarm(i) == 0) return i - 1;
    }
    return 0;
}

void AlarmWidget::closeEvent(QCloseEvent *event)
{
    Tim->start();
}

void AlarmWidget::slotCellEnter(QTableWidgetItem *Item)
{
    QToolTip::showText(QCursor::pos(),Tip[Item]);
}

void AlarmWidget::slotNavigation()
{
    QStringList Line = DataList[NowItem].split(" ");
    if(CurriculumNavigationPosition == "")
    {
        if(Line[Line.size() - 2] == "线上")
        {
          QMessageBox::information(this,tr("线上活动"),(const QString)("链接为:" + Line[Line.size() - 1]));
          return ;
        }
    }
    else
        if(CurriculumNavigationType == "线上上课")
        {
            QMessageBox::information(this,tr("线上课程"),(const QString)("链接为:" + CurriculumNavigationPosition));
            return ;
        }
    TxtAdd(JournalPath,AddDataTime("用户 " + User + " 进行了导航或查询了链接\n"),1);
    AlarmNavigation *N = new AlarmNavigation(NowItem,AlarmRow());
    N->show();
}
