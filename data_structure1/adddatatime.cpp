#include "adddatatime.h"
#include <QDateTime>

QString AddDataTime(QString data)
{
    QDateTime cur_time = QDateTime::currentDateTime();
    QString Time = cur_time.toString("yyyy.MM.dd hh:mm:ss ");
    return Time + data;
}
