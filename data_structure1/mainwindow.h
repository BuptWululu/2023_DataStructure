#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QTimer>
#include <QLabel>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

public:

signals:

private slots:
    void slotCellEnter(QTableWidgetItem *);
public slots:

};
struct SerTime{
    int year=2023,month=2,day=20,hour,minute,second,millisecond;
};
#endif // MAINWINDOW_H
