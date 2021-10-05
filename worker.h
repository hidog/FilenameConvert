#ifndef WORKER_H
#define WORKER_H

#include <QThread>
#include <QString>
#include <QDir>
#include <string>




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

    void    set_root( QString path );
    void    scan_folder( QString path );

    void    detect_code( std::wstring str );

private:

    QString root;


};




#endif