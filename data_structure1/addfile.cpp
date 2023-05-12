#include "addfile.h"
#include "txtadd.h"
#include <QFile>
#include <QDebug>
void AddFile(QString Path,QString Data)
{
    QFile file(Path);
    file.open(QIODevice::WriteOnly);
    TxtAdd(Path,Data,1);
    file.close();
}
