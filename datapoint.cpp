#include "datapoint.h"
#include<qdebug.h>

DataPoint::DataPoint(QObject *parent) : QObject(parent)
{

}


bool DataPoint::Adddata(int n,QGraphicsLineItem* line)
{
    if(m_da.count()<m_size)
    {
        DataInfo pipilu;
        pipilu.SN=n;
        pipilu.line=line;
        m_da.append(pipilu);
        return true;
    }
    else
    {
        delete line;
        return false;
    }
}

bool DataPoint::Cleardata()
{
    if(m_da.empty())
    {
        return false;
    }
    else
    {
        while(!m_da.empty())
        {
            DataInfo luxixi=m_da.takeFirst();
            delete luxixi.line;
        }
        return true;
    }
}

QVector<DataInfo> &DataPoint::GetData()
{
    return m_da;
}

bool DataPoint::Isempty()
{
    return m_da.empty();
}

bool DataPoint::Isfull()
{
    return m_da.count()==m_size;
}

bool DataPoint::Isexist(int n)
{
    if(m_da.empty())
    {
        return false;
    }
    else
    {
        for(int i=0;i!=m_da.count();i++)
        {
            if(m_da.at(i).SN==n)
            {
                return true;
            }
        }
        return false;
    }
}
