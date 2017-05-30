#ifndef DATAPOINT_H
#define DATAPOINT_H

#include <QObject>
#include<QGraphicsLineItem>
#include<QVector>

struct DataInfo{
    int SN;
    QGraphicsLineItem* line;
};

class DataPoint : public QObject
{
    Q_OBJECT
public:
    explicit DataPoint(QObject *parent = 0);
    bool Adddata(int n,QGraphicsLineItem* line);
    bool Cleardata();
    bool Isexist(int n);

    bool Isempty();
    bool Isfull();
    QVector<DataInfo>& GetData();


signals:

public slots:

private:
    QVector<DataInfo> m_da;
    static const int m_size=5;


};

#endif // DATAPOINT_H
