#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include "addwidget.h"
#include "optwidget.h"
#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QComboBox>
#include <vector>
#include <map>
#define MAXITEM 10000

class ListWidget : public QWidget
{
    Q_OBJECT
private:
    QString WeekName[8]={0,"星期一","星期二","星期三","星期四","星期五","星期六","星期日"};

public:
    QTableWidget *Table ;
    OptWidget *OW;
    QPushButton *SavePushButton,*AddPushButton,*SortPushButton;
    QComboBox *ItemsChosen;
    QString Hash[MAXITEM];
    std::vector<int> HashLine[MAXITEM];
    ListWidget(int Width,int Height,QWidget *parent = nullptr);
    AddWidget *AW;
    int GetFileLine(QString FilePath);
    void Init();
    void TableInit(QString FilePath,int Column);
    void ChangedTableEvent();
    void PushButtonInit(QString Text,int Width = 150 ,int Height = 60);
    void SetTableHeader(QStringList List);
    void SetTableItems(QString FilePath,int BeginColumn,int EndColumn,int WeekNameColumn,int DateColumn = -1,int WeekColumn = -1,int TimeColumn = -1);
    void AddItemsChosenComboBox(int ChosenColumn,int MoveWidth = 100,int MoveHeight = 0);
    void AddSortPushButton(QString Text,int DateColumn, int TimeColumn,int MoveWidth = 300, int MoveHeight = 0);
    void AddSavePushButton(QString Text, QString Path,int MoveWidth = 900, int MoveHeight = 300);
    void qSort(int DateColumn,int TimeColumn);
    void quick_sort(int *SortedId,int *PreId,int low,int high,int DateColumn, int TimeColumn);
    int CompareTime(int aRow,int bRow,int DateColumn, int TimeColumn);
    int FindInsertRow(QString data);
    int GetHash(QString S);
    int FindHash(QString S);
    void InsertHash(QString S);
signals:

public slots:
    void AddWidgetShow(int Width = 400,int Height = 600);
    void ShowOptWidget(int row,int column);
};

#endif // LISTWIDGET_H
