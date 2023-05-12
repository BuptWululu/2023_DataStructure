#include "txtadd.h"
#include <QFile>
#include <QTextStream>

void TxtAdd(QString Path, QString data,int flag,int row)
{
    QFile f(Path);
    if(flag == 0)
    {
        f.open(QIODevice::WriteOnly | QIODevice::Text); //覆盖写入
        QTextStream txtOutput(&f);
        txtOutput << data;
        f.close();
    }
    if(flag == 1)
    {
        if(row == -1)
        {
            f.open(QIODevice::ReadWrite | QIODevice::Append);   //以读写且追加的方式写入文本
            QTextStream txtOutput(&f);
            txtOutput << data;
            f.close();
        }
        else {
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
                             if(i == row)
                             {
                                 QString tempStr=strList.at(i);
                                 tempStr.replace(0,tempStr.length(),data);   //data是要替换的内容
                                 stream<<tempStr<<'\n';
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
    }
    if(flag == 2)
    {
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
                         if(i == row)
                             stream<<data<<'\n';
                         if(i==strList.count()-1)
                            stream<<strList.at(i);
                         else
                            stream<<strList.at(i)<<'\n';
                     }
             }
             writeFile.close();
    }
    return ;

}
