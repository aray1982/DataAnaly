#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QStringList>
#include"newview.h"
#include<qstandarditemmodel.h>
//#include<QPushButton>
#include<stdio.h>
#include<QTableView>
#include<QSplitter>

class Baseview;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    const QStringList& ReturnData();//return origData
private:

    int findemptydata();

private slots:
    void on_actionOpen_triggered();

    void on_actionExit_triggered();



private:
    Ui::MainWindow *ui;
    QStringList OrigData;
    NewView *m_chartview;
    QTableView *m_view;
    QStandardItemModel *m_model;
    QVector<int> m_sn;
    QSplitter *m_splitter;

public slots:
    void addData(QVector<QPointF> points);
    void clearData();


};

#endif // MAINWINDOW_H
