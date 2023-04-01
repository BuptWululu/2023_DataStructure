#include "txtadd.h"
#include <QFile>
#include <QTextStream>

void TxtAdd(QString Path, QString data)
{
    QFile f(Path);
    f.open(QIODevice::ReadWrite | QIODevice::Append);   //以读写且追加的方式写入文本
    QTextStream txtOutput(&f);
    txtOutput << data;
    f.close();
}
