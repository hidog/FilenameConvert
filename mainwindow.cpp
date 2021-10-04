#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QFileDialog>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    init();
}





MainWindow::~MainWindow()
{
    delete ui;
}





void MainWindow::init()
{
    connect(    ui->selectFolderButton,    SIGNAL(clicked()),      this,       SLOT(selet_folder_slot())   );
}




void MainWindow::selet_folder_slot()
{
    QString dirname = QFileDialog::getExistingDirectory( this, tr("select dir"), "D:\\", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks );
    ui->selectFolderButton->setDisabled(true);
}

