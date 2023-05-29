#include "listwidget.h"
#include "txtadd.h"
#include "txtdelete.h"
#include "addwidget.h"
#include "adddatatime.h"
#include "optwidget.h"
#include "login.h"
#include <QTableWidget>
#include <QDebug>
#include <QPushButton>
#include <QStringList>
#include <QHeaderView>
#include <QString>
#include <algorithm>
#include <QMessageBox>
#include <QDir>
#include <QTimer>
#include <map>
#define N 50
#define MAXITEM 10000
extern struct WriteQueue
{
    void *Type;
    int Tag;
}Queue[N];
extern struct SerTime{
    int year=2023,month=2,day=20,hour,minute,second,millisecond;
}*ServerTime;
std::map<QString,QString> weekNumber;
extern QTimer *Tim;
extern QString UserAlarmPath;
extern QString UserCurriculumPath;
extern QString MainPath;
extern QString JournalPath;
extern QString User;
extern QString UserPath;
extern QString UserExamPath;
extern QString UserExtracurricularPath;
extern QString UserTemporaryPath;
extern QString AddCurriculumPath;
ListWidget::ListWidget(int Width, int Height, QWidget *parent) : QWidget(parent)
{
    weekNumber["星期一"] = "1";weekNumber["星期二"] = "2";weekNumber["星期三"] = "3";
    weekNumber["星期四"] = "4";weekNumber["星期五"] = "5";weekNumber["星期六"] = "6";weekNumber["星期日"] = "7";
    this->resize(Width,Height);
    Table = new QTableWidget(this);
    QHeaderView* headerView = Table->verticalHeader();
    headerView->setHidden(true);
    SortPushButton = new QPushButton(this);
    SavePushButton = new QPushButton(this);
    SavePushButton->hide();
    ItemsChosen = new QComboBox(this);
    Table->move(0,50);
    AddPushButton = new QPushButton(this);
    Table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    Table->setSelectionBehavior(QAbstractItemView::SelectRows);
    AW = new AddWidget(400,600);
    OW = new OptWidget();
}

int ListWidget::GetFileLine(QString FilePath)
{
    QString data = GetInformation(FilePath);
    QStringList Line = data.split("\n");
    int ret = 0;
    for(int i = 0;i < Line.size();i++)
        if(Line[i] != "")
            ret++;
    return ret;
}
void DisConnect(WriteQueue Q){
    if(Q.Tag == 2)
        ((QComboBox *)Q.Type)->disconnect();
    if(Q.Tag == 3 || Q.Tag == 4)
    {
        ((QPushButton *)Q.Type)->disconnect();
    }

}
void ListWidget::Init()
{
    for(int i = 1;i <= AW->TotNumber ;i++)
        DisConnect(Queue[i]);
    AW->TotNumber = 0;
    AW->LineEditNumber = 0;
    AW->ComboBoxNumber = 0;
    AW->PushButtonNumber = 0;
    OW->disconnect();
    ItemsChosen->clear();
    for(int i = 0;i < MAXITEM ;i++)
        Hash[i] = "",HashLine[i].clear();
}
void ListWidget::TableInit(QString FilePath,int Column)
{
    Table->setStyleSheet("font-size:18px");
    Table->setSelectionBehavior(QAbstractItemView::SelectRows);
    Table->setSelectionMode(QAbstractItemView::SingleSelection);
    Table->resize(this->width(),this->height());
    Table->setRowCount(this->GetFileLine(FilePath));
    Table->setColumnCount(Column);
    Table->resize(this->width()*5/6,this->height()*9/10);
    connect(Table,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(ShowOptWidget(int,int)));
}

void ListWidget::ChangedTableEvent()
{
    Table->disconnect();
    Table->setEditTriggers(QAbstractItemView::DoubleClicked);
    Table->setSelectionBehavior(QAbstractItemView::SelectRows);
    Table->move(0,0);
    Table->resize(Table->width(),this->height());
    AddPushButton->hide();
    SortPushButton->hide();
    ItemsChosen->hide();
}

void ListWidget::PushButtonInit(QString Text,int Width,int Height)
{
    AddPushButton->setStyleSheet("font-size:20px");
    AddPushButton->move(1025,150);
    AddPushButton->resize(Width,Height);
    AddPushButton->setText(Text);
    connect(AddPushButton,SIGNAL(clicked(bool)), this, SLOT(AddWidgetShow()));
}

void ListWidget::SetTableHeader(QStringList List)
{
    Table->setHorizontalHeaderLabels(List);
}

void ListWidget::SetTableItems(QString FilePath, int BeginColumn, int EndColumn, int WeekNameColumn,int DateColumn,int WeekColumn, int TimeColumn)
{
    QString Data = GetInformation(FilePath);
    QStringList Line = Data.split("\n");
    int NowRow = 0;
    for(int i = 0;i < Line.size();i++)
    {
        if(Line[i] == "") continue;
        QStringList data = Line[i].split(" ");
        int NowColumn = 0;
        for(int j = BeginColumn;j <= EndColumn;j++)
        {
            if(j == WeekNameColumn)
                Table->setItem(NowRow,NowColumn,new QTableWidgetItem(WeekName[data[j].toInt()]));
            else
            if(j == DateColumn)
            {
                Table->setItem(NowRow,NowColumn,new QTableWidgetItem(data[j]+"年"+data[j+1]+"月"+data[j+2]+"日"));
                j+=2;
            }
            else
            if(j == WeekColumn)
            {
                Table->setItem(NowRow,NowColumn,new QTableWidgetItem(data[j]+"-"+data[j+1]+"周"));
                j++;
            }
            else
            if(j == TimeColumn)
            {
                Table->setItem(NowRow,NowColumn,new QTableWidgetItem(data[j]+"-"+data[j+1]));
                j++;
            }
            else
                Table->setItem(NowRow,NowColumn,new QTableWidgetItem(data[j]));
            NowColumn++;
        }
        NowRow++;
    }
    Table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    for (int i = 0; i < Table->rowCount(); i++)
        for (int j = 0; j < Table->columnCount(); j++)
            Table->item(i,j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
}

void ListWidget::AddItemsChosenComboBox(int ChosenColumn, int MoveWidth, int MoveHeight)
{
    ItemsChosen->move(MoveWidth,MoveHeight);
    ItemsChosen->addItem("ALL");
    //Table->setRowHidden(0, true);
    for(int i = 0;i < Table->rowCount();i++)
    {
        if(FindHash(Table->item(i,ChosenColumn)->text()) == -1)
        {
            InsertHash(Table->item(i,ChosenColumn)->text());
            ItemsChosen->addItem(Table->item(i,ChosenColumn)->text());
        }
        HashLine[GetHash(Table->item(i,ChosenColumn)->text())].push_back(i);
    }
    connect(ItemsChosen,&QComboBox::currentTextChanged,[=](){
        for(int i = 0;i < Table->rowCount();i++)
            Table->setRowHidden(i, true);
        if(ItemsChosen->currentText() == "ALL")
            for(int i = 0;i < Table->rowCount();i++)
                Table->setRowHidden(i, false);
        else
            for(int i = 0;i < Table->rowCount();i++)
                if(Table->item(i,ChosenColumn)->text() == ItemsChosen->currentText())
                    Table->setRowHidden(i, false);
        if(Table->columnCount() == 7)
        {
            if(ItemsChosen->currentText() == "ALL")
                TxtAdd(JournalPath,AddDataTime("用户 " + User + " 查询了所有课外活动\n"),1);
            else
                TxtAdd(JournalPath,AddDataTime("用户 " + User + " 查询了课外活动 " + ItemsChosen->currentText() + " \n"),1);
        }
        else
            if(Table->columnCount() == 4)
            {
                if(ItemsChosen->currentText() == "ALL")
                    TxtAdd(JournalPath,AddDataTime("用户 " + User + " 查询了所有临时事务\n"),1);
                else
                    TxtAdd(JournalPath,AddDataTime("用户 " + User + " 查询了临时事务 " + ItemsChosen->currentText() + " \n"),1);
            }
    });
}

void ListWidget::AddSortPushButton(QString Text, int DateColumn, int TimeColumn,int MoveWidth, int MoveHeight)
{
    SortPushButton->move(MoveWidth,MoveHeight);
    SortPushButton->setStyleSheet("font-size:20px");
    SortPushButton->setText(Text);
    connect(SortPushButton,&QPushButton::clicked,[=](){
        qSort(DateColumn,TimeColumn);
        if(Table->columnCount() == 7)
            TxtAdd(JournalPath,AddDataTime("用户 " + User + " 为课外活动按照时间排序\n"),1);
        if(Table->columnCount() == 4)
            TxtAdd(JournalPath,AddDataTime("用户 " + User + " 为临时事务按照时间排序\n"),1);
    });
}

void ListWidget::AddSavePushButton(QString Text, QString Path, int MoveWidth, int MoveHeight)
{
    SavePushButton->show();
    SavePushButton->move(MoveWidth,MoveHeight);
    SavePushButton->setText(Text);
    SavePushButton->setStyleSheet("font-size: 20px");
    SavePushButton->adjustSize();
    connect(SavePushButton,&QPushButton::clicked,[=](){
        TxtAdd(JournalPath,AddDataTime("管理员 " + User + " 修改了课程\n"),1);
        QString Data;
        for(int i = 0;i < Table->rowCount();i++)
        {
            for(int j = 0;j < Table->columnCount();j++)
            {
                QString NowData = Table->item(i,j)->text();
                QString searchText = "-";
                QString replaceText = " ";
                if(j == 2||j == 4)
                    NowData = NowData.replace(QRegExp(searchText), replaceText);
                searchText = "周";
                replaceText = "";
                if(j == 2)
                   NowData = NowData.replace(QRegExp(searchText), replaceText);
                if(j == 3)
                   NowData = weekNumber[NowData];
                if(j == Table->columnCount() - 1)
                    Data = Data + NowData + "\n";
                else
                    Data = Data + NowData + " ";
            }
        }
        TxtAdd(Path,Data,0);
        Data = GetInformation(Path);
        QDir *dir=new QDir(UserPath);
        QStringList DirList = dir->entryList(QDir::Dirs);
        DirList.removeOne(".");
        DirList.removeOne("..");
        for(int i = 0;i < DirList.size();i++)
            TxtAdd(MainPath + "User\\"+DirList[i]+"\\Curriculum.txt","\n",0);
        QStringList DataList = Data.split("\n");
        for(int i = 0;i < DataList.size();i++)
        {
            if(DataList[i] == "") continue;
            QStringList Line = DataList[i].split(" ");
            QString InsertData = "Curriculum";
            for(int j = 1;j < Line.size();j++)
                InsertData = InsertData + " " + Line[j];
            InsertData = InsertData + "\n";
            if(Line[0] == "ALL")
            {
                for(int i = 0;i < DirList.size();i++)
                    TxtAdd(MainPath + "User\\"+DirList[i]+"\\Curriculum.txt",InsertData,1);
            }
            else
            {
                for(int i = 0;i < DirList.size();i++)
                {
                    if(DirList[i] == Line[0])
                        TxtAdd(MainPath + "User\\"+DirList[i]+"\\Curriculum.txt",InsertData,1);
                }
            }
        }
    });
}
int ListWidget::CompareTime(int aRow,int bRow,int DateColumn, int TimeColumn)
{
    QString aData = Table->item(aRow,DateColumn)->text() + Table->item(aRow,TimeColumn)->text();
    QString bData = Table->item(bRow,DateColumn)->text() + Table->item(bRow,TimeColumn)->text();
    QRegExp Separator = QRegExp("年|月|日|:");
    QStringList aList = aData.split(Separator,QString::SkipEmptyParts);
    QStringList bList = bData.split(Separator,QString::SkipEmptyParts);
    for(int i = 0;i < aList.size();i++)
        if(aList[i].toInt() > bList[i].toInt())
            return 1;
        else
            if(aList[i].toInt() < bList[i].toInt())
                return 0;
    return 1;
    //>=
}
bool FindEarlierTime(QStringList aList,QStringList bList){
    for(int i = 0;i < aList.size();i++)
    {
        if(aList[i].toInt() < bList[i].toInt())
            return 1;
        if(aList[i].toInt() > bList[i].toInt())
            return 0;
    }
    return 0;
}

int ListWidget::FindInsertRow(QString data)
{
    QStringList datalist = data.split(" ");
    QStringList aList,bList;
    if(datalist.size() == 7)
    {
        QRegExp Separator = QRegExp("年|月|日|:");
        QStringList TemList = datalist[2].split(Separator,QString::SkipEmptyParts);
        for(int i = 0;i < TemList.size();i++)
            aList<<TemList[i];
        TemList = datalist[3].split(Separator,QString::SkipEmptyParts);
        for(int i = 0;i < TemList.size();i++)
            aList<<TemList[i];
    }
    if(datalist.size() == 4)
    {
        QRegExp Separator = QRegExp("年|月|日|:");
        QStringList TemList = datalist[1].split(Separator,QString::SkipEmptyParts);
        for(int i = 0;i < TemList.size();i++)
            aList<<TemList[i];
        TemList = datalist[2].split(Separator,QString::SkipEmptyParts);
        for(int i = 0;i < TemList.size();i++)
            aList<<TemList[i];
    }

    if(aList[0].toInt() < ServerTime->year)
        return -1;
    if(aList[0].toInt() == ServerTime->year&&aList[1].toInt() < ServerTime->month)
        return -1;
    if(aList[0].toInt() == ServerTime->year&&aList[1].toInt() == ServerTime->month&&aList[2].toInt() < ServerTime->day)
        return -1;
    if(aList[0].toInt() == ServerTime->year&&aList[1].toInt() == ServerTime->month&&aList[2].toInt() == ServerTime->day&&aList[3].toInt() < ServerTime->hour)
        return -1;
    if(aList[0].toInt() == ServerTime->year&&aList[1].toInt() == ServerTime->month&&aList[2].toInt() == ServerTime->day&&aList[3].toInt() == ServerTime->hour&&aList[4].toInt() <= ServerTime->hour)
        return -1;
    QString Data = GetInformation(UserAlarmPath);
    QStringList LineList = Data.split("\n");
    for(int i = 0;i < LineList.size();i++)
    {
        QRegExp Separator = QRegExp(" |:");
        QStringList Line = LineList[i].split(Separator,QString::SkipEmptyParts);
        if(Line.size() <= 1) continue;
        bList.clear();
        if(Line[0] == "Curriculum" || Line[0] == "Temporary")
        {
            for(int j = 2;j <= 6;j++)
                bList<<Line[j];
        }
        if(Line[0] == "Extracurricular")
        {
            for(int j = 3;j <= 7;j++)
                bList<<Line[j];
        }
        //qDebug()<<aList<<bList;
        if(FindEarlierTime(aList,bList))
            return i;

    }
    return LineList.size() - 1;
}
void ListWidget::quick_sort(int *SortedId,int *PreId,int low,int high,int DateColumn, int TimeColumn)
{
    int i,j;
    int tmp;
    i = low;
    j = high;
    tmp = SortedId[low];   //任命为中间分界线，左边比他小，右边比他大,通常第一个元素是基准数
    if(i > j)  //如果下标i大于下标j，函数结束运行
        return ;
    //qDebug()<<low<<high;
    while(i != j)
    {
        while(CompareTime(SortedId[j],tmp,DateColumn,TimeColumn) && j > i)
            j--;
        while(CompareTime(tmp,SortedId[i],DateColumn,TimeColumn) && j > i)
            i++;
        if(j > i)
        {
            std::swap(SortedId[i],SortedId[j]);
        }
    }
    SortedId[low] = SortedId[i];
    SortedId[i] = tmp;
    //num[low] = num[i];
    //num[i] = tmp;
    quick_sort(SortedId,PreId,low,i-1,DateColumn,TimeColumn);
    quick_sort(SortedId,PreId,i+1,high,DateColumn,TimeColumn);
}
void ListWidget::qSort(int DateColumn, int TimeColumn)
{
    int SortedId[MAXITEM],PreId[MAXITEM],NowItems = 0;
    if(ItemsChosen->currentText() == "ALL")
        for(int i = 0;i < Table->rowCount();i++)
        {
            //PreId[NowItems] = i;
            SortedId[NowItems] = i;//NowItems;
            NowItems++;
        }
    else
        for(int i = 0;i < (int) HashLine[GetHash(ItemsChosen->currentText())].size();i++)
        {
            //PreId[NowItems] = HashLine[GetHash(ItemsChosen->currentText())][i];
            SortedId[NowItems] = HashLine[GetHash(ItemsChosen->currentText())][i];//NowItems;
            NowItems++;
        }
    quick_sort(SortedId,PreId,0,NowItems - 1,DateColumn,TimeColumn);
    for(int i = 0;i < NowItems;i++)
        PreId[i] = SortedId[i];
    std::sort(PreId,PreId+NowItems);
    QTableWidgetItem *Tmp[MAXITEM][20];
    for(int i = 0;i < NowItems;i++)
        for(int j = 0;j < Table->columnCount();j++)
            Tmp[PreId[i]][j] = new QTableWidgetItem(Table->item(PreId[i],j)->text());
    for(int i = 0;i < NowItems;i++)
    {
        for(int j = 0;j < Table->columnCount();j++)
        {
            Table->setItem(PreId[i],j,Tmp[SortedId[i]][j]);
            Table->item(PreId[i],j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        }
    }
}

int ListWidget::GetHash(QString S)
{
    int Sco = 0;
    for(int i = 0;i < S.size();i++)
        Sco = (Sco + S[i].unicode() * 33) % MAXITEM;
    return Sco;
}

int ListWidget::FindHash(QString S)
{
    for(int i = GetHash(S); i < MAXITEM ; i++)
    {
        if(Hash[i] == S)
            return i;
        if(Hash[i] == "")
            return -1;
    }
    return -1;
}

void ListWidget::InsertHash(QString S)
{
    for(int i = GetHash(S); i < GetHash(S)+MAXITEM ; i++)
    {
        if(Hash[i % MAXITEM] == "")
        {
            Hash[i % MAXITEM] = S;
            return ;
        }
    }
    return ;
}

void ListWidget::AddWidgetShow(int Width,int Height)
{
    Tim->stop();
    AW->show();
}


void ListWidget::ShowOptWidget(int row, int column)
{
    OW->setWindowFlags(OW->windowFlags() | Qt::WindowStaysOnTopHint);
    OW->setWindowModality(Qt::ApplicationModal);
    Tim->stop();
    OW->show();
    QString Data;
    for(int i = 0;i < Table->columnCount();i++)
    {
        Data = Data + Table->item(row,i)->text();
        if(i != Table->columnCount() - 1)
            Data = Data + " ";
    }
    QString TemData = Data;
    qDebug()<<Data;
    QRegExp Separator = QRegExp("年|月|日");
    QStringList TemList = Data.split(Separator,QString::SkipEmptyParts);
    if(Table->columnCount() == 7)
        Data = "Extracurricular " + TemList[0];
    if(Table->columnCount() == 4)
        Data = "Temporary " + TemList[0];
        Data = Data + " " + TemList[1] + " " + TemList[2] + TemList[3];
    OW->disconnect();
    connect(OW,&OptWidget::ItemDelete,[=](){
        if(Table->columnCount() == 7)
        {
            TxtDelete(UserExtracurricularPath,Data);
            TxtAdd(JournalPath,AddDataTime("用户 " + User + " 删除了课外活动 " + TemList[0] + " \n"),1);
        }
        if(Table->columnCount() == 4)
        {
            TxtDelete(UserTemporaryPath,Data);
            TxtAdd(JournalPath,AddDataTime("用户 " + User + " 删除了临时事务 " + TemList[0] + " \n"),1);
        }
        Tim->start();
    });
    connect(OW,&OptWidget::SetAlarm,[=](){
        int AimRow = FindInsertRow(TemData);
        if(AimRow == -1)
        {
            QMessageBox::critical(this, tr("设置失败"),  tr("活动已经开始或结束"));
            return ;
        }
        TxtAdd(UserAlarmPath,Data,2,AimRow);
        if(Table->columnCount() == 7)
            TxtAdd(JournalPath,AddDataTime("用户 " + User + " 为课外活动 " + TemList[0] + " 设置了闹钟\n"),1);
        if(Table->columnCount() == 4)
            TxtAdd(JournalPath,AddDataTime("用户 " + User + " 为临时事务 " + TemList[0] + " 设置了闹钟\n"),1);
        Tim->start();
    });
}
