#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "worker.h"
#include "setting.h"



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE





class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow( QWidget *parent = nullptr );
    ~MainWindow();

public slots:
    void    selet_src_slot();
    void    selet_dst_slot();
    void    scan_slot();
    void    finish_worker_slot();
    void    full_path_slot( int state );

private:
    Ui::MainWindow  *ui;

    void        init();
    void        lock_button( bool lock );
    QString     analysis_ext( const QFileInfoList& list );

    Worker      worker;
    Setting     setting;
    QString     file_str,   full_file_str;
};




#endif // MAINWINDOW_H
