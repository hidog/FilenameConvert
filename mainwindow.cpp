#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>


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
    connect(    ui->selectSrcButton,        &QPushButton::clicked,          this,               &MainWindow::selet_src_slot         );
    connect(    ui->selectDstButton,        &QPushButton::clicked,          this,               &MainWindow::selet_dst_slot         );
    connect(    ui->scanButton,             &QPushButton::clicked,          this,               &MainWindow::scan_slot              );
    connect(    &worker,                    &Worker::finished,              this,               &MainWindow::finish_worker_slot     );
    connect(    &worker,                    &Worker::scan_item_name_sig,    ui->messageEdit,    &QLineEdit::setText                 );
}







void    MainWindow::selet_src_slot()
{
    QString dir     =   QFileDialog::getExistingDirectory( this, tr("select src dir"), "D:\\", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks );
    setting.src     =   dir;
    ui->srcEdit->setText( setting.src );
}




void    MainWindow::selet_dst_slot()
{
    QString dir     =   QFileDialog::getExistingDirectory( this, tr("select dst dir"), "D:\\", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks );
    setting.dst     =   dir;
    ui->dstEdit->setText( setting.dst );
}




void    MainWindow::scan_slot()
{
    lock_button(true);
    bool    result  =   false;

    if( setting.dst == setting.src )
        QMessageBox::warning( nullptr, "Setting", "src == dst", QMessageBox::Ok, QMessageBox::Ok );
    else if( setting.dst.isEmpty() )
        QMessageBox::warning( nullptr, "Setting", "dst is empty", QMessageBox::Ok, QMessageBox::Ok );
    else if( setting.src.isEmpty() )
        QMessageBox::warning( nullptr, "Setting", "src is empty", QMessageBox::Ok, QMessageBox::Ok );
    else if( QDir(setting.src).exists() == false )
        QMessageBox::warning( nullptr, "Setting", "src is not exist", QMessageBox::Ok, QMessageBox::Ok );
    else if( QDir(setting.dst).exists() == false )
        QMessageBox::warning( nullptr, "Setting", "dst is not exist", QMessageBox::Ok, QMessageBox::Ok );
    else
    {
        result  =   true;
        worker.set_mode( Mode::SCAN );
        worker.set_src( setting.src );
        worker.start();
    }

    if( result == false )
        lock_button(false);
}




void    MainWindow::finish_worker_slot()
{
    lock_button(false);
}





void    MainWindow::lock_button( bool lock )
{
    ui->selectSrcButton->setDisabled(lock);
    ui->selectDstButton->setDisabled(lock);
}
