#include "registerwidget.h"
#include "login.h"
#include "addfile.h"
#include "txtadd.h"
#include <QPushButton>
#include <QLabel>
#include <QFile>
#include <QLineEdit>
#include <QMessageBox>
#include <QDir>
#define WordSize 15
#define OneWidth 100
#define OneHeight 100
extern QString InformationPath;
extern QString MainPath;
extern QString UserCurriculumPath;
extern QString UserExamPath;
bool CmpUser(QString LineData,QString UserName)
{
    QString NowUser="";
    int pos;
    for(pos=0;;pos++)
    {
        if(LineData[pos]=='\n'||LineData[pos]==' ')
            break;
        NowUser=NowUser+LineData[pos];
    }
    if(NowUser==UserName)
        return true;
    else
        return false;
}
bool FindUser(QString Information,QString UserName)
{
    QString LineData="";

    for(int i=0;i<Information.size();i++)
    {
        LineData=LineData+Information[i];
        if(Information[i]=='\n')
        {
            if(CmpUser(LineData,UserName)==true)
                return true;
            LineData="";
            continue;
        }
    }
    return false;
}
bool IsRight(QString S)
{
    if(S.size()<6) return 0;
    for(int i=0;i<S.size();i++)
    {
        if(S[i]==' '||S[i]=='\n'||S[i]=='\t')
            return 0;
    }
    return 1;
}
RegisterWidget::RegisterWidget(QWidget *parent) : QWidget(parent)
{
    this->resize(400,300);
    this->setWindowTitle("注册账号");

    QPushButton *PushButtonBack = new QPushButton("后退",this);
    PushButtonBack->resize(this->width()/5,this->height()/10);
    PushButtonBack->move(this->width()/4.5,this->height()/1.5);

    QPushButton *PushButtonRegister = new QPushButton("注册",this);
    PushButtonRegister->resize(PushButtonBack->width(),PushButtonBack->height());
    PushButtonRegister->move(this->width()/1.7,this->height()/1.5);
    PushButtonRegister->setDefault(true);

    QLabel *LabelUser = new QLabel(this);
    LabelUser->setFont(QFont("宋体",WordSize));
    LabelUser->setText("用户名：");
    LabelUser->resize(this->width()/OneWidth*20,this->height()/OneHeight*10);
    LabelUser->move(this->width()/OneWidth*20,this->height()/OneHeight*16);

    QLabel *LabelPwd = new QLabel(this);
    LabelPwd->setFont(QFont("宋体",WordSize));
    LabelPwd->setText("密  码：");
    LabelPwd->resize(this->width()/OneWidth*20,this->height()/OneHeight*10);
    LabelPwd->move(this->width()/OneWidth*20,this->height()/OneHeight*29);

    QLabel *LabelPwdAgain = new QLabel(this);
    LabelPwdAgain->setFont(QFont("宋体",WordSize));
    LabelPwdAgain->setText("重复密码：");
    //LabelPwdAgain->resize(this->width()/OneWidth*20,this->height()/OneHeight*10);
    LabelPwdAgain->move(this->width()/OneWidth*15,this->height()/OneHeight*44);

    QLineEdit *LineEditUser = new QLineEdit(this);
    LineEditUser->resize(this->width()/OneWidth*40,this->height()/OneHeight*7);
    LineEditUser->move(this->width()/OneWidth*40,this->height()/OneHeight*18);


    QLineEdit *LineEditPwd = new QLineEdit(this);
    LineEditPwd->resize(this->width()/OneWidth*40,this->height()/OneHeight*7);
    LineEditPwd->move(this->width()/OneWidth*40,this->height()/OneHeight*31);
    LineEditPwd->setEchoMode(QLineEdit::Password);

    QLineEdit *LineEditPwdAgain = new QLineEdit(this);
    LineEditPwdAgain->resize(this->width()/OneWidth*40,this->height()/OneHeight*7);
    LineEditPwdAgain->move(this->width()/OneWidth*40,this->height()/OneHeight*44);
    LineEditPwdAgain->setEchoMode(QLineEdit::Password);

    connect(PushButtonBack,&QPushButton::clicked,[=](){
       emit this->back_login();
    });

    connect(PushButtonRegister,&QPushButton::clicked,[=](){
        QString User = LineEditUser->text();
        QString Pwd = LineEditPwd->text();
        QString PwdAgain =LineEditPwdAgain->text();
        if(!IsRight(User)||!IsRight(Pwd))
        {
            QMessageBox::critical(this, tr("注册失败"),  tr("用户名或密码不符合要求"));
        }
        else
        if(FindUser(GetInformation(InformationPath),User)==false)
        {
            if(Pwd!=PwdAgain)
            {
                QMessageBox::critical(this, tr("注册失败"),  tr("输入的密码不一致"));
            }
            else
            {
                QMessageBox::information(this, tr("提示"),  tr("注册成功"));
                QDir dir(MainPath+"User\\"+User);
                    dir.mkdir(MainPath+"User\\"+User);
                AddFile(MainPath+"User\\"+User+"\\Curriculum.txt");
                AddFile(MainPath+"User\\"+User+"\\Extracurricular.txt");
                AddFile(MainPath+"User\\"+User+"\\Temporary.txt");
                AddFile(MainPath+"User\\"+User+"\\Exam.txt");
                AddFile(MainPath+"User\\"+User+"\\Alarm.txt","\n2\n");
                TxtAdd(InformationPath,User+" "+Pwd+" 0\n",1);
                LineEditUser->clear();
                emit this->back_login();
            }
        }
        else
        {
            QMessageBox::critical(this, tr("注册失败"),  tr("用户名已存在"));
        }
        LineEditPwd->clear();
        LineEditPwdAgain->clear();
    });
}
