#include "setlabeltext.h"
#include <QLabel>
#include <QString>

void SetLabelText(QLabel *Now, QString data)
{
    Now->setAlignment(Qt::AlignTop|Qt::AlignLeft);
    Now->setFont(QFont("宋体",12));
    Now->setText(data);
}
