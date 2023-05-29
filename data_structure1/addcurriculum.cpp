#include "addcurriculum.h"
#include "login.h"
#include "txtadd.h"
#include "adddatatime.h"
#include <login.h>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QFont>
#include <QString>
#include <QDir>
#include <QFileDialog>
#include <QStringList>
#include <QDebug>
#include <QCompleter>
#include <QLineEdit>
#include <QComboBox>
#include <map>
QFont ft;
std::map<QString,int> WeekNumber;
extern QString InformationPath;
extern QString MainPath;
extern QString User;
extern QString JournalPath;
extern QString UserPath;
extern int TotWeek,BeginYear,BeginMonth,BeginDay;
extern QString AddCurriculumPath;
extern bool FindUser(QString Information,QString UserName);
extern QString InformationPath;
void LabelInit(QWidget *W)
{
    W->setStyleSheet("QLabel{"
                "font-size: 18px"
                "}"
                 "QComboBox{"
                     "font-size: 17px"
                     "}");
    QLabel *Title[10];
    for(int i = 1;i <= 9 ;i ++)
    {
        Title[i] = new QLabel(W);
    }
    Title[1]->setText("添加课程对象：");
    Title[1]->move(20,50);

    Title[2]->setText("课程名称：");
    Title[2]->move(57,110);

    Title[3]->setText("课程周次：");
    Title[3]->move(57,170);

    Title[4]->setText("至");
    Title[4]->move(230,170);

    Title[5]->setText("课程时间：");
    Title[5]->move(57,230);

    Title[6]->setText("至");
    Title[6]->move(305,230);

    Title[7]->setText("上课方式：");
    Title[7]->move(57,290);

    Title[8]->setText("上课地点/链接：");
    Title[8]->move(12,350);
}
QStringList GetUserList()
{
    QString data = GetInformation(InformationPath);
    QStringList Line = data.split("\n"),res;
    for(int i = 0;i < Line.size();i++)
    {
        if(Line[i]=="") continue;
        QStringList ALine = Line[i].split(" ");
        if(ALine[2]=="1") continue;
        res<<ALine[0];
    }
    return res;
}
void ChooserUserInit(QPushButton *P,QLineEdit *L)
{
    P->setText("所有用户");
    P->setFont(ft);
    P->move(140,46);
    P->adjustSize();
    L->setEnabled(false);
    L->resize(100,25);
    L->move(250,46);
}
void LineEditInit(QLineEdit *L,int row,int column)
{
    L->resize(150,25);
    L->move(row,column);
}
void ComboBoxInit(QComboBox *B,QComboBox *E)
{
    //B->setFont(ft);
    //E->setFont(ft);
    for(int i = 1;i <= TotWeek ;i++)
    {
        B->addItem(QString::number(i)+"周");
        E->addItem(QString::number(i)+"周");
    }
    B->move(150,166);
    E->move(260,166);
}
void ChangeEnd(QComboBox const *B,QComboBox *E,int gap)
{
    E->clear();
    for(int i = B->currentIndex()+gap;i < B->count() ;i++)
        E->addItem(B->itemText(i));
    return ;
}
void SetComboBoxTime(QComboBox **C)
{
    for(int i=0;i<3;i++)
        C[i]->setStyleSheet("font-size:14px");
    C[0]->move(140,226);
    QString Week[7] = {"星期一","星期二","星期三","星期四","星期五","星期六","星期日"};
    for(int i=0;i<7;i++)
        C[0]->addItem(Week[i]);
    C[1]->move(230,226);
    C[2]->move(330,226);
    for(int i=8;i<=19;i++)
    {
        C[1]->addItem(QString::number(i)+":00");
        C[2]->addItem(QString::number(i+1)+":00");
    }
    for(int i=0;i<3;i++)
        C[i]->adjustSize();
}
void PushButtonStyleInit(QPushButton *S)
{
    S->move(150,288);
    S->setFont(ft);
    S->setText("线下上课");
    S->adjustSize();
}
void PushButtonAddInit(QPushButton *A)
{
    A->move(220,425);
    A->setFont(ft);
    A->setText("添加课程");
    A->resize(125,50);
}
bool Earlier(QString Time1,QString Time2)
{
    QStringList list1 = Time1.split(":");
    QStringList list2 = Time2.split(":");
    return list1[0].toInt()<=list2[0].toInt();
}
bool AddCurriculumError(QString Name,int BeginWeek,int EndWeek,int Week,QString BeginHour,QString EndHour)
{
    QString Data = GetInformation(AddCurriculumPath);
    QStringList Line = Data.split("\n");
    for(int i=0;i<Line.size();i++)
    {
        if(Line[i] == "") continue;
        QStringList data = Line[i].split(" ");
        if(data[0] !="ALL"&&data[0] !=Name&&Name != "") continue;
        int beginweek = data[2].toInt(),endweek = data[3].toInt(),week = data[4].toInt();
        if(EndWeek<beginweek||endweek<BeginWeek) continue;
        if(week != Week) continue;
        if(Earlier(EndHour,data[5])|| Earlier(data[6],BeginHour)) continue;
        return 1;
    }
    return 0;
}
void WriteCurriculum(QString User,QString data)
{
    QString Path = MainPath + "User\\"+User+"\\Curriculum.txt";
    TxtAdd(Path,data,1);
}
int SetCurriculum(QPushButton *isChooseUser,QLineEdit *ChooseUser,QLineEdit *Name,QComboBox *BWeek,QComboBox *EWeek,QComboBox* const *Time,QPushButton *Style,QLineEdit *Position)
{
    QString BeginWeek = BWeek->currentText().remove(BWeek->currentText().size()-1,1);
    QString EndWeek = EWeek->currentText().remove(EWeek->currentText().size()-1,1);
    QString data = Name->text()+" "+BeginWeek+" "+EndWeek+" "+QString::number(WeekNumber[Time[0]->currentText()])+" "
            +Time[1]->currentText()+" "+Time[2]->currentText()+" "+Style->text()+" "+Position->text()+"\n";
    //qDebug()<<data;
    if(isChooseUser->text()=="单个用户")
        if(FindUser(GetInformation(InformationPath),ChooseUser->text())==false)
            return 1;
    if(Name->text()=="")
        return 2;
    if(AddCurriculumError(ChooseUser->text(),BeginWeek.toInt(),EndWeek.toInt(),WeekNumber[Time[0]->currentText()],Time[1]->currentText(),Time[2]->currentText()) == true)
        return 3;
    if(isChooseUser->text()=="所有用户")
    {
        TxtAdd(AddCurriculumPath,"ALL "+data,1);
        QDir *dir=new QDir(UserPath);
        QStringList DirList = dir->entryList(QDir::Dirs);
        DirList.removeOne(".");
        DirList.removeOne("..");
        for(int i = 0;i < DirList.size();i++)
            WriteCurriculum(DirList[i],"Curriculum "+data);
        TxtAdd(JournalPath,AddDataTime("管理员 "+ User + " 为所有用户添加课程 " + Name->text() + "\n"),1);
    }
    if(isChooseUser->text()=="单个用户")
    {
        TxtAdd(AddCurriculumPath,ChooseUser->text()+" "+data,1);
        WriteCurriculum(ChooseUser->text(),"Curriculum "+data);
        TxtAdd(JournalPath,AddDataTime("管理员 "+ User + " 为用户 " + ChooseUser->text() + " 添加课程 " + Name->text() + "\n"),1);
    }
    return 0;
}
AddCurriculum::AddCurriculum(QWidget *parent) : QWidget(parent)
{
    WeekNumber["星期一"]=1,WeekNumber["星期二"]=2,WeekNumber["星期三"]=3,WeekNumber["星期四"]=4;
    WeekNumber["星期五"]=5,WeekNumber["星期六"]=6,WeekNumber["星期日"]=7;
    ft.setPointSize(14);
    this->setWindowTitle("添加课程");
    this->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);
    this->setWindowModality(Qt::ApplicationModal);
    this->resize(400,600);
    LabelInit(this);

    int ChooseClickedNumber = 0;
    QPushButton *PushButtonChooseUser = new QPushButton(this);
    QLineEdit *LineEditChooseUser = new QLineEdit(this);
    ChooserUserInit(PushButtonChooseUser,LineEditChooseUser);

    connect(PushButtonChooseUser,&QPushButton::clicked,[=]()mutable{
        ChooseClickedNumber++;
        if(ChooseClickedNumber %2 ==1)
        {
            PushButtonChooseUser->setText("单个用户");
            LineEditChooseUser->setEnabled(true);
            LineEditChooseUser->setCompleter(new QCompleter(GetUserList()));
        }
        else
        {
            PushButtonChooseUser->setText("所有用户");
            LineEditChooseUser->clear();
            LineEditChooseUser->setEnabled(false);
        }
    });

    QLineEdit *LineEditName = new QLineEdit(this);
    LineEditInit(LineEditName,150,106);

    QComboBox *BeginWeek = new QComboBox(this);
    QComboBox *EndWeek = new QComboBox(this);
    ComboBoxInit(BeginWeek,EndWeek);
    connect(BeginWeek,&QComboBox::currentTextChanged,[=](){
        ChangeEnd(BeginWeek,EndWeek,0);
    });
    QComboBox *ComboBoxTime[3];
    for(int i = 0;i < 3;i++)
        ComboBoxTime[i] = new QComboBox(this);
    SetComboBoxTime(ComboBoxTime);
    connect(ComboBoxTime[1],&QComboBox::currentTextChanged,[=](){
        ChangeEnd(ComboBoxTime[1],ComboBoxTime[2],1);
        ComboBoxTime[2]->addItem("20:00");
    });

    QPushButton *PushButtonStyle = new QPushButton(this);
    PushButtonStyleInit(PushButtonStyle);
    int StyleClickedNumber = 0;
    connect(PushButtonStyle,&QPushButton::clicked,[=]()mutable{
       StyleClickedNumber++;
       if(StyleClickedNumber %2 == 1)
           PushButtonStyle->setText("线上上课");
       else
           PushButtonStyle->setText("线下上课");
    });
    QLineEdit *LineEditPosition = new QLineEdit(this);
    LineEditInit(LineEditPosition,150,345);

    QPushButton *PushButtonAdd = new QPushButton(this);
    PushButtonAddInit(PushButtonAdd);
    connect(PushButtonAdd,&QPushButton::clicked,[=](){
        int tag = SetCurriculum(PushButtonChooseUser,LineEditChooseUser,LineEditName,BeginWeek,EndWeek,ComboBoxTime,PushButtonStyle,LineEditPosition);
        if(tag == 0)
            QMessageBox::information(this, tr("添加成功"),  tr("添加课程成功"));
        if(tag == 1)
            QMessageBox::critical(this, tr("添加失败"),  tr("用户不存在"));
        if(tag == 2)
            QMessageBox::critical(this, tr("添加失败"),  tr("课程名字不能为空"));
        if(tag == 3)
            QMessageBox::critical(this, tr("添加失败"),  tr("课程时间发生冲突"));
    });
}
