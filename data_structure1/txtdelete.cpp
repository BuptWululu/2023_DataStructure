#include "txtdelete.h"
#include <QFile>
#include <QTextStream>


void TxtDelete(QString Path, QString data)
{
    QFile f(Path);
    f.open((QIODevice::ReadOnly|QIODevice::Text));
    QString strAll;
    QTextStream stream(&f);
    strAll=stream.readAll();
    f.close();
    QFile writeFile(Path);	//PATH是自定义写文件的地址
         if(writeFile.open(QIODevice::WriteOnly|QIODevice::Text))
         {
                 QTextStream stream(&writeFile);
                 QStringList strList=strAll.split("\n");           //以换行符为基准分割文本
                 for(int i=0;i<strList.count();i++)    //遍历每一行
                 {
                     if(strList.at(i).contains(data))
                     {
                         continue;
                     }
                     else
                     {
                         if(i==strList.count()-1)
                         {
                            stream<<strList.at(i);
                         }
                         else
                         {
                            stream<<strList.at(i)<<'\n';
                         }
                     }
                 }
         }
         writeFile.close();
}
