#include "mainwindow.h"
#include "navigation.h"
#include "login.h"
#include "journal.h"
#include <QPushButton>
#include <QLabel>
#include <QWidget>
#include <QDebug>
#include <QLineEdit>
#include <QString>
#include <vector>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QScrollArea>
#include <QStringList>
#include <QComboBox>
#define MAX_TYPE 4
QPushButton *pre=NULL;
extern QString User;
extern QString UserFilePath;
int ClickedTimes;
struct UserData{
    QString Name,IndividualOrCollective,Frequency,Time0,Time1,OfflineOrOnline,Position;
    int Year,Month,Day;
    bool isAlarm;
};
std::vector<UserData>Student[MAX_TYPE];
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
void SetLabel(QLabel **L)
{
    for(int i=0;i<=2;i++)
    {
        L[i]->setStyleSheet("font: 20px"
                            );
    }

    L[0]->move(385,32);
    L[0]->setText("年");

    L[1]->move(480,32);
    L[1]->setText("月");

    L[2]->move(575,32);
    L[2]->setText("日");

}
void SetComboBox(QComboBox **C)
{
    QStringList Year,Month,Day;
    for(int i=2022;i<=2030;i++)
        Year<<QString::number(i);
    for(int i=1;i<=12;i++)
        Month<<QString::number(i);
    for(int i=1;i<=31;i++)
        Day<<QString::number(i);

    C[0]->move(300,30);
    C[0]->addItems(Year);

    C[1]->move(415,30);
    C[1]->addItems(Month);

    C[2]->move(510,30);
    C[2]->addItems(Day);
}
void SetTable(QTableWidget *T)
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
    T->move(0,75);
    T->resize(1100,600);
    for(int i=0;i<12;i++)
    {
        T->setRowHeight(i,80);
    }
    for(int i=0;i<7;i++)
    {
        T->setColumnWidth(i,150);
    }
    T->setItem(0,0,new QTableWidgetItem("SBB"));
}
void UserInit(){
    QString data = GetInformation(UserFilePath+"\\"+User+".txt");
    QStringList Line = data.split("\n");
    for(int i=0;i<Line.size();i++)
    {
        QStringList List = Line[i].split(" ");
        UserData U;
        if(List[0] == "Curriculum")
        {
            U.Name = List[1];
            U.Frequency = List[2];
            U.Year = List[3].toInt();
            U.Month = List[4].toInt();
            U.Day = List[5].toInt();
            U.Time0 = List[6];
            U.Time1 = List[7];
            U.OfflineOrOnline = List[8];
            U.Position = List[9];
            U.isAlarm = (bool)List[10].toInt();
            Student[1].push_back(U);
        }
        if(List[0] == "Extracurricular")
        {
            U.Name = List[1];
            U.IndividualOrCollective = List[2];
            U.Frequency = List[3];
            U.Year = List[4].toInt();
            U.Month = List[5].toInt();
            U.Day = List[6].toInt();
            U.Time0 = List[7];
            U.Time1 = List[8];
            U.Position = List[9];
            U.isAlarm = (bool)List[10].toInt();
            Student[2].push_back(U);
        }
        if(List[0] == "Temporary")
        {
            U.Name = List[1];
            U.Year = List[2].toInt();
            U.Month = List[3].toInt();
            U.Day = List[4].toInt();
            U.Time0 = List[5];
            U.Position = List[6];
            U.isAlarm = (bool)List[7].toInt();
            Student[3].push_back(U);
        }
    }
}
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    UserInit();
    this->setFixedSize(1400,700);
    int PushButtonWidth=150;
    int PushButtonHeight=60;

    QPushButton *PushButtonCurriculum = new QPushButton("课程管理 ↓",this);
    PushButtonCurriculum->resize(PushButtonWidth,PushButtonHeight);
    PushButtonCurriculum->move(0,100);
    SetPushButtonStyle(PushButtonCurriculum);

    QWidget *W = new QWidget(this);
    W->move(200,0);
    W->resize(1200,700);

    QLabel *LabelDate[3];
    for(int i=0;i<3;i++)
    LabelDate[i] = new QLabel(W);
    SetLabel(LabelDate);

    QComboBox *ComboBoxDate[3];
    for(int i=0;i<3;i++)
    ComboBoxDate[i] = new QComboBox(W);
    SetComboBox(ComboBoxDate);
    for(int i=0;i<=1;i++)
    {
        connect(ComboBoxDate[i],&QComboBox::currentTextChanged,[=](){
            ComboBoxDate[2]->clear();
            for(int j=1;j<=GetDayNumber(ComboBoxDate[0]->currentText().toInt(),ComboBoxDate[1]->currentText().toInt());j++)
            {
                ComboBoxDate[2]->addItem(QString::number(j));
            }
        });
    }


    QTableWidget *Table = new QTableWidget(W);
    SetTable(Table);
    W->hide();

    connect(PushButtonCurriculum,&QPushButton::clicked,[=](){
       SetClickedStyle(PushButtonCurriculum);
       if(ClickedTimes%2==1)
           W->show();
       else
           W->hide();
    });

    QPushButton *PushButtonExtracurricular = new QPushButton("课外活动 ↓",this);
    PushButtonExtracurricular->resize(PushButtonWidth,PushButtonHeight);
    PushButtonExtracurricular->move(0,160);
    SetPushButtonStyle(PushButtonExtracurricular);

    connect(PushButtonExtracurricular,&QPushButton::clicked,[=](){
       SetClickedStyle(PushButtonExtracurricular);
    });

    QPushButton *PushButtonTemporary = new QPushButton("临时事务 ↓",this);
    PushButtonTemporary->resize(PushButtonWidth,PushButtonHeight);
    PushButtonTemporary->move(0,220);
    SetPushButtonStyle(PushButtonTemporary);

    connect(PushButtonTemporary,&QPushButton::clicked,[=](){
       SetClickedStyle(PushButtonTemporary);
    });

    QPushButton *PushButtonNavigation = new QPushButton("日程导航",this);
    PushButtonNavigation->resize(PushButtonWidth,PushButtonHeight);
    PushButtonNavigation->move(0,280);
    SetPushButtonStyle(PushButtonNavigation);

    connect(PushButtonNavigation,&QPushButton::clicked,[=](){
       SetClickedStyle(PushButtonNavigation);
       navigation *Nav = new navigation;
       Nav->show();
    });

    QPushButton *PushButtonJournal = new QPushButton("查看日志",this);
    PushButtonJournal->resize(PushButtonWidth,PushButtonHeight);
    PushButtonJournal->move(0,340);
    SetPushButtonStyle(PushButtonJournal);

    connect(PushButtonJournal,&QPushButton::clicked,[=](){
        SetClickedStyle(PushButtonJournal);
        Journal *Jou = new Journal;
        Jou->show();
    });

    QLabel *LabelUser = new QLabel("用户名：  "+User,this);
    LabelUser->move(1150,25);
    LabelUser->setStyleSheet("font: 20px;");
    LabelUser->adjustSize();

}
