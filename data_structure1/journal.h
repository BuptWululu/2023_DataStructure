#ifndef JOURNAL_H
#define JOURNAL_H

#include <QWidget>

class Journal : public QWidget
{
    Q_OBJECT
public:
    explicit Journal(QWidget *parent = nullptr);
protected:
    void closeEvent(QCloseEvent *event);
signals:

public slots:
};

#endif // JOURNAL_H
