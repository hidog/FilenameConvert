#ifndef WORKER_H
#define WORKER_H

#include <QThread>
#include <QString>
#include <QDir>
#include <string>
#include <functional>



namespace opencc {
    class SimpleConverter;
} // end namespace opencc
//class QTextCodec;






enum class Mode 
{
    DEFAULT     =   0,
    SCAN,
    RENAME,
    REMOVE,
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
    void    remove( QString path );

    void    handle_rename();
    void    handle_scan();
    void    handle_remove();

    const QFileInfoList&    get_scan_list();

signals:
    void    message_sig( QString );
    void    progress_init_sig( int, int );
    void    progress_sig( int );

private:
    std::function<void(QFileInfo,QDir,QString)>     rename_file, rename_folder;

    void    rename_file_basic( QFileInfo info, QDir dst_dir, QString dst_name );
    void    rename_folder_basic( QFileInfo info, QDir dst_dir, QString dst_name );

    void    rename_file_remove_full( QFileInfo info, QDir dst_dir, QString dst_name );
    void    rename_folder_remove_full( QFileInfo info, QDir dst_dir, QString dst_name );

    void    rename_file_remove_prefix( QFileInfo info, QDir dst_dir, QString dst_name );
    void    rename_folder_remove_prefix( QFileInfo info, QDir dst_dir, QString dst_name );

    int     solved_count    =   0;

    QString         src, dst;
    Mode            mode;

    QFileInfoList   scan_list;

    opencc::SimpleConverter   *conv     =   nullptr;
    //QTextCodec                *codec    =   nullptr;

};




#endif