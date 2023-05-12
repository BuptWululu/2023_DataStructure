#include "login.h"
#include <QApplication>
#include <QString>
#include <QStringList>
#include <QTextCodec>
#include <QFile>
#include <QDebug>
#include <QWidget>
QString MainPath;
QString InformationPath;
QString MapPath;
QString EdgePath;
QString JournalPath;
QString TimePath;
QString UserPath,UserCurriculumPath,UserExtracurricularPath,UserTemporaryPath,UserAlarmPath;
QString UserExamPath;
QString AddCurriculumPath;
QString MapFirstLine = "地点\tX\tY\n";
QString EdgeFirstLine = "起点\t终点\t拥挤度\n";
int BeginYear = 2023,BeginMonth = 2,BeginDay = 20;
void SetPath()
{
    MainPath = QCoreApplication::applicationDirPath();
    QStringList Line = MainPath.split("/");
    MainPath = "";
    for(int i = 0;i < Line.size() - 2;i++)
        MainPath = MainPath + Line[i] + "\\";
    MainPath = MainPath + "data_structure1\\";
    UserPath = MainPath + "User";
    InformationPath = MainPath+"UserInformation.txt";
    MapPath = MainPath+"Map.txt";
    EdgePath = MainPath+"Edge.txt";
    JournalPath = MainPath+"Journal.txt";
    TimePath = MainPath+"Time.txt";
    AddCurriculumPath = MainPath+"AddCurriculum.txt";
}
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec* codec = QTextCodec::codecForName("GBK");
    QTextCodec::setCodecForLocale(codec);
    SetPath();
    qApp->setStyleSheet(
                              "QComboBox{"
                              "color:#666666;"
                              "font-size:22px;"
                              "padding: 1px 15px 1px 3px;"
                              "border:1px solid rgba(228,228,228,1);"
                              "border-radius:5px 5px 0px 0px;"
                              "}"
                              "QWidget{background-color:rgb(240,248,255);}"
                              //正常状态样式
                              "QPushButton{"
                              "background-color:rgba(0,0,255,30);"//背景色（也可以设置图片）
                              "border-style:outset;"                  //边框样式（inset/outset）
                              "border-width:4px;"                     //边框宽度像素
                              "border-radius:10px;"                   //边框圆角半径像素
                              "border-color:rgba(255,255,255,30);"    //边框颜色
                              /*"font:bold 10px;"                       //字体，字体大小
                              "color:rgba(0,0,0,100);"                //字体颜色
                              "padding:6px;"                          //填衬*/
                              "}"
                              //鼠标按下样式
                              "QPushButton:pressed{"
                              "background-color:rgba(100,255,100,200);"
                              "border-color:rgba(255,255,255,30);"
                              "border-style:inset;"
                              "color:rgba(0,0,0,100);"
                              "}"
                              //鼠标悬停样式
                              "QPushButton:hover{"
                              "background-color:rgba(100,255,100,100);"
                              "border-color:rgba(255,255,255,200);"
                              "color:rgba(0,0,0,200);"
                              "}");
    login w;
    w.show();

    return a.exec();
}
