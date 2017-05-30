#ifndef NEWVIEW_H
#define NEWVIEW_H

#include <QObject>
#include <QWidget>
#include <QtCharts/QChartView>
#include <QtWidgets/QRubberBand>
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
#include <QtWidgets/QGraphicsTextItem>
#include<QGraphicsLineItem>
#include<QVector>
#include"datapoint.h"
#include<QSplineSeries>

QT_CHARTS_USE_NAMESPACE

class NewView : public QChartView
{
    Q_OBJECT
public:
    NewView(QWidget *parent = 0,const QStringList &clist=QStringList());
    ~NewView();

protected:

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);

private:
    QChart *m_chart;
    QSplineSeries *m_tpvseries;
    QSplineSeries *m_hpvseries;
    QSplineSeries *m_tsvseries;
    QSplineSeries *m_hsvseries;
    QGraphicsSimpleTextItem *m_coordX;
    QGraphicsSimpleTextItem *m_coordY;
    QPointF pos;
    DataPoint m_dap;
    double Round(const QString &str);
    bool SerieNo(qint64 times,const QLineSeries *seriex,int &result);//getserino
    QVector<QPointF> alignData(int sn,qint64 times);
    void connectMarker();
    bool Psure;
    void Drawline();
public slots:

    void handleMarkerClicked();
    void toolTip(QPointF pos,bool state);

signals:
    void newData(QVector<QPointF> points);
    void clearD();
};

#endif // NEWVIEW_H
