#include "worker.h"

#include <QDebug>
#include <QDir>
#include <cassert>

#include <opencc/opencc.h>
#include <QTextCodec>




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
        {
            try {
                assert( conv == nullptr );

                conv    =   new opencc::SimpleConverter( OPENCC_DEFAULT_CONFIG_SIMP_TO_TRAD );
                codec   =   QTextCodec::codecForName("Big5");

                rename( src, dst );

                delete conv;
                conv    =   nullptr;

            } catch( std::exception err ) {
                qDebug() << err.what();
                delete conv;
                conv    =   nullptr;
                assert(false);
            }
        }
        
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
    try {
        QDir    dir(src);

        dir.setFilter( QDir::Dirs | QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot );
        QFileInfoList   list    =   dir.entryInfoList();

        std::string     utf8_tc_str;  // tc = tradition chinese
        std::string     big5_str;

        for( auto& info : list )
        {
            //auto fn = info.fileName();
            //auto res = codec->fromUnicode( fn );
            //fprintf( p, "%s\n", res.data() );

            auto qstr = info.fileName();
            qstr.remove( QChar(' ') );

            utf8_tc_str     =   conv->Convert( qstr.toStdString().c_str() );
            big5_str        =   codec->fromUnicode( QString(utf8_tc_str.c_str()) );

            qDebug() << big5_str.c_str();

            /*if( info.isFile() == true )
            {
            QString filename = info.fileName();
            //codec->fromUnicode( filename );
            }*/

            //emit scan_item_name_sig( QString("scan item %1").arg(info.fileName()) );
            //qDebug() << info.fileName();

            //scan_list.push_back(info);
            rename( info.absoluteFilePath(), dst );
        }


    } catch( std::exception err ) {
        throw err;
    }
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
