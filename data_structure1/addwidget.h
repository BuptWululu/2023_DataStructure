#ifndef ADDWIDGET_H
#define ADDWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#define N 50
class AddWidget : public QWidget
{
    Q_OBJECT
public:
    int TotNumber = 0,LineEditNumber = 0,ComboBoxNumber = 0,PushButtonNumber = 0;
    QLineEdit *LineEdit[N];
    QComboBox *ComboBox[N];
    QPushButton *PushButton[N];
    AddWidget(int Width = 400,int Height = 600,QWidget *parent = nullptr);
    void AddLabel(int MoveWidth,int MoveHeight, QString Data);
    void AddLineEdit(int MoveWidth,int MoveHeight);
    void BuildDateConnect(QComboBox *Y,QComboBox *M,QComboBox *D);
    QComboBox* AddComboBox(int MoveWidth,int MoveHeight, int Begin, int End, QString Suffix = "", int SizeWidth = 100,int SizeHeight = 20);
    void AddDateComboBox(int MoveWidth,int MoveHeight);
    void AddPushButton(int MoveWidth,int MoveHeight,QStringList Name);
    void AddInsertPushButton(int MoveWidth, int MoveHeight, QString PushButtonName, QString FilePath,QString Prefix);
    void AddExamPushButton(int MoveWidth, int MoveHeight, QString PushButtonName);
    bool FindConflict(QStringList InsertItems,QString Path = "");
    int FindDateDistance(QStringList InsertItems,QStringList Line,int BeginColumn,int EndColumn);
    int FindEarlierDate(QStringList InsertItems,QStringList Line,int BeginColumn,int EndColumn);
    bool FindCurriculumConflict(QStringList InsertItems,QString FilePath,int BeginColumn,int EndColumn);
    bool FindOtherConflict(QStringList InsertItems,QString FilePath,int BeginColumn,int EndColumn,int FrequencyColumn = -1);
    ~AddWidget();
protected:
    void closeEvent(QCloseEvent *event);
signals:

public slots:
};

#endif // ADDWIDGET_H
