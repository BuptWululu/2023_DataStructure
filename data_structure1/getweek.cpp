#include "getweek.h"
#include "mainwindow.h"
#include <QDebug>
#include <QComboBox>
extern struct SerTime BeginTime;
extern int Month_Day[13];
extern int TotWeek;
int AimYear,AimMonth,AimDay;
int Calculate(int Y,int M,int D)
{
    //qDebug()<<Month_Day[M]<<Y<<M<<D;
    if(Y == AimYear&&M == AimMonth&&D == AimDay)
        return 0;
    if(M == 2 &&D == 29)
        return Calculate(Y,M+1,1)+1;
    if(Month_Day[M] == D)
    {
        if(((Y%400==0)||((Y%4==0)&&(Y%100!=0)))&&(M == 2))
            return Calculate(Y,M,D+1)+1;
        M++;
        if(M == 13)
        {
            M = 1;
            Y++;
        }
        return Calculate(Y,M,1)+1;
    }
    return Calculate(Y,M,D+1)+1;
}
int GetWeek(QComboBox *const *C)
{
    int NowYear = BeginTime.year;
    int NowMonth = BeginTime.month;
    int NowDay = BeginTime.day;
    AimYear = C[0]->currentText().toInt();
    AimMonth = C[1]->currentText().toInt();
    AimDay = C[2]->currentText().toInt();

    if(NowYear > AimYear)
        return 0;

    if(NowMonth > AimMonth&&NowYear == AimYear)
        return 0;

    if(NowDay > AimDay&&NowMonth == AimMonth&&NowYear == AimYear)
        return 0;
    //qDebug()<<NowYear<<NowMonth<<NowDay<<AimYear<<AimMonth<<AimDay;
    int Week = Calculate(NowYear,NowMonth,NowDay)/7+1;
    if(Week > TotWeek)
        return 0;
    else
        return Week;
}
