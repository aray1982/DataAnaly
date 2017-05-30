 #include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QFileDialog>
#include<QMessageBox>
#include<QFile>
#include<QTextStream>
#include<QDebug>
#include<QStringList>
#include<QSplitter>
#include<QListView>
#include<QTableView>
#include<QStandardItemModel>
#include<QIcon>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_chartview(new NewView(parent)),
    OrigData(QStringList()),
    m_model(new QStandardItemModel(5,5,parent)),
    m_view(new QTableView())
{
    ui->setupUi(this);
    m_view->setAlternatingRowColors(true);

    m_model->setHorizontalHeaderLabels(QStringList()<<tr("Time")<<tr("Tsv")<<tr("Tpv")<<tr("Hsv")<<tr("Hpv"));
    this->setWindowTitle(tr("DataAnaly"));
    QIcon Micon(":/images/MIcon");
    this->setWindowIcon(Micon);




}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_view;
    if(m_chartview)
        delete m_chartview;

}

void MainWindow::on_actionOpen_triggered()
{
    QString filename=QFileDialog::getOpenFileName(
                this,
                tr("Open CSV File"),
                "C://",
                "CSV files (*.csv)"
                );
    QFile OrigFile(filename);
    bool Isright=false;
     if ( !OrigFile.open(QFile::ReadOnly | QFile::Text) )
     {
         QMessageBox::critical(this,tr("Failure"),tr("Please check the file"),QMessageBox::Close,QMessageBox::Close);
     }
     else
     {
         QTextStream in(&OrigFile);


         if(!OrigData.isEmpty())
         OrigData.clear();
     while(!in.atEnd())
        {

         OrigData.push_back(in.readLine());


         }
     if(OrigData.isEmpty())
     {
         QMessageBox::critical(this,tr("Failure"),tr("The File is Empty!"),QMessageBox::Close,QMessageBox::Close);
     OrigData.clear();
     OrigFile.close();

     }
     else
     {
     QString Line=OrigData[0];
     QStringList Temp;
     for(QString item:Line.split(","))
     {
         item.remove("\"");
         Temp.push_back(item);
     }

     QStringList Compare=(QStringList()<<"Date"<<"Time"<<"TSV"<<"TPV"<<"HSV"<<"HPV");

     for(int i=0;i!=6;i++)
     {
         if(Compare[i]==Temp[i])
             Isright=true;
         else
         {
             Isright=false;
             break;
         }
     }

         if(!Isright)
           {
             QMessageBox::critical(this,tr("Failure"),tr("The File is not correct!"),QMessageBox::Close,QMessageBox::Close);
         OrigData.clear();
         }

     }

     if(Isright)
     {


         if(m_chartview)
        {
         delete m_chartview;
         m_chartview=new NewView(this,ReturnData());
         clearData();
         m_chartview->setRenderHint(QPainter::Antialiasing);

         m_splitter=new QSplitter(Qt::Vertical,this);


         m_view->setModel(m_model);
         m_splitter->addWidget(m_chartview);
         m_splitter->addWidget(m_view);

         m_splitter->show();
         this->setCentralWidget(m_splitter);
         QObject::connect(m_chartview,&NewView::newData,this,&MainWindow::addData);
          QObject::connect(m_chartview,&NewView::clearD,this,&MainWindow::clearData);
     }
     }
     }

}

const QStringList& MainWindow::ReturnData()
{
    return OrigData;
}


void MainWindow::on_actionExit_triggered()
{
    close();
}



void MainWindow::addData(QVector<QPointF> points)
{
    if(points.count()>0)
    {
        QString time;
        QVector<qreal> value;
        QDateTime times;

        for(int i=0;i<points.count();i++)
        {
            QPointF tifa=points[i];
            time=((QDateTime::fromMSecsSinceEpoch(tifa.x())).toString("MM/dd HH:mm:ss"));
            times=QDateTime::fromMSecsSinceEpoch(tifa.x());
            value.append(tifa.y());
        }

        int sn=findemptydata();


            QModelIndex index=m_model->index(sn,0,QModelIndex());
            m_model->setData(index,times);
            for(int i=1;i<m_model->columnCount();i++)
            {
                QModelIndex x=m_model->index(sn,i,QModelIndex());
                m_model->setData(x,value[i-1]);
            }
            m_model->sort(0);


    }


}

int MainWindow::findemptydata()
{

    int limit=5;
    int i=0;
    for(i;i<limit;i++)
    {
      QModelIndex x=m_model->index(i,0,QModelIndex());
      QVariant temp=m_model->data(x);
      if(temp.isNull())
      {
          break;
      }
      else continue;
    }
    return i;
}

void MainWindow::clearData()
{
    int rc=m_model->rowCount();
    for(int r=rc;r>=0;r--)
    {
        m_model->removeRow(r);
    }
    for(int t=0;t<rc;t++)
    {
        m_model->insertRow(t);
    }

}


