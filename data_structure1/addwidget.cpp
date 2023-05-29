#include "addwidget.h"
#include "login.h"
#include "adddatatime.h"
#include "txtadd.h"
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QDebug>
#include <QMessageBox>
#include <QLineEdit>
#include <QDir>
#include <algorithm>
#include <QTimer>
#define N 50
extern int Month_Day[13];
extern QTimer *Tim;
extern bool InRoot;
extern int BeginYear,BeginMonth,BeginDay;
extern QString JournalPath;
extern QString User;
extern QString UserPath,UserCurriculumPath,UserExtracurricularPath,UserTemporaryPath;
extern int GetDayNumber(int Year,int Month);
struct WriteQueue
{
    void *Type;
    int Tag;
}Queue[N];
struct WeekToDay{
    int Year,Month,Day;
};
extern WeekToDay GetWeekToDay(int Week,int WeekDay);
extern int GetDateDist(WeekToDay X,WeekToDay Y);
AddWidget::AddWidget(int Width, int Height, QWidget *parent) : QWidget(parent)
{
    this->setWindowTitle("添加活动/事务");
    this->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);
    this->setWindowModality(Qt::ApplicationModal);
    for(int i = 0;i < N;i++)
    {
        LineEdit[i] = new QLineEdit(this);
        LineEdit[i]->hide();
        ComboBox[i] = new QComboBox(this);
        ComboBox[i]->hide();
        PushButton[i] = new QPushButton(this);
        PushButton[i]->hide();
    }
    this->resize(Width,Height);
    this->setStyleSheet("font-size:18px");
}

void AddWidget::AddLabel(int MoveWidth, int MoveHeight, QString Data)
{
    QLabel *L = new QLabel(this);
    L->move(MoveWidth,MoveHeight);
    L->setText(Data);
    L->adjustSize();
    return ;
}

void AddWidget::AddLineEdit(int MoveWidth, int MoveHeight)
{
    LineEdit[++LineEditNumber]->move(MoveWidth,MoveHeight);
    LineEdit[LineEditNumber]->show();
    LineEdit[LineEditNumber]->resize(150,25);
    Queue[++TotNumber].Type = LineEdit[LineEditNumber];
    Queue[TotNumber].Tag = 1;
}

void AddWidget::BuildDateConnect(QComboBox *D, QComboBox *M, QComboBox *Y)
{
    connect(Y,&QComboBox::currentTextChanged,[=](){
        D->clear();
        int Tot = GetDayNumber(Y->currentText().toInt(),M->currentText().toInt());
        for(int i = 1;i <= Tot;i++)
            D->addItem(QString::number(i));
    });
    connect(M,&QComboBox::currentTextChanged,[=](){
        D->clear();
        int Tot = GetDayNumber(Y->currentText().toInt(),M->currentText().toInt());
        for(int i = 1;i <= Tot;i++)
            D->addItem(QString::number(i));
    });
}

QComboBox* AddWidget::AddComboBox(int MoveWidth, int MoveHeight, int Begin, int End, QString Suffix, int SizeWidth, int SizeHeight)
{
    ComboBox[++ComboBoxNumber]->move(MoveWidth,MoveHeight);
    QStringList Data;
    for(int i = Begin;i <= End ;i++)
        Data<<QString::number(i)+Suffix;
    ComboBox[ComboBoxNumber]->clear();
    ComboBox[ComboBoxNumber]->addItems(Data);
    ComboBox[ComboBoxNumber]->adjustSize();
    ComboBox[ComboBoxNumber]->show();
    Queue[++TotNumber].Type = ComboBox[ComboBoxNumber];
    Queue[TotNumber].Tag = 2;
    return ComboBox[ComboBoxNumber];
}

void AddWidget::AddDateComboBox(int MoveWidth, int MoveHeight)
{
    this->AddLabel(220,MoveHeight+4,"年");
    this->AddLabel(300,MoveHeight+4,"月");
    this->AddLabel(380,MoveHeight+4,"日");
    this->BuildDateConnect(this->AddComboBox(320,MoveHeight,1,31),this->AddComboBox(240,MoveHeight,1,12),this->AddComboBox(140,MoveHeight,2023,2030));
}

void AddWidget::AddPushButton(int MoveWidth, int MoveHeight, QStringList Name)
{
    int ClickedNumber = 0;
    PushButton[++PushButtonNumber]->move(MoveWidth,MoveHeight);
    PushButton[PushButtonNumber]->show();
    PushButton[PushButtonNumber]->setText(Name[ClickedNumber]);
    PushButton[PushButtonNumber]->adjustSize();
    int NowNumber = PushButtonNumber;
    connect(PushButton[PushButtonNumber],&QPushButton::clicked,[=]()mutable{
        ClickedNumber++;
        ClickedNumber%=Name.size();
        PushButton[NowNumber]->setText(Name[ClickedNumber]);
    });
    Queue[++TotNumber].Type = PushButton[PushButtonNumber];
    Queue[TotNumber].Tag = 3;
}
QString GetText(WriteQueue Q){
    if(Q.Tag == 1)
    {
        return ((QLineEdit *)Q.Type)->text();
    }
    if(Q.Tag == 2)
    {
        return ((QComboBox *)Q.Type)->currentText();
    }
    if(Q.Tag == 3 || Q.Tag == 4)
    {
        return ((QPushButton *)Q.Type)->text();
    }
}
void AddWidget::AddInsertPushButton(int MoveWidth, int MoveHeight, QString PushButtonName,QString FilePath, QString Prefix)
{
    PushButton[++PushButtonNumber]->move(MoveWidth,MoveHeight);
    PushButton[PushButtonNumber]->show();
    PushButton[PushButtonNumber]->setText(PushButtonName);
    PushButton[PushButtonNumber]->adjustSize();
    connect(PushButton[PushButtonNumber],&QPushButton::clicked,[=]()mutable{
        QString Data = Prefix;
        QStringList DataList ;
        for(int i = 1 ;i <= TotNumber ;i++)
            if(Queue[i].Tag == 2)
                DataList<<((QComboBox *)Queue[i].Type)->currentText();
        for(int i = 1 ;i <= TotNumber ;i++)
            if(Queue[i].Tag == 3 && (Queue[i-1].Tag == 2 || Queue[i+1].Tag == 2))
                DataList<<((QPushButton *)Queue[i].Type)->text();
        if(DataList.size() == 4)
            DataList<<""<<"单次";
        if(FindConflict(DataList))
        {
            int okTimeNr = 0;
            int okTime[4];
            if(DataList[4] == "")
                QMessageBox::critical(this, tr("添加失败"),  tr("与已有课程或活动冲突"));
            else
            {
                for(int i = 6;i < 22;i++)
                {
                    DataList[3] = QString::number(i) + ":00";
                    if(!FindConflict(DataList))
                    {
                        okTime[++okTimeNr] = i;
                        if(okTimeNr == 3)
                            break;
                    }
                }
                if(okTimeNr)
                {
                    QString WarningTip = "与已有课程或活动冲突,但有以下时间可供选择:";
                    for(int i = 1;i <= okTimeNr;i++)
                        WarningTip = WarningTip + "\n" + QString::number(okTime[i]) + ":00";
                    QMessageBox::critical(this, tr("添加失败"),  WarningTip);
                }
                else
                    if(DataList[4] == "个人活动")
                        QMessageBox::critical(this, tr("添加失败"),  tr("与已有课程或活动冲突且无可行时间"));
                    else
                    {
                        QString WarningTip = "与已有课程或活动冲突且无可行时间\n以下时间冲突人数最少：";
                        QDir *dir=new QDir(UserPath);
                        QStringList DirList = dir->entryList(QDir::Dirs);
                        DirList.removeOne(".");
                        DirList.removeOne("..");
                        int ConflictNr[30];
                        bool Choiced[30];
                        for(int i = 6;i < 22;i++)
                        {
                            ConflictNr[i] = 0;
                            Choiced[i] = 0;
                            DataList[3] = QString::number(i) + ":00";
                            for(int j = 0;j < DirList.size();j++)
                            {
                                if(FindConflict(DataList,DirList[j]))
                                    ConflictNr[i]++;
                            }
                        }
                        for(int i = 1;i <= 3;i++)
                        {
                            int MinNr = 1e9,tag;
                            for(int j = 6;j < 22;j++)
                            {
                                if(ConflictNr[j] < MinNr&&!Choiced[j])
                                {
                                    MinNr = ConflictNr[j];
                                    tag = j;
                                }
                            }
                            Choiced[tag] = 1;
                            WarningTip = WarningTip + "\n" +QString::number(tag) + ":00";
                        }
                        QMessageBox::critical(this, tr("添加失败"),  WarningTip);
                    }
            }
        }
        else
        {
            for(int i = 1 ;i <= TotNumber ;i++)
            {
                if(Queue[i].Tag != 4)
                    Data = Data + " " + GetText(Queue[i]);
            }
            Data = Data + "\n";
            TxtAdd(FilePath,Data,1);
            QMessageBox::information(this, tr("提示"),  tr("添加成功"));
            if(TotNumber == 10)
                TxtAdd(JournalPath,AddDataTime("用户 " + User + " 添加课外活动 " + GetText(Queue[1]) + " \n"),1);
            else
                TxtAdd(JournalPath,AddDataTime("用户 " + User + " 添加临时事务 " + GetText(Queue[1]) + " \n"),1);
        }
    });
    Queue[++TotNumber].Type = PushButton[PushButtonNumber];
    Queue[TotNumber].Tag = 4;
    return ;
}

void AddWidget::AddExamPushButton(int MoveWidth, int MoveHeight, QString PushButtonName)
{
    PushButton[++PushButtonNumber]->move(MoveWidth,MoveHeight);
    PushButton[PushButtonNumber]->show();
    PushButton[PushButtonNumber]->setText(PushButtonName);
    PushButton[PushButtonNumber]->adjustSize();
    connect(PushButton[PushButtonNumber],&QPushButton::clicked,[=]()mutable{
       QString Data = "";
       for(int i = 1;i <= TotNumber;i++)
       {
           if(Queue[i].Tag == 1)
           {
               if(Data == "")
                   Data = ((QLineEdit *)Queue[i].Type)->text();
               else
                   Data = Data + " " + ((QLineEdit *)Queue[i].Type)->text();
           }
           if(Queue[i].Tag == 2)
           {
               if(Data == "")
                   Data = ((QComboBox *)Queue[i].Type)->currentText();
               else
                   Data = Data + " " + ((QComboBox *)Queue[i].Type)->currentText();
           }
       }
       QDir *dir=new QDir(UserPath);
       QStringList DirList = dir->entryList(QDir::Dirs);
       DirList.removeOne(".");
       DirList.removeOne("..");
       for(int i = 0;i < DirList.size();i++)
       {
           TxtAdd(UserPath + "\\" + DirList[i] + "\\Exam.txt",Data + "\n",1);
       }
       QMessageBox::information(this, tr("发布成功"), tr("您已成功发布考试"));
       TxtAdd(JournalPath,AddDataTime("管理员 "+ User + " 发布了 " + ((QLineEdit *)Queue[1].Type)->text() + " 考试\n"),1);
    });
}

bool AddWidget::FindCurriculumConflict(QStringList InsertItems,QString FilePath, int BeginColumn, int EndColumn)
{
    QStringList SwitchControl;
    SwitchControl<<"单次"<<"每天"<<"每周";
    QString Data = GetInformation(FilePath);
    QStringList DataList = Data.split("\n");
    std::vector<WeekToDay> CurriculumList;
    std::vector<int> BeginTime;
    std::vector<int> EndTime;
    for(int i = 0;i < DataList.size();i++)
    {
        if(DataList[i] == "") continue;
        QStringList Line = DataList[i].split(" ");
        for(int j = Line[2].toInt();j <= Line[3].toInt();j++)
        {
            WeekToDay X = GetWeekToDay(j,Line[4].toInt());
            if(X.Year > InsertItems[0].toInt()||(X.Year == InsertItems[0].toInt()&&X.Month > InsertItems[1].toInt())||(X.Year == InsertItems[0].toInt()&&X.Month == InsertItems[1].toInt()&&X.Day >= InsertItems[2].toInt()))
            {
                CurriculumList.push_back(GetWeekToDay(j,Line[4].toInt()));
                QStringList Temp = Line[5].split(":");
                BeginTime.push_back(Temp[0].toInt());
                Temp = Line[6].split(":");
                EndTime.push_back(Temp[0].toInt());
            }
        }
    }
    QStringList Temp = InsertItems[3].split(":");
    int NowTime = Temp[0].toInt();
    switch (SwitchControl.indexOf(InsertItems[5])){
    case 0:
        for(int i = 0;i < (int)CurriculumList.size();i++)
        {
            WeekToDay X = CurriculumList[i];
            if(X.Year == InsertItems[0].toInt()&&X.Month == InsertItems[1].toInt()&&X.Day == InsertItems[2].toInt())
            {
                if(BeginTime[i] <= NowTime&& EndTime[i] > NowTime)
                    return 1;
            }
        }
        break;
    case 1:
        for(int i = 0;i < (int)CurriculumList.size();i++)
        {
            if(BeginTime[i] <= NowTime&& EndTime[i] > NowTime)
                return 1;
        }
        break;
    case 2:
        for(int i = 0;i < (int)CurriculumList.size();i++)
        {
            WeekToDay X = CurriculumList[i];
            WeekToDay Y;
            Y.Year = InsertItems[0].toInt();
            Y.Month = InsertItems[1].toInt();
            Y.Day = InsertItems[2].toInt();
            if(BeginTime[i] <= NowTime&& EndTime[i] > NowTime&&GetDateDist(Y,X) %7 == 0)
            {
                return 1;
            }
        }
        break;
    }
    std::vector<WeekToDay>().swap(CurriculumList);
    std::vector<int>().swap(BeginTime);
    std::vector<int>().swap(EndTime);
    return 0;
}

bool AddWidget::FindOtherConflict(QStringList InsertItems,QString FilePath, int BeginColumn, int EndColumn, int FrequencyColumn)
{
    QStringList SwitchControl;
    SwitchControl<<"单次"<<"每天"<<"每周";
    QString Data = GetInformation(FilePath);
    QStringList DataList = Data.split("\n");
    switch (SwitchControl.indexOf(InsertItems[5])) {
    case 0:
        for(int i = 0;i < DataList.size();i++)
        {
            if(DataList[i] == "") continue;
            QStringList Line = DataList[i].split(" ");
            switch (FrequencyColumn) {
            case -1 :
                if(FindEarlierDate(InsertItems,Line,BeginColumn,EndColumn) == 0)
                {
                    if(InsertItems[3] == Line[EndColumn])
                        return 1;
                }
                break;
            default:
                switch (SwitchControl.indexOf(Line[FrequencyColumn])) {
                case 0:
                    if(FindEarlierDate(InsertItems,Line,BeginColumn,EndColumn) == 0)
                    {
                        if(InsertItems[3] == Line[EndColumn])
                            return 1;
                    }
                    break;
                case 1:
                    if(FindEarlierDate(InsertItems,Line,BeginColumn,EndColumn) <= 0)
                    {
                        if(InsertItems[3] == Line[EndColumn])
                            return 1;
                    }
                    break;
                case 2:
                    if(FindEarlierDate(InsertItems,Line,BeginColumn,EndColumn) <= 0)
                    {
                        if(InsertItems[3] != Line[EndColumn])
                            break;
                        if(FindDateDistance(InsertItems,Line,BeginColumn,EndColumn)%7 == 0)
                            return 1;
                    }
                    break;
                default:
                    break;
                }
                break;
            }
        }
        break;
    case 1:
        for(int i = 0;i < DataList.size();i++)
        {
            if(DataList[i] == "") continue;
            QStringList Line = DataList[i].split(" ");
            switch (FrequencyColumn) {
            case -1 :
                if(FindEarlierDate(InsertItems,Line,BeginColumn,EndColumn) >= 0)
                {
                    if(InsertItems[3] == Line[EndColumn])
                        return 1;
                }
                break;
            default:
                switch (SwitchControl.indexOf(Line[FrequencyColumn])) {
                case 0:
                    if(FindEarlierDate(InsertItems,Line,BeginColumn,EndColumn) >= 0)
                    {
                        if(InsertItems[3] == Line[EndColumn])
                            return 1;
                    }
                    break;
                case 1:
                    if(InsertItems[3] == Line[EndColumn])
                        return 1;
                    break;
                case 2:
                    if(InsertItems[3] == Line[EndColumn])
                        return 1;
                    break;
                default:
                    break;
                }
                break;
            }
        }
        break;
    case 2:
        for(int i = 0;i < DataList.size();i++)
        {
            if(DataList[i] == "") continue;
            QStringList Line = DataList[i].split(" ");
            switch (FrequencyColumn) {
            case -1 :
                if(FindEarlierDate(InsertItems,Line,BeginColumn,EndColumn) >= 0)
                {
                    if(InsertItems[3] != Line[EndColumn])
                        break;
                    if(FindDateDistance(InsertItems,Line,BeginColumn,EndColumn)%7 == 0)
                        return 1;
                }
                break;
            default:
                switch (SwitchControl.indexOf(Line[FrequencyColumn])) {
                case 0:
                    if(FindEarlierDate(InsertItems,Line,BeginColumn,EndColumn) >= 0)
                    {
                        if(InsertItems[3] != Line[EndColumn])
                            break;

                        if(FindDateDistance(InsertItems,Line,BeginColumn,EndColumn)%7 == 0)
                            return 1;
                    }
                    break;
                case 1:
                    if(InsertItems[3] == Line[EndColumn])
                        return 1;
                    break;
                case 2:
                    if(InsertItems[3] != Line[EndColumn])
                        break;
                    if(FindDateDistance(InsertItems,Line,BeginColumn,EndColumn)%7 == 0)
                        return 1;
                    break;
                default:
                    break;
                }
                break;
            }
        }
        break;
    }
    return 0;
}

bool AddWidget::FindConflict(QStringList InsertItems,QString Path)
{
    if(Path == "")
        return FindOtherConflict(InsertItems,UserExtracurricularPath,3,6,7) |  FindCurriculumConflict(InsertItems,UserCurriculumPath,2,6);
    else
        return FindOtherConflict(InsertItems,UserPath + "\\" + Path+ "\\Extracurricular.txt",3,6,7) |  FindCurriculumConflict(InsertItems,UserPath + "\\" + Path+ "\\Curriculum.txt",2,6);;
}

int AddWidget::FindDateDistance(QStringList InsertItems, QStringList Line, int BeginColumn, int EndColumn)
{
    int AimYear = InsertItems[0].toInt(),AimMonth = InsertItems[1].toInt(),AimDay = InsertItems[2].toInt();
    int Y = Line[BeginColumn].toInt(),M = Line[BeginColumn+1].toInt(),D = Line[BeginColumn+2].toInt();
    if(AimYear < Y||(AimYear == Y&&AimMonth < M)||(AimYear == Y&&AimMonth == M&&AimDay < D))
    {
        std::swap(AimYear,Y);
        std::swap(AimMonth,M);
        std::swap(AimDay,D);
    }
    int Dist = 0;
    while (Y != AimYear||M != AimMonth||D != AimDay) {
        if(M == 2 &&D == 29)
        {
            M++;
            D = 1;
            Dist++;
            continue;
        }
        if(Month_Day[M] == D)
        {
            if(((Y%400==0)||((Y%4==0)&&(Y%100!=0)))&&(M == 2))
            {
                D++;
                Dist++;
                continue;
            }
            M++;
            if(M == 13)
            {
                M = 1;
                Y++;
            }
            Dist++;
            D = 1;
            continue;
        }
        D++;
        Dist++;
    }
    return Dist;
}

int AddWidget::FindEarlierDate(QStringList InsertItems, QStringList Line, int BeginColumn, int EndColumn)
{
    for(int i = 0;i <= 2;i++)
    {
        if(InsertItems[i].toInt() < Line[BeginColumn + i].toInt())
            return 1;
        if(InsertItems[i].toInt() > Line[BeginColumn + i].toInt())
            return -1;
    }
    return 0;
}
AddWidget::~AddWidget()
{

}

void AddWidget::closeEvent(QCloseEvent *event)
{
    if(!InRoot)
        Tim->start();
}
