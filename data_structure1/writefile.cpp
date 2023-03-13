#include <QFile>
#include <QString>
#include <QTextStream>
#include "writefile.h"
#include "login.h"
void WriteFile(QFile *file,QString data)
{
    data=GetInformation()+data;
    if((*file).open(QIODevice::WriteOnly |QIODevice::Text)){
            QTextStream stream(file);
            stream<<data<<"\n";
            (*file).close();
        }
}
