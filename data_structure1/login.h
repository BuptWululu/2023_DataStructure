#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QString>
namespace Ui {
class login;
}

class login : public QWidget
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = 0);
    ~login();

private:
    Ui::login *ui;
};

QString GetInformation(QString Path);
#endif // LOGIN_H
