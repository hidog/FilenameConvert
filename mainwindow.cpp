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
    // 本來希望做檔案內容轉換big5, gb, 但功能上有點問題, 故先關閉
    ui->convertButton->setDisabled(true);

    connect(    ui->selectSrcButton,        &QPushButton::clicked,          this,               &MainWindow::selet_src_slot         );
    connect(    ui->selectDstButton,        &QPushButton::clicked,          this,               &MainWindow::selet_dst_slot         );
    connect(    ui->scanButton,             &QPushButton::clicked,          this,               &MainWindow::scan_slot              );
    connect(    ui->renameButton,           &QPushButton::clicked,          this,               &MainWindow::rename_slot            );
    connect(    ui->removeButton,           &QPushButton::clicked,          this,               &MainWindow::remove_slot            );
    connect(    ui->convertButton,          &QPushButton::clicked,          this,               &MainWindow::convert_slot           );

    connect(    &worker,                    &Worker::finished,              this,               &MainWindow::finish_worker_slot     );
    connect(    &worker,                    &Worker::message_sig,           ui->messageEdit,    &QLineEdit::setText                 );
    connect(    ui->fullNameCheckBox,       &QCheckBox::stateChanged,       this,               &MainWindow::full_path_slot         );
    connect(    &worker,                    &Worker::progress_init_sig,     ui->progressBar,    &QProgressBar::setRange             );
    connect(    &worker,                    &Worker::progress_sig,          ui->progressBar,    &QProgressBar::setValue             );
}






void    MainWindow::remove_slot()
{
    lock_button(true);

    bool    result  =   false;

    if( setting.src.isEmpty() )
        QMessageBox::warning( nullptr, "Setting", "src is empty", QMessageBox::Ok, QMessageBox::Ok );
    else if( QDir(setting.src).exists() == false )
        QMessageBox::warning( nullptr, "Setting", "src is not exist", QMessageBox::Ok, QMessageBox::Ok );
    else
    {
        result  =   true;
        worker.set_mode( Mode::REMOVE );
        worker.set_src( setting.src );
        worker.start();
    }

    if( result == false )
        lock_button(false);
}






void    MainWindow::convert_slot()
{
    lock_button(true);

    bool    result  =   false;

    if( setting.src.isEmpty() )
        QMessageBox::warning( nullptr, "Setting", "src is empty", QMessageBox::Ok, QMessageBox::Ok );
    else if( QDir(setting.src).exists() == false )
        QMessageBox::warning( nullptr, "Setting", "src is not exist", QMessageBox::Ok, QMessageBox::Ok );
    else
    {
        result  =   true;
        worker.set_mode( Mode::CONVERT );
        worker.set_src( setting.src );
        worker.start();
    }

    if( result == false )
        lock_button(false);
}







void    MainWindow::full_path_slot( int state )
{
    ui->messageEdit->setText( QString("change full path state. %1").arg(state) );

    if( state == Qt::Checked )
        ui->itemBrowser->setText( full_file_str );
    else
        ui->itemBrowser->setText( file_str );
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




void    MainWindow::rename_slot()
{
    const QFileInfoList&    list    =   worker.get_scan_list();
    if( list.size() == 0 )
    {
        QMessageBox::warning( nullptr, "rename", "need scan or folder is empty.", QMessageBox::Ok, QMessageBox::Ok );
        return;
    }

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
        worker.set_mode( Mode::RENAME );
        worker.set_src( setting.src );
        worker.set_dst( setting.dst );
        worker.start();
    }

    if( result == false )
        lock_button(false);

}






void    MainWindow::finish_worker_slot()
{
    lock_button(false);

    if( worker.get_mode() == Mode::SCAN )
    {
        ui->messageEdit->setText("finish scan.");

        file_str.clear();
        full_file_str.clear();

        const QFileInfoList& list   =   worker.get_scan_list();

        file_str        +=  QString("file count = %1\n").arg(list.size());
        full_file_str   +=  QString("file count = %1\n").arg(list.size());
        file_str        +=  analysis_ext(list);
        full_file_str   +=  analysis_ext(list);

        for( auto& item : list )
        {
            file_str        +=  item.fileName();
            full_file_str   +=  item.absoluteFilePath();
            file_str        +=  "\n";
            full_file_str   +=  "\n";
        }

        if( ui->fullNameCheckBox->checkState() == Qt::Checked )
            ui->itemBrowser->setText( full_file_str );
        else
            ui->itemBrowser->setText( file_str );
    }
    else if( worker.get_mode() == Mode::RENAME )
    {}
    else if( worker.get_mode() == Mode::REMOVE )
    {}
    else
        assert(false);
}





QString     MainWindow::analysis_ext( const QFileInfoList& list )
{
    QList<QString>  ext_list;
    QString         ext;
    QString         result;

    for( auto& item : list )
    {
        if( item.isFile() == true )
        {
            ext     =   item.suffix();
            if( ext_list.contains(ext) == false )
                ext_list.push_back(ext);
        }
    }

    result  =   "extension list : ";
    for( auto& itr : ext_list )
    {
        result  +=  itr;
        result  +=  ", ";
    }
    int     size    =   result.size();
    result.remove( size-2, 2 );
    result  +=  "\n";

    return  result;
}





void    MainWindow::lock_button( bool lock )
{
    ui->selectSrcButton->setDisabled(lock);
    ui->selectDstButton->setDisabled(lock);
    ui->scanButton->setDisabled(lock);
    ui->renameButton->setDisabled(lock);
}
