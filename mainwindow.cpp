#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Multiplier1 = 1;
    Multiplier2 = 1;
    Multiplier3 = 1;

    ui->lineEdit1->setText(QString::number(Multiplier1));
    ui->lineEdit2->setText(QString::number(Multiplier2));
    ui->lineEdit3->setText(QString::number(Multiplier3));

    DrawGraph1();
}

void MainWindow::on_myButton_clicked()
{
    bool ok;
    Multiplier1 = ui->lineEdit1->text().toInt(&ok);
    qDebug()<<"lineEdit1 ok"<<ok;
    Multiplier2 = ui->lineEdit2->text().toInt(&ok);
    qDebug()<<"lineEdit2 ok"<<ok;
    Multiplier3 = ui->lineEdit3->text().toInt(&ok);
    qDebug()<<"lineEdit3 ok"<<ok;

    DrawGraph1();
    ui->widget->replot();
}

void MainWindow::DrawGraph()
{
    QVector<double> x( 1000 + Multiplier1), y( 1000 + Multiplier1 ); // initialize with entries 0..100
        for (int i=0; i<1000 + Multiplier1; ++i)
        {
            x[i] = i/50.0 - 5; // x goes from -1 to 1
            y[i] = x[i] * x[i];  // let's plot a quadratic function
        }

        ui->widget->addGraph();
        ui->widget->graph(0)->setData( x, y );

        ui->widget->xAxis->setLabel( "x" );
        ui->widget->yAxis->setLabel( "y" );

        ui->widget->xAxis->setRange( -10, 10 );
        ui->widget->yAxis->setRange( 0, 10 );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::DrawGraph1()
{
    // add two new graphs and set their look:
    ui->widget->clearGraphs();
    qDebug()<<"graphCount"<<ui->widget->graphCount();
    ui->widget->addGraph();
    ui->widget->graph(0)->setPen(QPen(Qt::blue)); // line color blue for first graph
    ui->widget->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20))); // first graph will be filled with translucent blue
    ui->widget->addGraph();
    ui->widget->graph(1)->setPen(QPen(Qt::red)); // line color red for second graph
    // generate some points of data (y0 for first, y1 for second graph):
    QVector<double> x(251), y0(251), y1(251);
    for (int i=0; i<251; ++i)
    {
      x[i] = i;
      y0[i] = qExp(-i/(10.00 + Multiplier3))*qCos(i/(1.0 + Multiplier2)); // exponentially decaying cosine
      y1[i] = qExp(-i/(10.00 + Multiplier3));              // exponential envelope
    }
    // configure right and top axis to show ticks but no labels:
    // (see QCPAxisRect::setupFullAxesBox for a quicker method to do this)
    ui->widget->xAxis2->setVisible(true);
    ui->widget->xAxis2->setTickLabels(false);
    ui->widget->yAxis2->setVisible(true);
    ui->widget->yAxis2->setTickLabels(false);
    // make left and bottom axes always transfer their ranges to right and top axes:
    connect(ui->widget->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->widget->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->widget->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->widget->yAxis2, SLOT(setRange(QCPRange)));
    // pass data points to graphs:
    ui->widget->graph(0)->setData(x, y0);
    ui->widget->graph(1)->setData(x, y1);
    // let the ranges scale themselves so graph 0 fits perfectly in the visible area:
    ui->widget->graph(0)->rescaleAxes();
    // same thing for graph 1, but only enlarge ranges (in case graph 1 is smaller than graph 0):
    ui->widget->graph(1)->rescaleAxes(true);
    // Note: we could have also just called customPlot->rescaleAxes(); instead
    // Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:
    ui->widget->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
}
