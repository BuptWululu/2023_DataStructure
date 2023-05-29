#include "caution.h"
#include "txtadd.h"
#include "adddatatime.h"
#include "login.h"
#include <QDebug>
#include <QToolTip>
#include <vector>
#include <QHeaderView>
#include <map>
#include <QTimer>
extern QTimer *Tim;
extern struct SerTime{
    int year=2023,month=2,day=20,hour,minute,second,millisecond;
}*ServerTime;
extern int Month_Day[13];
extern QString JournalPath;
extern QString User;
extern int BeginYear,BeginMonth,BeginDay;
int PreCautionDay = -1;
extern QString UserPath,UserCurriculumPath,UserExtracurricularPath,UserTemporaryPath,UserAlarmPath;
struct WeekToDay{
    int Year,Month,Day;
};
extern std::map<QTableWidgetItem *,QString> Tip;
WeekToDay GetWeekToDay(int Week,int WeekDay)
{
    WeekToDay X;
    X.Year = BeginYear,X.Month = BeginMonth,X.Day = BeginDay;
    int RemainDay = (Week - 1) * 7 + (WeekDay - 1);
    while(RemainDay --)
    {
        if(X.Month == 2&&(X.Year%400 == 0||(X.Year%4 == 0&&X.Year%100 != 0)))
            X.Day++;
        else
        if(Month_Day[X.Month] == X.Day||(X.Month == 2&&X.Day == 29))
        {
            X.Month++;
            X.Day = 1;
            if(X.Month == 13)
                X.Year++,X.Month = 1;
        }
        else {
            X.Day ++;
        }
    }
    return X;
}
int GetDateDist(WeekToDay X,WeekToDay Y){
    if(X.Day == Y.Day&&X.Month == Y.Month&&X.Year == Y.Year)
        return 0;
    if(X.Month == 2&&(X.Year%400 == 0||(X.Year%4 == 0&&X.Year%100 != 0)))
        X.Day++;
    else
    if(Month_Day[X.Month] == X.Day||(X.Month == 2&&X.Day == 29))
    {
        X.Month++;
        X.Day = 1;
        if(X.Month == 13)
            X.Year++,X.Month = 1;
    }
    else {
        X.Day ++;
    }
    return GetDateDist(X,Y) + 1;
}
Caution::Caution(QWidget *parent) : QWidget(parent)
{
    if(ServerTime->hour != 23||PreCautionDay == ServerTime->day||ServerTime->minute != 0)
    {
        delete this;
        return ;
    }
    this->setWindowTitle("提前提醒");
    this->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);
    this->setWindowModality(Qt::ApplicationModal);
    Tim->stop();
    PreCautionDay = ServerTime->day;
    int NowDate[3] = {ServerTime->year,ServerTime->month,ServerTime->day};
    if(ServerTime->month == 2&&(ServerTime->year%400 == 0||(ServerTime->year%4 == 0&&ServerTime->year%100 != 0)))
        NowDate[2]++;
    else
    if(Month_Day[ServerTime->month] == ServerTime->day||(ServerTime->month == 2&&ServerTime->day == 29))
    {
        NowDate[1] ++;
        NowDate[2] = 1;
        if(NowDate[1] == 13)
            NowDate[0]++,NowDate[1] = 1;
    }
    else {
        NowDate[2] ++;
    }
    this->resize(350,500);
    Table->resize(this->width(),this->height());
    Table->verticalHeader()->setHidden(true);
    Table->setStyleSheet("font-size: 18px");
    Table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QString Data = GetInformation(UserCurriculumPath) + GetInformation(UserExtracurricularPath);
    QStringList DataList = Data.split("\n");
    std::vector<QStringList> TableData;
    for(int i = 0;i < DataList.size();i++)
    {
        if(DataList[i] == "") continue;
        QStringList Line = DataList[i].split(" ");
        if(Line[0] == "Curriculum")
        {
            for(int j = Line[2].toInt();j <= Line[3].toInt();j++)
            {
                WeekToDay WD = GetWeekToDay(j,Line[4].toInt());
                if(WD.Year == NowDate[0]&&WD.Month == NowDate[1]&&WD.Day == NowDate[2])
                {
                    TableData.push_back(Line);
                    break;
                }
            }
        }
        if(Line[0] == "Extracurricular")
        {
            bool EarlierFlag = 0;
            for(int j = 0;j < 3;j++)
            {
                if(NowDate[j] < Line[j + 3].toInt())
                    EarlierFlag = 1;
                if(NowDate[j] > Line[j + 3].toInt())
                    break;
            }
            if(EarlierFlag) continue;
            bool Flag = 1;
            if(Line[7] == "单次")
            {
                for(int j = 0;j < 3;j++)
                    if(NowDate[j] != Line[j + 3].toInt())
                    {
                        Flag = 0;
                        break;
                    }
                if(Flag)
                    TableData.push_back(Line);
            }
            if(Line[7] == "每天")
                TableData.push_back(Line);
            if(Line[7] == "每周")
            {
                WeekToDay X,Y;
                X.Year = Line[3].toInt();
                X.Month = Line[4].toInt();
                X.Day = Line[5].toInt();
                Y.Year = NowDate[0];
                Y.Month = NowDate[1];
                Y.Day = NowDate[2];
                if(GetDateDist(X,Y)%7 == 0)
                    TableData.push_back(Line);
            }
        }
    }
    Table->setRowCount(TableData.size());
    Table->setColumnCount(1);
    Table->setColumnWidth(0,Table->width());
    for(int i = 0;i < Table->rowCount();i++)
    {
        QTableWidgetItem *Item = new QTableWidgetItem;
        Item->setText(TableData[i][1]);
        Table->setItem(i,0,Item);
        Table->item(i,0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        Tip[Item] = GetTip(TableData[i]);
    }
    connect(Table,SIGNAL(itemClicked(QTableWidgetItem*)),this,SLOT(slotCellEnter(QTableWidgetItem*)));
    //qDebug()<<TableData;
    TxtAdd(JournalPath,AddDataTime("用户 " + User + " 提前提醒了第二天的所有课程和课外活动\n"),1);
    this->show();
    std::vector<QStringList>().swap(TableData);
}

QString Caution::GetTip(QStringList Line)
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

void Caution::closeEvent(QCloseEvent *event)
{
    Tim->start();
}
void Caution::slotCellEnter(QTableWidgetItem *Item)
{
    QToolTip::showText(QCursor::pos(),Tip[Item]);
}
