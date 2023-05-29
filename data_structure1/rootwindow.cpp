#include "rootwindow.h"
#include "listwidget.h"
#include "adddatatime.h"
#include "addcurriculum.h"
#include "addwidget.h"
#include <QPushButton>
const int PushButtonWidth=250;
const int PushButtonHeight=100;
extern QString JournalPath;
extern QString User;
extern QString AddCurriculumPath;
bool InRoot = 0;
void PushButtonInit(QPushButton *P,int x,int y,QString data)
{
    P->move(x,y);
    P->setText(data);
    P->resize(PushButtonWidth,PushButtonHeight);
    P->setStyleSheet("font:bold 20px;");
}
RootWindow::RootWindow(QWidget *parent) : QMainWindow(parent)
{
    this->setFixedSize(1400,700);
    this->setWindowTitle("管理员 " + User);
    QPushButton *Curriculum = new QPushButton(this);
    PushButtonInit(Curriculum,380,250,"添加课程");
    connect(Curriculum,&QPushButton::clicked,[=](){
        AddCurriculum *A = new AddCurriculum;
        A->show();
    });
    QPushButton *Delete = new QPushButton(this);
    PushButtonInit(Delete,800,250,"修改课程");
    connect(Delete,&QPushButton::clicked,[=](){
        ListWidget *LW = new ListWidget(1000,500);
        LW->setWindowTitle("修改课程");
        LW->setWindowFlags(LW->windowFlags() | Qt::WindowStaysOnTopHint);
        LW->setWindowModality(Qt::ApplicationModal);
        LW->TableInit(AddCurriculumPath,7);
        QStringList Header;
        Header<<"添加对象"<<"课程名称"<<"课程周次"<<"课程时间"<<"上课时间"<<"上课方式"<<"地点/链接";
        LW->SetTableHeader(Header);
        LW->SetTableItems(AddCurriculumPath,0,8,4,-1,2,5);
        LW->ChangedTableEvent();
        LW->AddSavePushButton("保存修改",AddCurriculumPath,850);
        LW->show();
    });
    QPushButton *Exam = new QPushButton(this);
    PushButtonInit(Exam,590,400,"发布考试");
    connect(Exam,&QPushButton::clicked,[=](){
        AddWidget *AW = new AddWidget();
        InRoot = 1;
        AW->setWindowTitle("发布考试");
        AW->AddLabel(57,50,"考试名称:");
        AW->AddLineEdit(150,46);
        AW->AddLabel(20,110,"考试开始日期:");
        AW->AddDateComboBox(150,106);
        AW->AddLabel(20,170,"考试开始时间:");
        AW->AddComboBox(150,166,8,20,":00");
        AW->AddLabel(57,230,"考试地点:");
        AW->AddLineEdit(150,226);
        AW->AddExamPushButton(180,300,"发布考试");
        AW->show();
    });
}
