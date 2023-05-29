#include "getpath.h"
#include "navigation.h"
#include "adddatatime.h"
#include <QString>
#include <QDebug>
#define N 200
extern struct Position Area[N];
extern double GetDist(Position a,Position b);
QString Dir[10]={"","东","南","西","北","东北","东南","西南","西北"};
int GetDirection(int pre,int now)
{
    if(Area[pre].y==Area[now].y)
    {
        if(Area[pre].x<Area[now].x)
            return 1;
        else
            return 3;
    }
    if(Area[pre].x==Area[now].x)
    {
        if(Area[pre].y<Area[now].y)
            return 2;
        else
            return 4;
    }
    if(Area[pre].x<Area[now].x)
    {
        if(Area[pre].y<Area[now].y)
            return 6;
        else
            return 5;
    }
    else
    {
        if(Area[pre].y<Area[now].y)
            return 7;
        else
            return 8;
    }
    return 0;
}
int Para;
QString Pre;
QString GetPath(int *PrePosition, int now,int BeginPara )
{
    if(!PrePosition[now])
    {
        Pre = "";
        Para = BeginPara;
        return "";
    }
    QString dir = Dir[GetDirection(PrePosition[now],now)];
    QString line = GetPath(PrePosition,PrePosition[now],BeginPara);
    if(Pre==dir)
    {
       int pos,len=0;
       for(int i=line.size();i>0;i--)
       {
           if(line[i]=="进")
           {
               pos=i;
               break;
           }
       }
       for(int i=pos+1;i;i++)
       {
           if(line[i]<'0'||line[i]>'9')
               break;
           len++;
       }
       int dist = (line.mid(pos+1,len)).toInt()+(int)GetDist(Area[PrePosition[now]],Area[now]);
       line.replace(pos+1,len,QString::number(dist));
       return line;
    }
    else
    {
        Para++;
        Pre = dir;
        return line+QString::number(Para)+"、向"+dir+"前进"+
                QString::number((int)GetDist(Area[PrePosition[now]],Area[now]))+"米\n";
    }
}
