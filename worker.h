#ifndef WORKER_H
#define WORKER_H

#include <QThread>
#include <QString>
#include <QDir>
#include <string>



enum class Mode 
{
    DEFAULT     =   0,
    SCAN,
};




class Worker : public QThread
{
    Q_OBJECT

public:
    Worker()    =   default;
    ~Worker()   =   default;
    Worker( const Worker& ) =   delete;
    Worker( Worker&& )      =   delete;
    Worker& operator = ( const Worker& )    =   delete;
    Worker& operator = ( Worker&& )         =   delete;

    void    run() override;

    void    set_mode( Mode m );
    void    set_src( QString path );
    void    scan_folder( QString path );

    const QFileInfoList&    get_scan_list();

signals:
    void    scan_item_name_sig( QString );


private:

    QString         src;
    Mode            mode;

    QFileInfoList   scan_list;
};




#endif