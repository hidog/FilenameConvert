#ifndef WORKER_H
#define WORKER_H

#include <QThread>
#include <QString>
#include <QDir>
#include <string>




namespace opencc {
    class SimpleConverter;
} // end namespace opencc
//class QTextCodec;






enum class Mode 
{
    DEFAULT     =   0,
    SCAN,
    RENAME,
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
    void    set_dst( QString path );
    Mode    get_mode();

    void    scan_folder( QString path );    
    void    rename( QString src, QString dst );

    const QFileInfoList&    get_scan_list();

signals:
    void    scan_item_name_sig( QString );


private:

    QString         src, dst;
    Mode            mode;

    QFileInfoList   scan_list;

    opencc::SimpleConverter   *conv     =   nullptr;
    //QTextCodec                *codec    =   nullptr;

};




#endif