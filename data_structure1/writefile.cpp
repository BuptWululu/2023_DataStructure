#include <QFile>
#include <QString>
#include <QTextStream>
#include <QDebug>
#include "writefile.h"
#include "login.h"
void WriteFile(QString Path,QString data)
{
    data=GetInformation(Path)+data;
    QFile file(Path);
    if(file.open(QIODevice::WriteOnly |QIODevice::Text)){
            QTextStream stream(&file);
            stream<<data<<"\n";
            file.close();
        }
}
