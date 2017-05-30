#include "newview.h"
#include<QStringList>
#include<QDebug>
#include<QDateTime>
#include <QtCharts/QLegend>
#include <QtCharts/QLegendMarker>
#include <QtCharts/QXYLegendMarker>
#include<QVector>

NewView::NewView(QWidget *parent,const QStringList &clist):QChartView(parent),m_dap(parent)
{
    m_tpvseries = new QSplineSeries(this);
    m_hpvseries=new QSplineSeries(this);
    m_tsvseries = new QSplineSeries(this);
    m_hsvseries=new QSplineSeries(this);

    if(!clist.isEmpty())
    {

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
        m_tpvseries->append(momentIntime.toMSecsSinceEpoch(),Round(luna[3]));
        m_hpvseries->append(momentIntime.toMSecsSinceEpoch(),Round(luna[9]));
        m_tsvseries->append(momentIntime.toMSecsSinceEpoch(),Round(luna[2]));
        m_hsvseries->append(momentIntime.toMSecsSinceEpoch(),Round(luna[8]));
    }
    }

    m_tpvseries->setName(tr("tpv"));
    m_hpvseries->setName(tr("hpv"));
    m_tsvseries->setName(tr("tsv"));
    m_hsvseries->setName(tr("hsv"));

    m_chart=new QChart();

    m_chart->addSeries(m_tpvseries);
    m_chart->addSeries(m_hpvseries);
    m_chart->addSeries(m_hsvseries);
    m_chart->addSeries(m_tsvseries);
    m_chart->setTitle(tr("Historical curve"));
    m_chart->setVisible(true);
    m_chart->legend()->setAlignment(Qt::AlignBottom);
    m_chart->setAnimationOptions(QChart::AllAnimations);
    connectMarker();

    QDateTimeAxis *axisX = new QDateTimeAxis;
    axisX->setTickCount(4);
    axisX->setFormat("MM/dd HH:mm:ss");
    axisX->setTitleText("Time");
     m_chart->addAxis(axisX, Qt::AlignBottom);

    m_tpvseries->attachAxis(axisX);
    m_hpvseries->attachAxis(axisX);
    m_hsvseries->attachAxis(axisX);
    m_tsvseries->attachAxis(axisX);


    QValueAxis *axisY = new QValueAxis;//temperatrue axis
    axisY->setLabelFormat("%3.1f");
    axisY->setTitleText(tr("Temperature"));
    m_chart->addAxis(axisY, Qt::AlignLeft);
    m_tpvseries->attachAxis(axisY);
    m_tsvseries->attachAxis(axisY);

    QValueAxis *axisY1=new QValueAxis;//humidity axis
    axisY1->setMin(0.0);
    axisY1->setMax(100.0);
    axisY1->setLabelFormat("%3.1f");
    axisY1->setTitleText(tr("Humidity"));
    m_chart->addAxis(axisY1,Qt::AlignRight);
    m_hpvseries->attachAxis(axisY1);
    m_hsvseries->attachAxis(axisY1);

    m_coordX = new QGraphicsSimpleTextItem(m_chart);

    m_coordY = new QGraphicsSimpleTextItem(m_chart);

    m_chart->legend()->setAlignment(Qt::AlignTop);
    m_chart->setMinimumSize(400,400);
    this->setChart(m_chart);



    connect(m_tpvseries,SIGNAL(hovered(QPointF,bool)),this,SLOT(toolTip(QPointF,bool)));
    connect(m_hpvseries,SIGNAL(hovered(QPointF,bool)),this,SLOT(toolTip(QPointF,bool)));
    connect(m_tsvseries,SIGNAL(hovered(QPointF,bool)),this,SLOT(toolTip(QPointF,bool)));
    connect(m_hsvseries,SIGNAL(hovered(QPointF,bool)),this,SLOT(toolTip(QPointF,bool)));

    setRubberBand(QChartView::RectangleRubberBand);
    installEventFilter(this);

}

NewView::~NewView()
{
    delete m_chart;


}

void NewView::connectMarker()
{
    foreach (QLegendMarker* marker, m_chart->legend()->markers()) {
        // Disconnect possible existing connection to avoid multiple connections
        QObject::disconnect(marker, SIGNAL(clicked()), this, SLOT(handleMarkerClicked()));
        QObject::connect(marker, SIGNAL(clicked()), this, SLOT(handleMarkerClicked()));
}
}

void NewView::handleMarkerClicked()
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

double NewView::Round(const QString &str)
{
    double temp=str.toDouble();
    QString temp1=QString::number(temp,'f',1);
    return temp1.toDouble();

}

void NewView::keyPressEvent(QKeyEvent *event)
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
        break;

    default:
        QChartView::keyPressEvent(event);
        break;
    }
}

void NewView::wheelEvent(QWheelEvent *wheels)
{
    if(Psure==true)
    {

        wheels->accept();
    }
    else
    {
    QPointF cici=wheels->angleDelta();
    if(cici.y()>0)
    {
        m_chart->zoom(1.5);
    }
    else if(cici.y()<0)
    {
        m_chart->zoom(0.5);
    }

        QChartView::wheelEvent(wheels);
    }

}

void NewView::mousePressEvent(QMouseEvent *event)
{

    if(event->button()==Qt::LeftButton)
    {
        pos=event->pos();

    }
        else if(event->button()==Qt::MiddleButton)
    {
        m_chart->zoomReset();
        Psure=false;
        emit clearD();
        if(!m_dap.Isempty())
        {
            m_dap.Cleardata();
        }
        setRubberBand(QChartView::RectangleRubberBand);
    }
    QChartView::mousePressEvent(event);

}

void NewView::mouseMoveEvent(QMouseEvent *event)
{

        if(m_chart->contains(event->pos()))
        {
            QPointF Time=m_chart->mapToValue(event->pos(),m_tpvseries);
            QDateTime xone=QDateTime::fromMSecsSinceEpoch(Time.x());
            QString temp1(tr("Time: "));
            temp1.append(xone.toString("MM/dd HH:mm:ss"));
            m_coordX->setPos(10,25);
            m_coordX->setText(temp1);
            m_coordX->show();

        }
        else
        {
            m_coordX->hide();
        }
        QChartView::mouseMoveEvent(event);

}

void NewView::mouseReleaseEvent(QMouseEvent *event)
{

    if(event->button()==Qt::LeftButton)
    {
    if(pos==event->pos())
    {
        Psure=true;
        setRubberBand(QChartView::NoRubberBand);
        QRectF xlove=m_chart->plotArea();
         if(xlove.contains(pos))
            {
             QPointF xluna=m_chart->mapToValue(pos,m_tpvseries);
             int tno;
             if(SerieNo(xluna.x(),m_tpvseries,tno))
             {
                 qreal x=m_chart->mapToPosition(m_tpvseries->at(tno),m_tpvseries).x();
                 qreal top=m_chart->plotArea().top();
                 qreal bottom=m_chart->plotArea().bottom();
                 QPointF f(x,top);
                 QPointF s(x,bottom);
                 QLineF XY(f,s);
                 QPen luna(Qt::red);
                 if(!m_dap.Isfull())
                 {
                     if(!m_dap.Isexist(tno))
                     {
                      QGraphicsLineItem *line=scene()->addLine(XY,luna);
                      m_dap.Adddata(tno,line);
                      QVector<QPointF> beita;
                      QPointF suke=m_tsvseries->at(tno);
                      beita.append(suke);
                      suke=m_tpvseries->at(tno);
                      beita.append(suke);
                      suke=m_hsvseries->at(tno);
                      beita.append(suke);
                      suke=m_hpvseries->at(tno);
                      beita.append(suke);
                      emit newData(beita);
                     }
                 }
    }
    }


           }
    }
    QChartView::mouseReleaseEvent(event);

}

void NewView::toolTip(QPointF pos, bool state)
{
    if(state)
    {
        if(sender()==m_tpvseries)
        {


            QString temp2(tr("TPV: "));
            QString tpv=QString("%1").arg(pos.y(),0,'f',1);

            temp2.append(tpv);        
            m_coordY->setPos(150,25);
            m_coordY->setText(temp2);
            m_coordY->show();


        }
        else if(sender()==m_tsvseries)
        {
            QString temp2(tr("TSV: "));
            QString tsv=QString("%1").arg(pos.y(),0,'f',1);

            temp2.append(tsv);
            m_coordY->setPos(150,25);
            m_coordY->setText(temp2);
            m_coordY->show();


        }
        else if(sender()==m_hpvseries)
            {

            QString temp2(tr("HPV: "));
            QString hpv=QString("%1").arg(pos.y(),0,'f',1);
            temp2.append(hpv);          
            m_coordY->setPos(150,25);
            m_coordY->setText(temp2);
            m_coordY->show();
        }
        else if(sender()==m_hsvseries)
            {

            QString temp2(tr("HSV: "));
            QString hsv=QString("%1").arg(pos.y(),0,'f',1);
            temp2.append(hsv);
            m_coordY->setPos(150,25);
            m_coordY->setText(temp2);
            m_coordY->show();
        }

}
    else
       {
        m_coordY->hide();
    }
}



bool NewView::SerieNo(qint64 times, const QLineSeries *seriex, int &result)
{
    int tn=seriex->count();
    result=-1;
    if(seriex->at(0).x()>times)
    {
        return false;
    }
    else
    {
        for(int i=0;i!=tn;i++)
    {
        if(seriex->at(i).x()<times)
            continue;
        else
        {
            result=i;
            break;
        }
    }
    if(result==-1)
        return false;
    else
        return true;
    }
}

bool NewView::eventFilter(QObject *obj, QEvent *event)
{
    if(Psure==true)
    {
        if(obj==this)
        {
            if(event->type()==QEvent::KeyPress)
            {
                return true;
            }
            else if(event->type()==QEvent::Resize)
            {
                if(Psure)
                {
                    if(!m_dap.Isempty())
                    {
                        QVector<DataInfo> suke=m_dap.GetData();
                        for(int i=0;i!=suke.count();i++)
                        {
                            DataInfo temp=suke.at(i);
                            qreal x=m_chart->mapToPosition(m_tpvseries->at(temp.SN),m_tpvseries).x();
                            qreal top=m_chart->plotArea().top();
                            qreal bottom=m_chart->plotArea().bottom();
                            QPointF f(x,top);
                            QPointF s(x,bottom);
                            QLineF XY(f,s);
                            temp.line->setLine(XY);
                        }
                    }
                }

                return false;
            }
            else return false;
        }
        else
            return false;
    }
    else
        return false;
}

QVector<QPointF> NewView::alignData(int sn,qint64 times)
{
    QVector<QPointF> temp;
    if(sn==(m_tpvseries->count()-1))
    {
        temp.append(m_tsvseries->at(sn));
        temp.append(m_tpvseries->at(sn));
        temp.append(m_hsvseries->at(sn));
        temp.append(m_hpvseries->at(sn));
    }
    else
    {
        qint64 start=m_tpvseries->at(sn).x();
        qint64 end=m_tpvseries->at(sn+1).x();
        qreal xscale=(end-times)/(end-start);
        qreal svalue=m_tsvseries->at(sn).y();
        qreal evalue=m_tsvseries->at(sn+1).y();
        qreal fvalue=(svalue-evalue)*xscale+evalue;
        QPointF tick;
        tick.setX(times);
        tick.setY(fvalue);
        temp.append(tick);
        svalue=m_tpvseries->at(sn).y();
        evalue=m_tpvseries->at(sn+1).y();
        fvalue=(svalue-evalue)*xscale+evalue;
        tick.setX(times);
        tick.setY(fvalue);
        temp.append(tick);
        svalue=m_hsvseries->at(sn).y();
        evalue=m_hsvseries->at(sn+1).y();
        fvalue=(svalue-evalue)*xscale+evalue;
        tick.setX(times);
        tick.setY(fvalue);
        temp.append(tick);
        svalue=m_hpvseries->at(sn).y();
        evalue=m_hpvseries->at(sn+1).y();
        fvalue=(svalue-evalue)*xscale+evalue;
        tick.setX(times);
        tick.setY(fvalue);
        temp.append(tick);
    }
    return temp;
}

