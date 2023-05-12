#ifndef ALARMNAVIGATION_H
#define ALARMNAVIGATION_H

#include <QWidget>
#include "navigation.h"
#include <QLineEdit>
#include <QLabel>
#include <vector>

class AlarmNavigation : public navigation
{
    Q_OBJECT
public:
    QLineEdit *LineEditStart = new QLineEdit(this);
    std::vector<QStringList> PassPositionList;
    QStringList AimPositionList;
    QString Tip;
    int PreRow,NowRow;
    std::vector<int> PassPositionId;
    explicit AlarmNavigation(int preRow,int nowRow);
    virtual void paintEvent(QPaintEvent *event);
    void FindAllCombination(QStringList PositionList,int Row);
    void FindAimPositionList();
    void FindPassPosition();
    void Recursion(int Now);
    void Init();
    void LineEditInit(QLineEdit *L,int MoveWidth,int MoveHeight,int SizeWidth = 200,int SizeHeight = 30);
signals:

public slots:
};

#endif // ALARMNAVIGATION_H
