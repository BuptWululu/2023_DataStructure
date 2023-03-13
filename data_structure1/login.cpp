#include "login.h"
#include "ui_login.h"
#include "registerwidget.h"
#include "mainwindow.h"
#include <QPushButton>
#include <QDebug>
#include <QLabel>
#include <QLineEdit>
#include <QFile>
#include <QMessageBox>
#include <bits/stdc++.h>
using std::string;
#define WordSize 15
#define OneWidth 100
#define OneHeight 100
QString InformationPath="E:\\QT_test\\data_structure1\\UserInformation.txt";
QString GetInformation()
{
    QFile *File = new QFile(InformationPath);
    File->open(QIODevice::ReadOnly|QIODevice::Text);
    QString Data = QString(File->readAll());
    return Data;
}
bool CmpUserPwd(QString LineData,QString UserName,QString UserPwd)
{
    QString NowUser="";
    QString NowPwd="";
    int pos;
    for(pos=0;;pos++)
    {
        if(LineData[pos]=='\n'||LineData[pos]==' ')
        {
            pos++;
            break;
        }
        NowUser=NowUser+LineData[pos];
    }
    for(;;pos++)
    {
        if(LineData[pos]=='\n'||LineData[pos]==' ')
            break;
        NowPwd=NowPwd+LineData[pos];
    }
    if(NowUser==UserName&&NowPwd==UserPwd)
        return true;
    else
        return false;
}
bool FindUserPwd(QString Information,QString UserName,QString UserPwd)
{
    QString LineData="";

    for(int i=0;i<Information.size();i++)
    {
        LineData=LineData+Information[i];
        if(Information[i]=='\n')
        {
            if(CmpUserPwd(LineData,UserName,UserPwd)==true)
                return true;
            LineData="";
            continue;
        }
    }
    return false;
}
login::login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);

    //设置登录窗口大小
    this->resize(400,300);

    //设置窗口标题
    this->setWindowTitle("用户登录");

    //设置按钮
    QPushButton *PushbuttonLogin = new QPushButton(this);
    PushbuttonLogin->setText("登录");
    PushbuttonLogin->resize(this->width()/OneWidth*20,this->height()/OneHeight*10);
    PushbuttonLogin->move(this->width()/OneWidth*22,this->height()/OneHeight*67);

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
    MainWindow *MaWindow = new MainWindow;

    connect(PushbuttonLogin,&QPushButton::clicked,[=](){
       QString User = LineEditUser->text();
       QString Pwd = LineEditPwd->text();
       if(FindUserPwd(GetInformation(),User,Pwd))
       {
           this->hide();
           MaWindow->show();
           qDebug()<<"登录成功"<<endl;
           //qDebug()<<"成功"<<endl;
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
