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
    connect(    ui->selectFolderButton,    &QPushButton::clicked,      this,        &MainWindow::selet_folder_slot   );
    connect(    &worker,                   &Worker::finished,          this,        &MainWindow::finish_worker       );
}




void    MainWindow::selet_folder_slot()
{
    QString dir     =   QFileDialog::getExistingDirectory( this, tr("select dir"), "D:\\", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks );
    ui->selectFolderButton->setDisabled(true);

    worker.set_root( dir );
    worker.start();
}





void    MainWindow::finish_worker()
{
    ui->selectFolderButton->setDisabled(false);
}

