#include "alarmwidget.h"
#include "alarmnavigation.h"
#include "txtadd.h"
#include "login.h"
#include "navigation.h"
#include <QDebug>
#include <QTableWidget>
#include <QToolTip>
#include <QMessageBox>
#include <QHeaderView>
#include <map>
extern QString UserAlarmPath;
extern struct SerTime{
    int year=2023,month=2,day=20,hour,minute,second,millisecond;
}*ServerTime;
extern int BeginYear,BeginMonth,BeginDay;
extern int Month_Day[13];
std::map<QTableWidgetItem *,QString> Tip;
AlarmWidget::AlarmWidget(int Width, int Height, QWidget *parent)
{ 
    this->resize(Width,Height);
    QString Data = GetInformation(UserAlarmPath);
    DataList = Data.split("\n");
    NowItem = DataList[1].toInt();
    bool FindFlag = 0;
    if (NowItem <= AlarmRow()) {
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
        }
        this->show();
    }
    if(FindFlag == 0)
    {
        delete this;
        return ;
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

void AlarmWidget::slotCellEnter(QTableWidgetItem *Item)
{
    QToolTip::showText(QCursor::pos(),Tip[Item]);
}

void AlarmWidget::slotNavigation()
{
    qDebug()<<"SlotNavigation";
    QStringList Line = DataList[NowItem].split(" ");
    if(Line[Line.size() - 2] == "线上")
    {
        QMessageBox::information(this,tr("线上活动"),(const QString)("链接为:" + Line[Line.size() - 1]));
        return ;
    }
    AlarmNavigation *N = new AlarmNavigation(NowItem,AlarmRow());
    N->show();
}
