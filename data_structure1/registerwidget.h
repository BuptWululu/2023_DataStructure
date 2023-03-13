#ifndef REGISTERWIDGET_H
#define REGISTERWIDGET_H

#include <QWidget>

class RegisterWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RegisterWidget(QWidget *parent = nullptr);

signals:
    void back_login();

public slots:
};

#endif // REGISTERWIDGET_H
