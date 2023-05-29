#ifndef ALARMWIDGET_H
#define ALARMWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>

class AlarmWidget : public QWidget
{
    Q_OBJECT

public:
    int NowItem = 0;
    QStringList DataList;
    QTableWidget *Table;
    QPushButton *NavigationPushButton = new QPushButton(this);
    AlarmWidget(int Width = 500,int Height = 500,QWidget *parent = nullptr);
    void TableInit();
    void InsertNextPeriod();
    void ClearEndl();
    int CompareTime(QStringList a,QStringList b);
    QString AddDay(QStringList Now,int AddNumber);
    void PushButtonInit(QPushButton *P,QString Text,int MoveWidth = 400,int MoveHeight = 240);
    bool FindAlarm(int row);
    QString GetTip(QStringList Line);
    int AlarmRow();
    void ShowAlarm();
protected:
    void closeEvent(QCloseEvent *event);
signals:

public slots:
    void slotCellEnter(QTableWidgetItem *Item);
    void slotNavigation();
};

#endif // ALARMWIDGET_H
