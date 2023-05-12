#include "adddatatime.h"
#include "login.h"
extern QString TimePath;
extern QString ChangeLength(QString *T);
QString AddDataTime(QString data)
{
    QString Time = GetInformation(TimePath);
    QStringList Line = Time.split(" ");
    Time = Line[0]+"."+Line[1]+"."+Line[2]+" "+ChangeLength(&Line[3])+":"+ChangeLength(&Line[4])+":"+ChangeLength(&Line[5])+" ";
    return Time + data;
}
