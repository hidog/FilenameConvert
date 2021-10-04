#include "worker.h"

#include <QDebug>
#include <QDir>
#include <QTextCodec>

#include <stdio.h>
    


void    Worker::run()
{
    qDebug() << "test";

    scan_folder( root );
}





void    Worker::scan_folder( QString path )
{
    qDebug() << path;

    QDir    dir(path);
    dir.setFilter( QDir::Dirs | QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot );
    QFileInfoList   list    =   dir.entryInfoList();

    //QTextCodec *codec = QTextCodec::codecForName("Big5");

    for( int i = 0; i < list.size(); ++i )
    {
        QFileInfo info  =   list.at(i);
        //qDebug() << info.fileName();

        if( info.isFile() == true )
        {
            QString filename = info.fileName();
            //codec->fromUnicode( filename );
            detect_code( filename.toStdString() );
        }

        scan_folder( info.absoluteFilePath() );
    }
}




void    Worker::detect_code( std::string str )
{
    printf("str = %s\n", str.c_str() );
}





void    Worker::set_root( QString path )
{
    root = path;
}
