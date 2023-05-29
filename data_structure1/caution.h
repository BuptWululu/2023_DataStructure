#ifndef CAUTION_H
#define CAUTION_H

#include <QWidget>
#include <QTableWidget>
class Caution : public QWidget
{
    Q_OBJECT
public:
    QTableWidget *Table = new QTableWidget(this);
    explicit Caution(QWidget *parent = nullptr);
    QString GetTip(QStringList Line);
protected:
    void closeEvent(QCloseEvent *event);
signals:

public slots:
    void slotCellEnter(QTableWidgetItem *Item);
};

#endif // CAUTION_H
