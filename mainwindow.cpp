#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    time(0)
{
    ui->setupUi(this);

    screen = QGuiApplication::primaryScreen();
    screenGeometry = screen->geometry();

    int width = screenGeometry.width();
    int heigth = screenGeometry.height();

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(newSecond()));
    timer->start(100);

    QMainWindow::showFullScreen();

    sensor.generate_temp(sensor.get_temp1(), sensor.get_temp2(), sensor.get_temp3(), sensor.get_temp4(), 10);

    ui->gridLayout_3->setHorizontalSpacing(50);
    ui->gridLayout_3->setVerticalSpacing(20);
    ui->gridLayout_3->setRowStretch(1, 20);
    ui->gridLayout_3->setRowStretch(3, 20);

    ui->gridLayout_5->setColumnStretch(0, width * 0.25);
    ui->gridLayout_5->setColumnStretch(1, width * 0.50);
    ui->gridLayout_5->setColumnStretch(2, width * 0.25);

    ui->pushButton->setFixedHeight(heigth * 0.07);
    ui->pushButton_2->setFixedHeight(ui->pushButton->height());

    ui->progressBar->setFixedHeight(ui->pushButton->height());
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(300);

    ui->lcdNumber_1->display(sensor.get_temp1());
    ui->lcdNumber_2->display(sensor.get_temp2());
    ui->lcdNumber_3->display(sensor.get_temp3());
    ui->lcdNumber_4->display(sensor.get_temp4());

    ui->chart->setBackground(QBrush(QColor(31, 31, 31)));
    ui->chart->axisRect()->setupFullAxesBox();
    connect(ui->chart->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->chart->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->chart->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->chart->yAxis2, SLOT(setRange(QCPRange)));

    ui->chart->yAxis->setBasePen(QPen(QColor(89, 171, 227)));
    ui->chart->yAxis->setTickPen(QPen(QColor(89, 171, 227)));
    ui->chart->yAxis->setTickLabelColor(QColor(89, 171, 227));
    ui->chart->yAxis->setSubTickPen(QPen(QColor(89, 171, 227)));
    ui->chart->xAxis->setBasePen(QPen(QColor(89, 171, 227)));
    ui->chart->xAxis->setTickPen(QPen(QColor(89, 171, 227)));
    ui->chart->xAxis->setTickLabelColor(QColor(89, 171, 227));
    ui->chart->xAxis->setSubTickPen(QPen(QColor(89, 171, 227)));

    ui->chart->yAxis2->setBasePen(QPen(QColor(89, 171, 227)));
    ui->chart->yAxis2->setTickPen(QPen(QColor(89, 171, 227)));
    ui->chart->yAxis2->setTickLabelColor(QColor(89, 171, 227));
    ui->chart->yAxis2->setSubTickPen(QPen(QColor(89, 171, 227)));
    ui->chart->xAxis2->setBasePen(QPen(QColor(89, 171, 227)));
    ui->chart->xAxis2->setTickPen(QPen(QColor(89, 171, 227)));
    ui->chart->xAxis2->setTickLabelColor(QColor(89, 171, 227));
    ui->chart->xAxis2->setSubTickPen(QPen(QColor(89, 171, 227)));

    ui->chart->xAxis->grid()->setVisible(false);
    ui->chart->yAxis->grid()->setVisible(false);

    ui->chart->addGraph();
    ui->chart->addGraph();
    ui->chart->addGraph();
    ui->chart->addGraph();
    ui->chart->graph(0)->setPen(QPen(QColor(254, 198, 6)));
    ui->chart->graph(1)->setPen(QPen(QColor(240, 73, 3)));
    ui->chart->graph(2)->setPen(QPen(QColor(151, 206, 104)));
    ui->chart->graph(3)->setPen(QPen(QColor(136, 112, 255)));
    ui->chart->graph(0)->setName("Temperatura 1");
    ui->chart->graph(1)->setName("Temperatura 2");
    ui->chart->graph(2)->setName("Temperatura 3");
    ui->chart->graph(3)->setName("Temperatura 4");
    ui->chart->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssPlus, 5));
    ui->chart->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssPlus, 5));
    ui->chart->graph(2)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssPlus, 5));
    ui->chart->graph(3)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssPlus, 5));
    ui->chart->yAxis->setRange(-5, 30);

    ui->chart->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->chart->xAxis->setDateTimeFormat("hh:mm:ss");

    QCPLayoutGrid *subLayout = new QCPLayoutGrid;
    ui->chart->plotLayout()->addElement(1, 0, subLayout);
//    subLayout->addElement(0, 0, new QCPLayoutElement);
    subLayout->addElement(0, 0, ui->chart->legend);
//    subLayout->addElement(0, 2, new QCPLayoutElement);
    ui->chart->plotLayout()->setRowStretchFactor(1, 0.001);

    ui->chart->legend->setBrush(QBrush(QColor(31, 31, 31)));
    ui->chart->legend->setBorderPen(QPen(QColor(31, 31, 31)));
    ui->chart->legend->setTextColor(QColor(89, 171, 227));
//    ui->chart->legend->setFillOrder(QCPLayoutGrid::FillOrder::foRowsFirst);
    ui->chart->legend->setVisible(true);

    drawLinesOnGraph();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    sensor.generate_temp(sensor.get_temp1(), sensor.get_temp2(), sensor.get_temp3(), sensor.get_temp4(), 1);

    ui->lcdNumber_1->display(sensor.get_temp1());
    ui->lcdNumber_2->display(sensor.get_temp2());
    ui->lcdNumber_3->display(sensor.get_temp3());
    ui->lcdNumber_4->display(sensor.get_temp4());

    time = 0;

    ui->progressBar->reset();

    drawLinesOnGraph();
}

void MainWindow::on_pushButton_2_clicked()
{
    QCoreApplication::quit();
}

void MainWindow::newSecond()
{
    if (time == 300)
    {
        sensor.generate_temp(sensor.get_temp1(), sensor.get_temp2(), sensor.get_temp3(), sensor.get_temp4(), 1);

        ui->lcdNumber_1->display(sensor.get_temp1());
        ui->lcdNumber_2->display(sensor.get_temp2());
        ui->lcdNumber_3->display(sensor.get_temp3());
        ui->lcdNumber_4->display(sensor.get_temp4());

        ui->progressBar->reset();

        time = 0;

        drawLinesOnGraph();
    }

    ui->progressBar->setValue(time);

    time++;
}

void MainWindow::drawLinesOnGraph()
{
    QVector<double> dates;

    QVector<double> temps1;
    QVector<double> temps2;
    QVector<double> temps3;
    QVector<double> temps4;

    qtime = QDateTime::currentDateTime();

    for(int i = 0; i < 10; i++)
        qtime = qtime.addSecs(-30);

    for(int j = 0; j < 10; j++)
    {
        dates.push_back(qtime.toMSecsSinceEpoch()/1000);
        qtime = qtime.addSecs(30);
    }

    for(int k = 0; k < 10; k++)
        temps1.push_back(sensor.get_temps1(k));

    for(int l = 0; l < 10; l++)
        temps2.push_back(sensor.get_temps2(l));

    for(int m = 0; m < 10; m++)
        temps3.push_back(sensor.get_temps3(m));

    for(int n = 0; n < 10; n++)
        temps4.push_back(sensor.get_temps4(n));

    ui->chart->graph(0)->setData(dates, temps1);
    ui->chart->graph(1)->setData(dates, temps2);
    ui->chart->graph(2)->setData(dates, temps3);
    ui->chart->graph(3)->setData(dates, temps4);

    ui->chart->rescaleAxes();
    ui->chart->yAxis->setRange(-5, 35);
    ui->chart->replot();

    if(sensor.get_min() <= -5)
        ui->chart->yAxis->setRange((int)sensor.get_min() - 5, 35);

    if(sensor.get_max() >= 35)
        ui->chart->yAxis->setRange(-5, (int)sensor.get_max() + 5);
}
