#include "worker.h"

#include <QDebug>
#include <QDir>
#include <QTextCodec>

#include <stdio.h>


FILE *p;



void    Worker::run()
{
    qDebug() << "test";

    p = fopen("test.txt", "w+");
    scan_folder( root );
    fclose(p);
}






void    Worker::scan_folder( QString path )
{
    qDebug() << path;

    //fprintf( p, "%s\n", path.toStdString().c_str() );

    QDir    dir(path);
    dir.setFilter( QDir::Dirs | QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot );
    QFileInfoList   list    =   dir.entryInfoList();

    QTextCodec *codec = QTextCodec::codecForName("GB18030");

    for( int i = 0; i < list.size(); ++i )
    {
        QFileInfo info  =   list.at(i);
        //qDebug() << info.fileName();

        //auto fn = info.fileName();
        //auto res = codec->fromUnicode( fn );
        //fprintf( p, "%s\n", res.data() );

        auto qstr = info.fileName();
        qstr.remove( QChar(' ') );
        fprintf( p, "%s\n", qstr.toStdString().c_str() );


        if( info.isFile() == true )
        {
            QString filename = info.fileName();
            //codec->fromUnicode( filename );
            detect_code( filename.toStdWString() );
        }

        scan_folder( info.absoluteFilePath() );
    }
}




void    Worker::detect_code( std::wstring str )
{
}





void    Worker::set_root( QString path )
{
    root = path;
}
