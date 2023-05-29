#include <bits/stdc++.h>
#include <QDebug>
#include "navigation.h"
#include "adddatatime.h"
#define N 200
extern struct Position Area[N];
extern struct Edge *Node[N];
extern int TotArea;
const double MaxD = 1e9;
double MinDist,ShortestDist[N];
bool Vis[N];
int PrePosition[N];
double GetDist(Position a,Position b)
{
    return std::sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));
}
double FindShortest(int Start,int End)
{
    for(int i=1;i<=TotArea;i++)
    {
        ShortestDist[i] = MaxD;
        Vis[i] = PrePosition[i] = 0;
    }
    if(Start == End)
    {
        MinDist = 0;
        return 0;
    }
    if(Start == 0||End == 0)
        return 0;
    ShortestDist[Start] = 0;
    for(int i=1;i<=TotArea;i++)
    {
        int Now = 0;
        double Minn = MaxD;
        for(int j=1;j<=TotArea;j++)
        {
            if(Minn > ShortestDist[j]&&!Vis[j])
            {
                Minn = ShortestDist[j];
                Now = j;
            }
        }
        //
        if(!Now) break;

        Vis[Now] = 1;
        for(Edge *j=Node[Now]->next;j!=NULL;j=j->next)
        {
            if(!Vis[j->Next_id]&&ShortestDist[j->Next_id]>ShortestDist[j->From]
                    +GetDist(Area[j->From],Area[j->Next_id]))
            {
                PrePosition[j->Next_id]=j->From;
                ShortestDist[j->Next_id]=ShortestDist[j->From]
                        +GetDist(Area[j->From],Area[j->Next_id]);
            }
        }
    }
    MinDist = ShortestDist[End];
    return MinDist;
}
