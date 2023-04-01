#ifndef NAVIGATION_H
#define NAVIGATION_H

#include <QWidget>

class navigation : public QWidget
{
    Q_OBJECT
public:
    explicit navigation(QWidget *parent = nullptr);
    virtual void paintEvent(QPaintEvent *event);

signals:
    void back_mainwindow();

public slots:
};
struct Position{
    QString Name;
    int x,y;
};
struct Edge
{
    Edge *next;
    int From,Next_id,Congestion;
};
#endif // NAVIGATION_H
