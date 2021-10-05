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
    void    finish_worker();

private:
    Ui::MainWindow  *ui;

    void    init();

    Worker      worker;
    Setting     setting;
};




#endif // MAINWINDOW_H
