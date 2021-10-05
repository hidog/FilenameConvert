#include "worker.h"

#include <QDebug>
#include <QDir>
#include <cassert>

#include <opencc/opencc.h>
//#include <QTextCodec>




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

                emit progress_init_sig( 0, scan_list.size() );

                conv            =   new opencc::SimpleConverter( OPENCC_DEFAULT_CONFIG_SIMP_TO_TRAD );
                //codec         =   QTextCodec::codecForName("Big5");
                solved_count    =   0;

                rename( src, dst );

                delete conv;
                conv    =   nullptr;

                qDebug() << "solved_count = " << solved_count;

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
    //qDebug() << "src = " << src << "\ndst = " << dst << "\n";
    
    QDir    src_dir(src);
    QDir    dst_dir(dst);       
    
    src_dir.setFilter( QDir::Dirs | QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot );
    QFileInfoList   list    =   src_dir.entryInfoList();
    
    std::string     utf8_tc_str;  // tc = tradition chinese
    //std::string     big5_str;
    QString         dst_path, dst_name;
    
    bool    flag;
    
    //
    //for( auto& info : list )
    for( int i = 0; i < list.size(); i++ )
    {
        auto info = list.at(i);
        auto qstr = info.fileName();
        qstr.remove( QChar(' ') );
    
        utf8_tc_str     =   conv->Convert( qstr.toStdString().c_str() );
        //big5_str        =   codec->fromUnicode( QString(utf8_tc_str.c_str()) );
    
        dst_name    =   utf8_tc_str.c_str();

        //qDebug() << "dst_name = " << dst_name;    

        if( info.isFile() == true )
        {
            dst_path    =   dst_dir.filePath(dst_name);
            
            if( QFile::exists(dst_path) == true )            
                qDebug() << "file exist. " << dst_path;            
            else
            {
                QFile   src_file( info.absoluteFilePath() );
                flag        =   src_file.copy( dst_path );
                if( flag == false )
                {
                    qDebug() << "error";
                    assert(false);
                }
            }

            solved_count++;
            emit progress_sig(solved_count);
            emit message_sig( QString("copy file %1").arg(dst_name) );
        }
        else if( info.isDir() == true )
        {
            flag   =   dst_dir.mkdir( dst_name );
            if( flag == false )
            {
                qDebug() << "error";
                assert(false);
            }
            solved_count++;
            emit progress_sig(solved_count);
            emit message_sig( QString("make dir %1").arg(dst_name) );
            rename( info.absoluteFilePath(), dst_dir.filePath(dst_name) );
        }
        else
            assert(false);
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
        emit message_sig( QString("scan item %1").arg(info.fileName()) );

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
