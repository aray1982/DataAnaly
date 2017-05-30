#ifndef BASEVIEW_H
#define BASEVIEW_H

#include <QWidget>
#include<QGraphicsView>
#include<QtCharts/QChartGlobal>
#include<QChart>
#include<QtCharts/QChart>
//QT_CHARTS_BEGIN_NAMESPACE
//class QChart;
//QT_CHARTS_END_NAMESPACE
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCore/QDateTime>
#include <QtCharts/QDateTimeAxis>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QDebug>
#include <QtCharts/QValueAxis>
#include<QStringList>
#include<QDateTime>
#include<QVector>
#include<QtCharts/QSplineSeries>
#include<QWheelEvent>


QT_CHARTS_USE_NAMESPACE

class MainWindow;

class Baseview : public QGraphicsView
{
    Q_OBJECT
public:
    Baseview(const QStringList &clist,QWidget *parent = 0);
    ~Baseview();

private:
    QChart *m_chart;
    bool IsPressed;
    QPointF pPos;
public slots:

    void handleMarkerClicked();
    void toolTip(QPointF pos,bool state);
    //void handleWheelOnGraphicsScene(QGraphicsSceneEvent *scrollevent);
    //void handleWheelevent(QWheelEvent *wheels);
private:
    double Round(const QString &str);
protected:
    void connectMarker();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *wheels);
    //bool eventFilter(QObject *obj, QEvent *event);
    //bool eventFilter(QObject *obj, QEvent *event);


};

#endif // BASEVIEW_H
