#include "baseview.h"
#include<QStringList>
#include<QDebug>
#include<QDateTime>
#include <QtCharts/QLegend>
#include <QtCharts/QLegendMarker>
#include <QtCharts/QXYLegendMarker>

Baseview::Baseview(const QStringList &clist,QWidget *parent):QGraphicsView(new QGraphicsScene,parent),IsPressed(false),pPos(0,0)
{

    QLineSeries *series = new QLineSeries();
    QLineSeries *series1=new QLineSeries();

    for(auto x=clist.cbegin()+1;x!=clist.cend();x++)
    {
        QStringList luna;
        QString temp=*x;
        for(QString item:temp.split(","))
        {
            item.remove("\"");
            luna.push_back(item);
        }

        temp.clear();
        temp=luna[0];
        temp.append(luna[1]);
        QDateTime momentIntime=QDateTime::fromString(temp,"yyyy/MM/ddHH:mm:ss");
        series->append(momentIntime.toMSecsSinceEpoch(),Round(luna[3]));
        series1->append(momentIntime.toMSecsSinceEpoch(),Round(luna[9]));
    }

    series->setName(tr("tpv"));
    series1->setName(tr("hpv"));

    m_chart=new QChart;
    m_chart->setMinimumSize(500,500);
    //m_chart->setMaximumSize(200,200);
    //m_chart->legend()->hide();
    m_chart->addSeries(series);
    m_chart->addSeries(series1);
    m_chart->setTitle(tr("历史曲线"));
    m_chart->setVisible(true);
    m_chart->legend()->setAlignment(Qt::AlignBottom);
    m_chart->setAnimationOptions(QChart::SeriesAnimations);
    connectMarker();
    //m_chart->createDefaultAxes();
    QDateTimeAxis *axisX = new QDateTimeAxis;
    axisX->setTickCount(3);
    axisX->setFormat("MM/dd HH:mm:ss");
    axisX->setTitleText("Time");
    m_chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);
    series1->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis;
    axisY->setLabelFormat("%3.1f");
    axisY->setTitleText("Tpv");
    m_chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    QValueAxis *axisY1=new QValueAxis;
    axisY1->setMin(0.0);
    axisY1->setMax(100.0);
    axisY1->setLabelFormat("%3.1f");
    axisY1->setTitleText("Hpv");
    m_chart->addAxis(axisY1,Qt::AlignRight);
    series1->attachAxis(axisY1);


    scene()->addItem(m_chart);
    scene()->installEventFilter(this);
    connect(series,SIGNAL(hovered(QPointF,bool)),this,SLOT(toolTip(QPointF,bool)));
    this->setMouseTracking(true);
    //m_chart->installEventFilter(this);
    //setRubberBandSelectionMode();
}

Baseview::~Baseview()
{
    delete m_chart;
}

void Baseview::connectMarker()
{
    foreach (QLegendMarker* marker, m_chart->legend()->markers()) {
        // Disconnect possible existing connection to avoid multiple connections
        QObject::disconnect(marker, SIGNAL(clicked()), this, SLOT(handleMarkerClicked()));
        QObject::connect(marker, SIGNAL(clicked()), this, SLOT(handleMarkerClicked()));
}
}

void Baseview::handleMarkerClicked()
{
    QLegendMarker* marker = qobject_cast<QLegendMarker*> (sender());
    Q_ASSERT(marker);
//![3]

//![4]
    switch (marker->type())
//![4]
    {
        case QLegendMarker::LegendMarkerTypeXY:
        {
//![5]
        // Toggle visibility of series
        marker->series()->setVisible(!marker->series()->isVisible());

        // Turn legend marker back to visible, since hiding series also hides the marker
        // and we don't want it to happen now.
        marker->setVisible(true);
//![5]

//![6]
        // Dim the marker, if series is not visible
        qreal alpha = 1.0;

        if (!marker->series()->isVisible()) {
            alpha = 0.5;
        }

        QColor color;
        QBrush brush = marker->labelBrush();
        color = brush.color();
        color.setAlphaF(alpha);
        brush.setColor(color);
        marker->setLabelBrush(brush);

        brush = marker->brush();
        color = brush.color();
        color.setAlphaF(alpha);
        brush.setColor(color);
        marker->setBrush(brush);

        QPen pen = marker->pen();
        color = pen.color();
        color.setAlphaF(alpha);
        pen.setColor(color);
        marker->setPen(pen);

//![6]
        break;
        }
    default:
        {
        qDebug() << "Unknown marker type";
        break;
        }
    }
}

double Baseview::Round(const QString &str)
{
    double temp=str.toDouble();
    QString temp1=QString::number(temp,'f',1);
    return temp1.toDouble();

}

void Baseview::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Plus:
        m_chart->zoomIn();
        //qDebug()<<"IN";
        break;
    case Qt::Key_Minus:
        m_chart->zoomOut();
        //qDebug()<<"Out";
        break;
//![1]
    case Qt::Key_Left:
        m_chart->scroll(-10, 0);
        break;
    case Qt::Key_Right:
        m_chart->scroll(10, 0);
        break;
    case Qt::Key_Up:
        m_chart->scroll(0, 10);
        break;
    case Qt::Key_Down:
        m_chart->scroll(0, -10);
        break;
    case Qt::Key_Alt:
        m_chart->scale();
    default:
        QGraphicsView::keyPressEvent(event);
        break;
    }
}

/*bool Baseview::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type()==QEvent::GraphicsSceneWheel)
    {
        QGraphicsSceneWheelEvent *scrollevent =static_cast<QGraphicsSceneWheelEvent*>(event);
        handleWheelOnGraphicsScene(scrollevent);
        return true;
    }
    return false;
}

void Baseview::handleWheelOnGraphicsScene(QGraphicsSceneEvent *scrollevent)
{
    const int degrees = scrollevent->delta()  / 8;
       qDebug() << degrees;
       int steps = degrees / 15;
       qDebug() << steps;
       double scaleFactor = 1.0; //How fast we zoom
       const qreal minFactor = 1.0;
       const qreal maxFactor = 10.0;
       if(steps > 0)
       {
          h11 = (h11 >= maxFactor) ? h11 : (h11 + scaleFactor);
          h22 = (h22 >= maxFactor) ? h22 : (h22 + scaleFactor);
       }
       else
      {
          h11 = (h11 <= minFactor) ? minFactor : (h11 - scaleFactor);
          h22 = (h22 <= minFactor) ? minFactor : (h22 - scaleFactor);
      }
    //ui2->graphicsView->setTransformationAnchor();
    //ui2->graphicsView->setTransform(QTransform(h11, 0, 0,0, h22, 0, 0,0,1));
       this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
       this->setTransform(QTransform(h11, 0, 0,0, h22, 0, 0,0,1));


}
*/
void Baseview::wheelEvent(QWheelEvent *wheels)
{
    /*QPointF cici=wheels->angleDelta();
    if(cici.y()>0)
    {
        m_chart->zoom(1.5);
    }
    else if(cici.y()<0)
    {
        m_chart->zoom(0.5);
    }*/

        QGraphicsView::wheelEvent(wheels);

}

/*bool Baseview::eventFilter(QObject *obj, QEvent *event)
{
    if(obj==m_chart)
    {
        if(event->type()==QEvent::MouseButtonPress)
        {
            QMouseEvent *wEvent=static_cast<QMouseEvent*>(event);
            //if(m_chart->contains(wEvent->posF()))
            //    qDebug()<<"in";
            //else
            //    qDebug()<<"Out";
            //return true;
            QPointF c=wEvent->pos();
            qDebug()<<c.x();
            qDebug()<<c.y();
            return true;
        }
        else {
            return false;
        }
    }
    else
    {
        return QGraphicsView::eventFilter(obj,event);
    }
}*/

void Baseview::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        IsPressed=true;
        pPos=event->pos();
        qDebug()<<pPos.x();
    }
    else QGraphicsView::mousePressEvent(event);
}

void Baseview::mouseMoveEvent(QMouseEvent *event)
{
    if(IsPressed)
    {
        QPointF dist=event->pos()-pPos;
        dist=dist/10;
        m_chart->scroll(dist.x(),dist.y());
    }
    else
    {
        qDebug()<<event->pos().x();
        qDebug()<<event->pos().y();
        QRectF cixi=m_chart->boundingRect();
        qDebug()<<"Rect";
        //QPoint x=cixi.topLeft();
        //QPoint lt=mapToParent(x);
        qDebug()<<cixi.x();
        qDebug()<<cixi.y();
        qDebug()<<cixi.bottomRight().x();
        qDebug()<<cixi.bottomRight().y();
        QGraphicsView::mouseMoveEvent(event);
    }
}

void Baseview::mouseReleaseEvent(QMouseEvent *event)
{
    if(IsPressed)
        IsPressed=false;
    QGraphicsView::mouseReleaseEvent(event);
}

void Baseview::toolTip(QPointF pos, bool state)
{
    if(state)
    {
        qDebug()<<pos.x();
        qDebug()<<pos.y();
    }
}
