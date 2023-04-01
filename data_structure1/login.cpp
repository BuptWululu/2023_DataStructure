#include "login.h"
#include "ui_login.h"
#include "registerwidget.h"
#include "adddatatime.h"
#include "mainwindow.h"
#include "rootwindow.h"
#include "txtadd.h"
#include <QPushButton>
#include <QDebug>
#include <QLabel>
#include <QLineEdit>
#include <QStringList>
#include <QFile>
#include <QMessageBox>
#include <bits/stdc++.h>
using std::string;
#define WordSize 15
#define OneWidth 100
#define OneHeight 100
extern QString InformationPath;
extern QString JournalPath;
QString User;
bool isRoot = 0;
QString GetInformation(QString Path)
{
    QFile *File = new QFile(Path);
    File->open(QIODevice::ReadOnly|QIODevice::Text);
    QString Data;
    if(Path == JournalPath)
        Data = QString::fromLocal8Bit(File->readAll());
    else
        Data = QString(File->readAll());
    return Data;
}
bool CmpUserPwd(QString LineData,QString UserName,QString UserPwd)
{
    QStringList Line = LineData.split(" ");
    if(Line[0]==UserName&&Line[1]==UserPwd)
    {
        isRoot = (bool)Line[2].toInt();
        return true;
    }
    else
        return false;
}
bool FindUserPwd(QString Information,QString UserName,QString UserPwd)
{
    QStringList LineData = Information.split("\n");

    for(int i=0;i<LineData.size();i++)
    {
        if(CmpUserPwd(LineData[i],UserName,UserPwd)==true)
            return true;
    }
    return false;
}
login::login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);

    //设置登录窗口大小
    this->setFixedSize(400,300);

    //设置窗口标题
    this->setWindowTitle("用户登录");

    //设置按钮
    QPushButton *PushbuttonLogin = new QPushButton(this);
    PushbuttonLogin->setText("登录");
    PushbuttonLogin->resize(this->width()/OneWidth*20,this->height()/OneHeight*10);
    PushbuttonLogin->move(this->width()/OneWidth*22,this->height()/OneHeight*67);
    PushbuttonLogin->setDefault(true);

    QPushButton *PushButtonRegister = new QPushButton(this);
    PushButtonRegister->setText("注册");
    PushButtonRegister->resize(PushbuttonLogin->width(),PushbuttonLogin->height());
    PushButtonRegister->move(this->width()/OneWidth*59,this->height()/OneHeight*67);

    //设置输入提示
    QLabel *LabelUser = new QLabel(this);
    LabelUser->setFont(QFont("宋体",WordSize));
    LabelUser->setText("用户名：");
    LabelUser->resize(this->width()/OneWidth*20,this->height()/OneHeight*10);
    LabelUser->move(this->width()/OneWidth*20,this->height()/OneHeight*19);

    QLabel *LabelPwd = new QLabel(this);
    LabelPwd->setFont(QFont("宋体",WordSize));
    LabelPwd->setText("密  码：");
    LabelPwd->resize(this->width()/OneWidth*20,this->height()/OneHeight*10);
    LabelPwd->move(this->width()/OneWidth*20,this->height()/OneHeight*38.5);

    //设置输入框
    QLineEdit *LineEditUser = new QLineEdit(this);
    LineEditUser->resize(this->width()/OneWidth*40,this->height()/OneHeight*7);
    LineEditUser->move(this->width()/OneWidth*40,this->height()/OneHeight*21);

    QLineEdit *LineEditPwd = new QLineEdit(this);
    LineEditPwd->resize(LineEditUser->width(),LineEditUser->height());
    LineEditPwd->move(this->width()/OneWidth*40,this->height()/OneHeight*40);
    LineEditPwd->setEchoMode(QLineEdit::Password);
    //点击登录按钮

    connect(PushbuttonLogin,&QPushButton::clicked,[=](){
       User = LineEditUser->text();
       QString Pwd = LineEditPwd->text();
       if(FindUserPwd(GetInformation(InformationPath),User,Pwd))
       {
           TxtAdd(JournalPath,AddDataTime("用户 "+User+" 登录成功\n"));
           this->hide();
           if(isRoot == true)
           {
               RootWindow *RtWindow = new RootWindow;
               RtWindow->show();
           }
           else
           {
               MainWindow *MnWindow = new MainWindow;
               MnWindow->show();
           }
       }
       else
       {
           QMessageBox::critical(this, tr("登录失败"),  tr("用户名或密码错误！"));
       }
    });
    RegisterWidget *ReWidget = new RegisterWidget;
    connect(PushButtonRegister,&QPushButton::clicked,[=](){
        this->hide();
        ReWidget->show();
    });
    connect(ReWidget,&RegisterWidget::back_login,[=](){
       ReWidget->hide();
       this->show();
    });
}

login::~login()
{
    delete ui;
}
