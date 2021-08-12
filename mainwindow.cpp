#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dragtableview.h"

#include <QLabel>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();

    this->setStyleSheet( "DragTableView #tmpWidget {border:1px solid red} "
                        "DragCustomWidget {border:1px solid green} ");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    DragTableView *widget = new DragTableView(this);
    ui->widget->layout()->addWidget(widget);


    for ( int i=0; i<10 ; i++)
    {
        widget->addExecuWidget(QString("number: %1").arg(i));
    }
}

