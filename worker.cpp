#include "worker.h"

#include <QDebug>
#include <QDir>
#include <QTextCodec>
#include <cassert>





void    Worker::run()
{
    switch( mode )
    {
    case Mode::SCAN:
        if( src.isEmpty() == true )
        {
            qDebug() << "src is empty.";
            assert(false);
        }
        else
        {
            scan_list.clear();
            scan_folder( src );        
        }
        break;
    case Mode::RENAME :
        if( src.isEmpty() == true || dst.isEmpty() == true )
        {
            qDebug() << "src or dst is empty.";
            assert(false);
        }
        else        
            rename( src, dst );
        
        break;
    default:
        assert(false);
    }
}




Mode    Worker::get_mode()
{
    return mode;
}




void    Worker::rename( QString src, QString dst )
{
#if 0
    QDir    dir(path);

    dir.setFilter( QDir::Dirs | QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot );
    QFileInfoList   list    =   dir.entryInfoList();

    //QTextCodec *codec = QTextCodec::codecForName("GB18030");

    //for( int i = 0; i < list.size(); ++i )
    for( auto& info : list )
    {
        //QFileInfo info  =   list.at(i);
        //qDebug() << info.fileName();

        //auto fn = info.fileName();
        //auto res = codec->fromUnicode( fn );
        //fprintf( p, "%s\n", res.data() );

        //auto qstr = info.fileName();
        //qstr.remove( QChar(' ') );


        /*if( info.isFile() == true )
        {
        QString filename = info.fileName();
        //codec->fromUnicode( filename );
        }*/

        emit scan_item_name_sig( QString("scan item %1").arg(info.fileName()) );
        //qDebug() << info.fileName();

        scan_list.push_back(info);
        scan_folder( info.absoluteFilePath() );
    }
#endif
}





void    Worker::set_mode( Mode m )
{
    mode    =   m;
}





void    Worker::scan_folder( QString path )
{
    QDir    dir(path);

    dir.setFilter( QDir::Dirs | QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot );
    QFileInfoList   list    =   dir.entryInfoList();

    for( auto& info : list )
    {
        emit scan_item_name_sig( QString("scan item %1").arg(info.fileName()) );

        scan_list.push_back(info);
        scan_folder( info.absoluteFilePath() );
    }
}




const QFileInfoList&    Worker::get_scan_list()
{
    return  scan_list;
}






void    Worker::set_src( QString path )
{
    src     =   path;
}





void    Worker::set_dst( QString path )
{
    dst     =   path;
}
