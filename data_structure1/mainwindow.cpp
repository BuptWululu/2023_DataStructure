#include "mainwindow.h"
#include "navigation.h"
#include "login.h"
#include "txtadd.h"
#include "getweek.h"
#include "adddatatime.h"
#include "alarmwidget.h"
#include "listwidget.h"
#include "caution.h"
#include "journal.h"
#include <QPushButton>
#include <QLabel>
#include <QWidget>
#include <QDebug>
#include <QTimer>
#include <QFont>
#include <QLineEdit>
#include <QString>
#include <vector>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QScrollArea>
#include <QStringList>
#include <QToolTip>
#include <QCursor>
#include <QComboBox>
#include <map>
#define MAX_TYPE 4
#define MAX_SIZE 1000000
int Interval = 2,TotWeek = 19;
QPushButton *pre=NULL;
extern QString User;
extern QString JournalPath;
extern QString UserCurriculumPath;
extern QString UserExamPath;
extern QString UserExtracurricularPath;
extern QString UserTemporaryPath;
extern QString UserAlarmPath;
extern QString MainPath;
extern QString TimePath;
QTimer *Tim;
int ClickedTimes,NowSpeed = 1,Month_Day[13] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
struct SerTime *ServerTime,BeginTime;
struct UserData{
    QString Name,IndividualOrCollective,Frequency,Time0,Time1,OfflineOrOnline,Position;
    int BeginWeek,EndWeek,Week,Year,Month,Day;
};
struct WeekToDay{
    int Year,Month,Day;
};
extern int GetDateDist(WeekToDay X,WeekToDay Y);
std::vector<UserData>Student[MAX_TYPE];
std::map<QTableWidgetItem *,UserData > ItemToStudent;
QString ChangeLength(QString *T)
{
    if(T->size()==1) return "0"+(*T);
    return *T;
}
QString GetTimeString()
{
    QString H,M,S;
    H = QString::number(ServerTime->hour);
    M = QString::number(ServerTime->minute);
    S = QString::number(ServerTime->second);
    WeekToDay X,Y;
    X.Year = BeginTime.year;
    X.Month = BeginTime.month;
    X.Day = BeginTime.day;
    Y.Year = ServerTime->year;
    Y.Month = ServerTime->month;
    Y.Day = ServerTime->day;
    QString WeekDay[8] = {0,"星期一","星期二","星期三","星期四","星期五","星期六","星期日"};
    int NowWeekDay = GetDateDist(X,Y) % 7 + 1;
    return QString::number(ServerTime->year)+"."+QString::number(ServerTime->month)+"."+QString::number(ServerTime->day)+" "
            +ChangeLength(&H)+":"+ChangeLength(&M)+":"+ChangeLength(&S) + " " + WeekDay[NowWeekDay];
}
void SetServerTime(SerTime *T)
{
    QString data = GetInformation(TimePath);
    QStringList List = data.split(" ");
    T->year = List[0].toInt();
    T->month = List[1].toInt();
    T->day = List[2].toInt();
    T->hour = List[3].toInt();
    T->minute = List[4].toInt();
    T->second = List[5].toInt();
    T->millisecond = List[6].toInt();
}
void AddDay()
{
    if(ServerTime->month==2&&ServerTime->day==29)
    {
        ServerTime->month ++;
        ServerTime->day = 1;
        return ;
    }
    if(Month_Day[ServerTime->month] == ServerTime->day)
    {
        if((ServerTime->year%400==0)||(ServerTime->year%4==0&&ServerTime->year%100!=0))
        {
            ServerTime->day++;
            return ;
        }
        ServerTime->day = 1;
        ServerTime->month ++;
        if(ServerTime->month == 13)
        {
            ServerTime->month = 1;
            ServerTime->year++;
        }
    }
    else
        ServerTime->day++;
    return ;
}
void UpdateTime(QLabel *T)
{
    int PastTime = Interval * NowSpeed;
    ServerTime->millisecond += PastTime;
    int JinWei ;
    JinWei = ServerTime->millisecond/1000;
    ServerTime->millisecond %= 1000;
    ServerTime->second += JinWei;
    JinWei = ServerTime->second/60;
    ServerTime->second %= 60;
    ServerTime->minute += JinWei;
    JinWei = ServerTime->minute/60;
    ServerTime->minute %= 60;
    ServerTime->hour += JinWei;
    JinWei = ServerTime->hour/24;
    ServerTime->hour %= 24;

    if(JinWei == 1)
        AddDay();
    QString data = QString::number(ServerTime->year)+" "+QString::number(ServerTime->month)+" "+QString::number(ServerTime->day)+" "+
            QString::number(ServerTime->hour)+" "+QString::number(ServerTime->minute)+" "+QString::number(ServerTime->second)+" "+
            QString::number(ServerTime->millisecond);
    AlarmWidget *AW = new AlarmWidget();
    Caution *CW = new Caution();
    TxtAdd(TimePath,data,0);
    T->setText("服务器时间："+GetTimeString());
    return ;
}
void SetPushButtonStyle(QPushButton *p)
{
    if(!p) return ;
    if(p->text().mid(p->text().size()-1)=="→")
    p->setText(p->text().replace(p->text().size()-1,p->text().size(),"↓"));
    p->setStyleSheet("QPushButton{"
                     "background-color:rgba(0,0,255,30);"    //背景色（也可以设置图片）
                     "border-style:outset;"                  //边框样式（inset/outset）
                     "border-width:4px;"                     //边框宽度像素
                     "border-radius:0px;"                   //边框圆角半径像素
                     "border-color:rgba(0,0,0,100);"        //边框颜色
                     "font:bold 20px;"                       //字体，字体大小
                     /*"color:rgba(0,0,0,100);"                //字体颜色
                     "padding:6px;"                          //填衬*/
                     "}"
                     "QPushButton:hover{"
                     "background-color:rgba(100,255,100,100);"
                     "border-color:rgba(255,255,255,200);"
                     "color:rgba(0,0,0,200);"
                     "}"
                     );
}
void SetChangeStyle(QPushButton *p)
{
    if(!p) return ;
    if(p->text().mid(p->text().size()-1)=="↓")
    p->setText(p->text().replace(p->text().size()-1,p->text().size(),"→"));
    p->setStyleSheet("QPushButton{"
                     "background-color:rgba(100,255,100,100);"    //背景色（也可以设置图片）
                     "border-style:outset;"                  //边框样式（inset/outset）
                     "border-width:4px;"                     //边框宽度像素
                     "border-radius:0px;"                   //边框圆角半径像素
                     "border-color:rgba(0,0,0,100);"        //边框颜色
                     "font:bold 20px;"                       //字体，字体大小
                     /*"color:rgba(0,0,0,100);"                //字体颜色
                     "padding:6px;"                          //填衬*/
                     "}"
                     "QPushButton:hover{"
                     "background-color:rgba(0,0,255,30);"
                     "border-color:rgba(255,255,255,200);"
                     "color:rgba(0,0,0,200);"
                     "}");
}
void SetClickedStyle(QPushButton *p)
{
    if(pre == p)
    {
        ClickedTimes++;
        if(ClickedTimes % 2 == 0)
            SetPushButtonStyle(p);
        else
            SetChangeStyle(p);
    }
    else
    {
        ClickedTimes = 1;
        SetPushButtonStyle(pre);
        SetChangeStyle(p);
    }
    pre = p;
    return ;
}
int GetDayNumber(int Year,int Month)
{
    if(Month == 2)
    {
        if(Year %400 == 0||(Year %4 == 0 &&Year %100 != 0))
            return 29;
        else
            return 28;
    }
    else
    if(Month == 1 || Month == 3 || Month == 5 || Month == 7 || Month == 8 || Month == 10 || Month == 12)
        return 31;
    else
        return 30;
}
void SetLabelWeek(QLabel *W,QComboBox *const *C)
{
    W->setStyleSheet("font: 20px");
    W->move(125,20);
    if(GetWeek(C) == 0)
        W->setText("不在教学周历内");
    else
        W->setText("第"+QString::number(GetWeek(C))+"周/共"+QString::number(TotWeek)+"周");
}
void SetLabelDate(QLabel **L)
{
    for(int i=0;i<=2;i++)
    {
        L[i]->setStyleSheet("font: 20px");
    }

    L[0]->move(385,17);
    L[0]->setText("年");

    L[1]->move(480,17);
    L[1]->setText("月");

    L[2]->move(575,17);
    L[2]->setText("日");

}
void SetComboBox(QComboBox **C)
{
    QStringList Year,Month,Day;
    for(int i=2023;i<=2030;i++)
        Year<<QString::number(i);
    for(int i=1;i<=12;i++)
        Month<<QString::number(i);
    for(int i=1;i<=31;i++)
        Day<<QString::number(i);

    C[0]->move(300,15);
    C[0]->addItems(Year);

    C[1]->move(415,15);
    C[1]->addItems(Month);

    C[2]->move(510,15);
    C[2]->addItems(Day);
}
void TableInit(QTableWidget *T)
{
    QStringList ColumnList;
    for(int i=8;i<=19;i++)
        ColumnList<<QString::number(i)+":00～"+QString::number(i+1)+":00";

    QStringList RowList;
        RowList<<"星期一"<<"星期二"<<"星期三"<<"星期四"<<"星期五"<<"星期六"<<"星期日";
    T->setRowCount(12);
    T->setColumnCount(7);
    T->setStyleSheet("font: 20px;");
    T->setVerticalHeaderLabels(ColumnList);
    T->setHorizontalHeaderLabels(RowList);
    T->move(0,55);
    T->resize(1100,600);
    T->setEditTriggers(QAbstractItemView::NoEditTriggers);
    T->setMouseTracking(true);
    //T->setToolTip();
    for(int i=0;i<12;i++)
    {
        T->setRowHeight(i,80);
    }
    for(int i=0;i<7;i++)
    {
        T->setColumnWidth(i,150);
    }
    //T->setItem(0,0,new QTableWidgetItem("SBB"));
}
void UserInit(){
    UserCurriculumPath = MainPath + "User\\"+User+"\\Curriculum.txt";
    UserExtracurricularPath = MainPath + "User\\"+User+"\\Extracurricular.txt";
    UserTemporaryPath = MainPath + "User\\"+User+"\\Temporary.txt";
    UserExamPath = MainPath + "User\\"+User+"\\Exam.txt";
    UserAlarmPath = MainPath + "User\\"+User+"\\Alarm.txt";
    QString data = GetInformation(UserCurriculumPath);
    QStringList Line = data.split("\n");
    for(int i=0;i<Line.size();i++)
    {
        if(Line[i]=="") continue;
        QStringList List = Line[i].split(" ");
        UserData U;

        U.Name = List[1];
        U.BeginWeek = List[2].toInt();
        U.EndWeek = List[3].toInt();
        U.Week = List[4].toInt();
        U.Time0 = List[5];
        U.Time1 = List[6];
        U.OfflineOrOnline = List[7];
        U.Position = List[8];
        Student[1].push_back(U);

    }

    data = GetInformation(UserExtracurricularPath);
    Line = data.split("\n");
    for(int i=0;i<Line.size();i++)
    {
        if(Line[i]=="") continue;
        QStringList List = Line[i].split(" ");
        UserData U;
        U.Name = List[1];
        U.IndividualOrCollective = List[2];
        U.Frequency = List[3];
        U.Year = List[4].toInt();
        U.Month = List[5].toInt();
        U.Day = List[6].toInt();
        U.Time0 = List[7];
        U.Time1 = List[8];
        U.Position = List[9];
        Student[2].push_back(U);

    }

    data = GetInformation(UserTemporaryPath);
    Line = data.split("\n");
    for(int i=0;i<Line.size();i++)
    {
        if(Line[i]=="") continue;
        QStringList List = Line[i].split(" ");
        UserData U;
        U.Name = List[1];
        U.Year = List[2].toInt();
        U.Month = List[3].toInt();
        U.Day = List[4].toInt();
        U.Time0 = List[5];
        U.Position = List[6];
        Student[3].push_back(U);
    }
    ServerTime = new SerTime;
    SetServerTime(ServerTime);
}
void SetTimeAndSpeed(QLabel *T,QLabel **P,QComboBox *C)
{
    T->setText("服务器时间："+GetTimeString());
    T->setStyleSheet("font:25px");
    T->adjustSize();
    QStringList V;
    V<<"1"<<"5"<<"60"<<"300"<<"1200";
    C->addItems(V);
    C->setStyleSheet("font:15px");
    C->move(90,50);

    C->adjustSize();

    P[0]->setText("时间流速：");
    P[1]->setText("倍速");
    P[0]->move(0,50);
    P[1]->move(160,50);
    for(int i=0;i<2;i++)
    {
        P[i]->setStyleSheet("font:18px");
        P[i]->adjustSize();
    }
}
/*int FindLastPossibleIndex(std::vector<UserData> U,int LastWeek)
{
    int TotCurriculum = U.size();
    int res = -1,l = 0,r = TotCurriculum-1;
    while(l <= r)
    {
        int mid = (l+r)>>1;
        if(U[mid].BeginWeek <= LastWeek)
        {
            res = mid;
            l = mid + 1;
        }
        else
            r = mid - 1;
    }
    return res;
}*/
int Zeller(int Y,int M,int D){
    int m = M;
    if(m < 3)
    {
        m += 12;
        Y--;
    }
    int j = Y/100;
    int k = Y%100;
    int res = (D + 26*(m+1)/10 + k + k/4 + j/4 + 5*j)%7;
    return res;
}
void PushCurriculumToTable(UserData U,QTableWidget *T)
{
    //QTableWidgetItem
    int BeginRow,EndRow,Column;
    Column = U.Week - 1;
    if(Column < 0)
        Column += 7;
    //qDebug()<<Column;
    QStringList list = U.Time0.split(":");
    BeginRow = list[0].toInt() - 8;
    list = U.Time1.split(":");
    EndRow = list[0].toInt() - 9;
    for(int i = BeginRow;i<=EndRow;i++)
    {
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setText(U.Name);
        ItemToStudent[item] = U;
        T->setItem(i,Column,item);
    }
    return ;
}
void SetTable(QTableWidget *T,QComboBox *const *C)
{
    T->clearContents();
    int Week = GetWeek(C);
    if(Week == 0)
        return ;
    int TotCurriculum = Student[1].size();
    //int LastPossibleIndex = FindLastPossibleIndex(Student[1],Week);
    for(int i = 0;i<TotCurriculum;i++)
    {
        if(Student[1][i].BeginWeek > Week) continue;
        if(Student[1][i].EndWeek < Week) continue;
        PushCurriculumToTable(Student[1][i],T);
    }
    return ;
}
void MainWindow::slotCellEnter(QTableWidgetItem * item) {
    if(item)
    {
        QString Tip ;
        UserData U = ItemToStudent[item];
        Tip = U.Name + "\n";
        Tip = Tip + "上课形式：" + U.OfflineOrOnline + "\n";
        Tip = Tip + "上课周次：" + QString::number(U.BeginWeek) + "-" + QString::number(U.EndWeek) + "周\n";
        Tip = Tip + "上课地点：" + U.Position + "\n";

        QToolTip::showText(QCursor::pos(),Tip);
    }
}
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    //for(int i = 0; i < MAX_TYPE ; i++)
        //Student[i].reserve(MAX_SIZE);
    UserInit();
    this->setWindowTitle("用户 " + User);
    this->setFixedSize(1400,700);
    int PushButtonWidth=150;
    int PushButtonHeight=60;
    Tim = new QTimer(this);
    Tim->setInterval(Interval);


    QLabel *Time = new QLabel(this);
    QLabel *Per[2];
    for(int i=0;i<2;i++)
        Per[i] = new QLabel(this);
    QComboBox *Speed = new QComboBox(this);
    SetTimeAndSpeed(Time,Per,Speed);

    Tim->start();
    connect(Tim,&QTimer::timeout,[=](){
        UpdateTime(Time);
    });
    connect(Speed,&QComboBox::currentTextChanged,[=](){
        TxtAdd(JournalPath,AddDataTime("用户 " + User + " 修改时间流速为 " +Speed->currentText() + "倍\n"),1);
        NowSpeed = Speed->currentText().toInt();
    });

    QPushButton *PushButtonCurriculum = new QPushButton("课程管理 ↓",this);
    PushButtonCurriculum->resize(PushButtonWidth,PushButtonHeight);
    PushButtonCurriculum->move(0,100);
    SetPushButtonStyle(PushButtonCurriculum);

    QWidget *W = new QWidget(this);
    ListWidget *LWExtracurricular = new ListWidget(1200,600,this);
    LWExtracurricular->hide();
    ListWidget *LWTemporary = new ListWidget(1200,600,this);
    LWTemporary->hide();
    W->move(200,30);
    W->resize(1200,700);

    QLabel *Week = new QLabel(W);

    QLabel *LabelDate[3];
    for(int i=0;i<3;i++)
    LabelDate[i] = new QLabel(W);
    SetLabelDate(LabelDate);

    QComboBox *ComboBoxDate[3];
    for(int i=0;i<3;i++)
    ComboBoxDate[i] = new QComboBox(W);
    SetComboBox(ComboBoxDate);
    SetLabelWeek(Week,ComboBoxDate);

    QTableWidget *Table = new QTableWidget(W);
    TableInit(Table);
    connect(Table,SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(slotCellEnter(QTableWidgetItem *)));
    SetTable(Table,ComboBoxDate);
    for(int i=0;i<=2;i++)
    {
        connect(ComboBoxDate[i],&QComboBox::currentTextChanged,[=](){
            if(ComboBoxDate[2]->currentText()!="")
            {
                SetLabelWeek(Week,ComboBoxDate);
                SetTable(Table,ComboBoxDate);
            }
            if(i == 0 || i == 1)
            {
                ComboBoxDate[2]->clear();
                for(int j=1;j<=GetDayNumber(ComboBoxDate[0]->currentText().toInt(),ComboBoxDate[1]->currentText().toInt());j++)
                {
                    ComboBoxDate[2]->addItem(QString::number(j));
                }
            }
        });
    }
    QPushButton *ExamPushButton = new QPushButton(W);
    ExamPushButton->setStyleSheet("font-size : 20px");
    ExamPushButton->setText("查看考试");
    ExamPushButton->move(1100,200);
    QWidget *ExamWidget = new QWidget();
    ExamWidget->setWindowTitle("考试");
    QTableWidget *ExamTable = new QTableWidget(ExamWidget);
    ExamWidget->resize(400,600);
    ExamWidget->setWindowFlags(ExamWidget->windowFlags() | Qt::WindowStaysOnTopHint);
    ExamWidget->setWindowModality(Qt::ApplicationModal);
    ExamTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ExamTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ExamTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ExamTable->resize(ExamWidget->width(),ExamWidget->height());
    QStringList ExamDataList = GetInformation(UserExamPath).split("\n");
    int ExamTableRow = 0;
    for(int i = 0;i < ExamDataList.size();i++)
        if(ExamDataList[i] != "") ExamTableRow++;
    ExamTable->setRowCount(ExamTableRow);
    ExamTable->setColumnCount(4);
    QStringList ExamHeader;
    ExamHeader<<"考试名称"<<"考试日期"<<"考试时间"<<"考试地点";
    ExamTable->setHorizontalHeaderLabels(ExamHeader);
    ExamTableRow = 0;
    for(int i = 0;i < ExamDataList.size();i++)
    {
        if(ExamDataList[i] == "") continue;
        QStringList Line = ExamDataList[i].split(" ");
        ExamTable->setItem(ExamTableRow,0,new QTableWidgetItem(Line[0]));
        ExamTable->setItem(ExamTableRow,1,new QTableWidgetItem(Line[1] + "年" + Line[2] + "月" + Line[3] + "日"));
        ExamTable->setItem(ExamTableRow,2,new QTableWidgetItem(Line[4]));
        ExamTable->setItem(ExamTableRow++,3,new QTableWidgetItem(Line[5]));
    }

    for (int i = 0; i < ExamTable->rowCount(); i++)
        for (int j = 0; j < ExamTable->columnCount(); j++)
            ExamTable->item(i,j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    connect(ExamPushButton,&QPushButton::clicked,[=](){
       ExamWidget->show();
    });
    W->hide();
    connect(PushButtonCurriculum,&QPushButton::clicked,[=](){
       SetClickedStyle(PushButtonCurriculum);
       if(ClickedTimes%2==1)
           W->show();
       else
           W->hide();
       LWExtracurricular->hide();
       LWTemporary->hide();
    });

    QPushButton *PushButtonExtracurricular = new QPushButton("课外活动 ↓",this);
    PushButtonExtracurricular->resize(PushButtonWidth,PushButtonHeight);
    PushButtonExtracurricular->move(0,160);
    SetPushButtonStyle(PushButtonExtracurricular);

    connect(PushButtonExtracurricular,&QPushButton::clicked,[=](){
       SetClickedStyle(PushButtonExtracurricular);
       LWExtracurricular->Init();
       LWExtracurricular->move(200,50);
       LWExtracurricular->TableInit(UserExtracurricularPath,7);
       QStringList Header;
       Header<<"名称"<<"类型"<<"开始日期"<<"开始时间"<<"频率"<<"方式"<<"地点";
       LWExtracurricular->SetTableHeader(Header);
       LWExtracurricular->PushButtonInit("添加课外活动");
       LWExtracurricular->SetTableItems(UserExtracurricularPath,1,9,-1,3);
       LWExtracurricular->AW->AddLabel(57,50,"活动名称:");
       LWExtracurricular->AW->AddLineEdit(150,46);
       LWExtracurricular->AW->AddLabel(57,110,"活动类型:");
       QStringList Data;
       Data<<"个人活动"<<"集体活动";
       LWExtracurricular->AW->AddPushButton(150,106,Data);
       LWExtracurricular->AW->AddLabel(20,170,"活动开始日期:");
       LWExtracurricular->AW->AddDateComboBox(150,166);
       LWExtracurricular->AW->AddLabel(20,230,"活动开始时间:");
       LWExtracurricular->AW->AddComboBox(150,226,6,21,":00");
       LWExtracurricular->AW->AddLabel(57,290,"活动频率:");
       Data.clear();
       Data<<"单次"<<"每天"<<"每周";
       LWExtracurricular->AW->AddPushButton(150,286,Data);
       LWExtracurricular->AW->AddLabel(57,350,"活动方式:");
       Data.clear();
       Data<<"线下"<<"线上";
       LWExtracurricular->AW->AddPushButton(150,346,Data);
       LWExtracurricular->AW->AddLabel(12,410,"活动地点/链接:");
       LWExtracurricular->AW->AddLineEdit(150,406);
       LWExtracurricular->AW->AddInsertPushButton(200,500,"添加活动",UserExtracurricularPath,"Extracurricular");
       LWExtracurricular->AddItemsChosenComboBox(1);
       LWExtracurricular->AddSortPushButton("按时间排序",2,3);
       if(ClickedTimes%2==1)
           LWExtracurricular->show();
       else
           LWExtracurricular->hide();
       W->hide();
       LWTemporary->hide();
    });

    QPushButton *PushButtonTemporary = new QPushButton("临时事务 ↓",this);
    PushButtonTemporary->resize(PushButtonWidth,PushButtonHeight);
    PushButtonTemporary->move(0,220);
    SetPushButtonStyle(PushButtonTemporary);

    connect(PushButtonTemporary,&QPushButton::clicked,[=](){
       SetClickedStyle(PushButtonTemporary);
       LWTemporary->Init();
       LWTemporary->move(200,50);
       LWTemporary->TableInit(UserTemporaryPath,4);
       QStringList Header;
       Header<<"名称"<<"开始日期"<<"开始时间"<<"地点";
       LWTemporary->Init();
       LWTemporary->SetTableHeader(Header);
       LWTemporary->PushButtonInit("添加临时活动");
       LWTemporary->SetTableItems(UserTemporaryPath,1,6,-1,2);
       LWTemporary->AW->AddLabel(57,50,"活动名称:");
       LWTemporary->AW->AddLineEdit(150,46);
       LWTemporary->AW->AddLabel(20,110,"活动开始日期:");
       LWTemporary->AW->AddDateComboBox(150,106);
       LWTemporary->AW->AddLabel(20,170,"活动开始时间:");
       LWTemporary->AW->AddComboBox(150,166,6,21,":00");
       LWTemporary->AW->AddLabel(57,230,"活动地点:");
       LWTemporary->AW->AddLineEdit(150,226);
       LWTemporary->AW->AddInsertPushButton(200,500,"添加活动",UserTemporaryPath,"Temporary");
       LWTemporary->AddItemsChosenComboBox(0);
       LWTemporary->AddSortPushButton("按时间排序",1,2);
       if(ClickedTimes%2==1)
           LWTemporary->show();
       else
           LWTemporary->hide();
       W->hide();
       LWExtracurricular->hide();
    });

    QPushButton *PushButtonNavigation = new QPushButton("日程导航",this);
    PushButtonNavigation->resize(PushButtonWidth,PushButtonHeight);
    PushButtonNavigation->move(0,280);
    SetPushButtonStyle(PushButtonNavigation);

    connect(PushButtonNavigation,&QPushButton::clicked,[=](){
       SetClickedStyle(PushButtonNavigation);
       W->hide();
       navigation *Nav = new navigation;
       Nav->show();
    });

    QPushButton *PushButtonJournal = new QPushButton("查看日志",this);
    PushButtonJournal->resize(PushButtonWidth,PushButtonHeight);
    PushButtonJournal->move(0,340);
    SetPushButtonStyle(PushButtonJournal);

    connect(PushButtonJournal,&QPushButton::clicked,[=](){
        SetClickedStyle(PushButtonJournal);
        W->hide();
        TxtAdd(JournalPath,AddDataTime("用户 " + User + " 查看了日志\n"),1);
        Journal *Jou = new Journal;
        Jou->show();
    });

    QLabel *LabelUser = new QLabel("用户名：  "+User,this);
    LabelUser->move(1150,25);
    LabelUser->setStyleSheet("font: 20px;");
    LabelUser->adjustSize();

}
