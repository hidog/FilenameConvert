#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QFileDialog>



MainWindow::MainWindow(QWidget *parent)
:   QMainWindow(parent), 
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    init();
}





MainWindow::~MainWindow()
{
    delete ui;
}





void    MainWindow::init()
{
    connect(    ui->selectSrcButton,       &QPushButton::clicked,      this,        &MainWindow::selet_src_slot      );
    connect(    ui->selectDstButton,       &QPushButton::clicked,      this,        &MainWindow::selet_dst_slot      );
    connect(    &worker,                   &Worker::finished,          this,        &MainWindow::finish_worker       );
}




void    MainWindow::selet_src_slot()
{
    QString dir     =   QFileDialog::getExistingDirectory( this, tr("select src dir"), "D:\\", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks );
    setting.src     =   dir;
    ui->srcEdit->setText( setting.src );

    /*ui->selectSrcButton->setDisabled(true);
    worker.set_root( dir );
    worker.start();*/
}




void    MainWindow::selet_dst_slot()
{
    QString dir     =   QFileDialog::getExistingDirectory( this, tr("select dst dir"), "D:\\", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks );
    setting.dst     =   dir;
    ui->dstEdit->setText( setting.dst );
}





void    MainWindow::finish_worker()
{
    //ui->selectSrcButton->setDisabled(false);
}

