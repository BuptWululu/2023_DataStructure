#include "rootwindow.h"
#include "listwidget.h"
#include "addcurriculum.h"
#include <QPushButton>
const int PushButtonWidth=250;
const int PushButtonHeight=100;
extern QString AddCurriculumPath;
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
        LW->TableInit(AddCurriculumPath,7);
        QStringList Header;
        Header<<"添加对象"<<"课程名称"<<"课程周次"<<"课程时间"<<"上课时间"<<"上课方式"<<"地点/链接";
        LW->SetTableHeader(Header);
        LW->SetTableItems(AddCurriculumPath,0,8,4,-1,2,5);
        LW->ChangedTableEvent();
        LW->AddSavePushButton("保存修改",AddCurriculumPath,850);
        LW->show();
    });
}
