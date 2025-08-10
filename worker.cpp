#include "worker.h"

#include <QDebug>
#include <QDir>
#include <cassert>

#include <QTextCodec>




void    Worker::run()
{
    switch( mode )
    {
    case Mode::SCAN:
        handle_scan();
        break;
    case Mode::RENAME :
        handle_rename();        
        break;
    case Mode::REMOVE :
        handle_remove();
        break;
    case Mode::CONVERT :
        handle_convert();
        break;
    default:
        assert(false);
    }
}





void    Worker::handle_convert()
{
    if( src.isEmpty() == true )
    {
        qDebug() << "src is empty.";
        assert(false);
    }
    else
    {}
}






void    Worker::handle_remove()
{
    if( src.isEmpty() == true )
    {
        qDebug() << "src is empty.";
        assert(false);
    }
    else
    {
        remove( src );    
        emit message_sig( QString("finish remove.") );
    }
}




void    Worker::convert( QString path )
{
    QDir    dir(path);
    dir.setFilter( QDir::Dirs | QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot );

    printf("path = %s", path.toStdString().c_str() );

#if 0
    QFileInfoList   list   =   dir.entryInfoList();
    QFileInfo       info;
    bool            res;
    std::string     big5_str,   str;


    int     i;
    for( i = 0; i < list.size(); i++ )
    {
        info    =   list.at(i);
        if( info.suffix() == QString("cue") )
        {
            QFile       file( info.absoluteFilePath() );
            QString     output;

            if( file.open( QIODevice::ReadWrite | QIODevice::Text ) == false )
            {
                qDebug() << "error !!";
                assert(false);
            }
            else
            {
                emit message_sig( QString("convert file. %1").arg(info.absoluteFilePath()) );

                while( file.atEnd() == false )
                {
                    QString line    =   file.readLine();
                    big5_str        =   conv->Convert( line.toStdString() );
                    str             =   codec->fromUnicode( QString("%1").arg(big5_str.c_str()) );
                    output          +=  QString( "%1" ).arg(big5_str.c_str());
                }
                
                file.remove();
                file.close();
                QFile   file2( info.absoluteFilePath() );
                file2.open( QIODevice::ReadWrite | QIODevice::Text );
                QTextStream out(&file2);
                out << output;
                file2.close();
            }
        }
        else if( info.isDir() == true )
        {
            convert( info.absoluteFilePath() );
        }
    }
#endif
}






void    Worker::remove( QString path )
{
    return;
}






void    Worker::handle_scan()
{
    if( src.isEmpty() == true )
    {
        qDebug() << "src is empty.";
        assert(false);
    }
    else
    {
#ifdef WIN32
        fp = fopen("D:\\test.sh", "w+");
#else
#error not define
#endif

        scan_list.clear();
        scan_folder( src );

        fclose(fp);
    }
}





void    Worker::handle_rename()
{
    if( src.isEmpty() == true || dst.isEmpty() == true )
    {
        qDebug() << "src or dst is empty.";
        assert(false);
    }
    else        
    {
        rename( src, dst );
    }
}





Mode    Worker::get_mode()
{
    return mode;
}




QString     Worker::remove_full_font( QString input )
{
    input.remove( 0, 7 );
    return  input;
}




void    Worker::rename( QString src, QString dst )
{
    QDir    src_dir(src);     
    
    src_dir.setFilter( QDir::Dirs | QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot );
    QFileInfoList   list    =   src_dir.entryInfoList();
    
    int         i;
    QFileInfo   info;
    for( i = 0; i < list.size(); i++ )
    {
        info = list.at(i);
        qDebug() << info.absolutePath();
    }

}



void    Worker::rename_file_remove_full( QFileInfo info, QDir dst_dir, QString dst_name )
{
    // remove 全形英文
    int         i;
    QString     qstr    =   dst_name;
    for( i = 0; i < dst_name.size(); i++ )
    {
        if( qstr[i].unicode() == 12288 ) // full space
            qstr[i] =   QChar(' ');
        else if( qstr[i].unicode() >= 65296 && qstr[i].unicode() <= 65305 )
            qstr[i] =   QChar( '0' + qstr[i].unicode() - 65296 );
        else if( qstr[i].unicode() >= 65313 && qstr[i].unicode() <= 65338 )
            qstr[i] =   QChar( 'A' + qstr[i].unicode() - 65313 );
        else if( qstr[i].unicode() >= 65345 && qstr[i].unicode() <= 65370 )
            qstr[i] =   QChar( 'a' + qstr[i].unicode() - 65345 );
    }
    dst_name    =   qstr;

    //
    QString dst_path    =   dst_dir.filePath(dst_name);
    bool    flag;

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





void    Worker::rename_folder_remove_full( QFileInfo info, QDir dst_dir, QString dst_name )
{
    // remove 全形英文
    int         i;
    QString     qstr    =   dst_name;
    for( i = 0; i < dst_name.size(); i++ )
    {
        if( qstr[i].unicode() == 12288 ) // full space
            qstr[i] =   QChar(' ');
        else if( qstr[i].unicode() >= 65296 && qstr[i].unicode() <= 65305 )
            qstr[i] =   QChar( '0' + qstr[i].unicode() - 65296 );
        else if( qstr[i].unicode() >= 65313 && qstr[i].unicode() <= 65338 )
            qstr[i] =   QChar( 'A' + qstr[i].unicode() - 65313 );
        else if( qstr[i].unicode() >= 65345 && qstr[i].unicode() <= 65370 )
            qstr[i] =   QChar( 'a' + qstr[i].unicode() - 65345 );
    }
    dst_name    =   qstr;

    //
    bool    flag   =   dst_dir.mkdir( dst_name );
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




void    Worker::rename_file_remove_prefix( QFileInfo info, QDir dst_dir, QString dst_name )
{
    // remove 全形英文
    int         i,  n;
    QString     qstr    =   dst_name;
    for( i = 0; i < dst_name.size(); i++ )
    {
        if( qstr[i].unicode() == 12288 ) // full space
            qstr[i] =   QChar(' ');
        else if( qstr[i].unicode() >= 65296 && qstr[i].unicode() <= 65305 )
            qstr[i] =   QChar( '0' + qstr[i].unicode() - 65296 );
        else if( qstr[i].unicode() >= 65313 && qstr[i].unicode() <= 65338 )
            qstr[i] =   QChar( 'A' + qstr[i].unicode() - 65313 );
        else if( qstr[i].unicode() >= 65345 && qstr[i].unicode() <= 65370 )
            qstr[i] =   QChar( 'a' + qstr[i].unicode() - 65345 );
    }
    dst_name    =   qstr;

    // remove prefix
    // 有些檔名剛好同資料夾名稱, 所以只能刪除一次, 不能直接用 remove(QString)
    n   =   dst_name.indexOf( dst_dir.dirName() );
    if( n != -1 )
        dst_name.remove( n, dst_dir.dirName().size() );
    for( i = 0; i < dst_name.size(); i++ )
    {
        if( dst_name[i] != QChar(' ') && dst_name[i] != QChar('-') )
            break;
    }
    if( i > 0 )
        dst_name.remove( 0, i );

    // find - and mkdir 
    n   =   dst_name.indexOf( QString(" -") );
    QString     subfix;
    if( n != -1 )
    {
        subfix  =   dst_name.left(n);

        for( i = subfix.size()-1; i >= 0; i-- )
        {
            if( subfix[i] != QChar(' ') && subfix[i] != QChar('-') )
                break;
        }
        if( i != subfix.size()-1 )
            subfix.remove( i + 1, subfix.size() - i + 1);
    }

    // remove subfix again
    if( n != -1 )
    {
        n   =   dst_name.indexOf( subfix );
        if( n != -1 )
            dst_name.remove( n, subfix.size() );

        for( i = 0; i < dst_name.size(); i++ )
        {
            if( dst_name[i] != QChar(' ') && dst_name[i] != QChar('-') )
                break;
        }
        if( i > 0 )
            dst_name.remove( 0, i );
    }

    // create subdir
    if( subfix.size() > 0 && n != -1 )
    {
        if( dst_dir.exists(subfix) == false )
            dst_dir.mkdir( subfix );
        dst_dir.cd(subfix);
    }

    //
    if( dst_name[0] == QChar('.') )
        qDebug() << "error " << info.absoluteFilePath() << "   " << dst_name ;
    QString dst_path    =   dst_dir.filePath(dst_name);
    bool    flag;

    if( QFile::exists(dst_path) == true )            
        qDebug() << "file exist. " << dst_path;            
    else
    {
        QFile   src_file( info.absoluteFilePath() );
        flag        =   src_file.copy( dst_path );
        if( flag == false )
        {
            qDebug() << "error   " << info.absoluteFilePath();
            assert(false);
        }
    }

    solved_count++;
    emit progress_sig(solved_count);
    emit message_sig( QString("copy file %1").arg(dst_name) );
}




void    Worker::rename_folder_remove_prefix( QFileInfo info, QDir dst_dir, QString dst_name )
{
    // remove 全形英文
    int         i;
    QString     qstr    =   dst_name;
    for( i = 0; i < dst_name.size(); i++ )
    {
        if( qstr[i].unicode() == 12288 ) // full space
            qstr[i] =   QChar(' ');
        else if( qstr[i].unicode() >= 65296 && qstr[i].unicode() <= 65305 )
            qstr[i] =   QChar( '0' + qstr[i].unicode() - 65296 );
        else if( qstr[i].unicode() >= 65313 && qstr[i].unicode() <= 65338 )
            qstr[i] =   QChar( 'A' + qstr[i].unicode() - 65313 );
        else if( qstr[i].unicode() >= 65345 && qstr[i].unicode() <= 65370 )
            qstr[i] =   QChar( 'a' + qstr[i].unicode() - 65345 );
    }
    dst_name    =   qstr;

    bool    flag   =   dst_dir.mkdir( dst_name );
    if( flag == false )
    {
        qDebug() << "error   " << info.absolutePath();
        assert(false);
    }
    solved_count++;
    emit progress_sig(solved_count);
    emit message_sig( QString("make dir %1").arg(dst_name) );
    rename( info.absoluteFilePath(), dst_dir.filePath(dst_name) );
}





void    Worker::rename_file_basic( QFileInfo info, QDir dst_dir, QString dst_name )
{
    QString dst_path    =   dst_dir.filePath(dst_name);
    bool    flag;

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





void    Worker::rename_folder_basic( QFileInfo info, QDir dst_dir, QString dst_name )
{
    bool    flag   =   dst_dir.mkdir( dst_name );
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





void    Worker::set_mode( Mode m )
{
    mode    =   m;
}





void    Worker::scan_folder( QString path )
{
    qDebug() << path;
    QDir        dir(path);
    QString     new_filename;

    dir.setFilter( QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot );
    QFileInfoList   list    =   dir.entryInfoList();

    for( auto& info : list )
    {
        if( info.isFile() == true )
        {
            if( info.suffix().compare("mp3", Qt::CaseInsensitive) == 0 )
            {
                // do nothing
                qDebug() << info.absoluteFilePath();
            }
            else if( info.suffix().compare("flac", Qt::CaseInsensitive) == 0 )
            {
                qDebug() << info.fileName();

                new_filename    =   info.absoluteFilePath();
                new_filename.remove( info.absoluteFilePath().size() - 4, 4 );
                new_filename += "mp3";
                qDebug() << new_filename;

                QFile   file(new_filename);
                if( file.exists() == true )
                    qDebug() << "repeat";

                fprintf(fp, "ffmpeg -i \"%s\" -ab 320k -ar 44100 -map_metadata 0 -d3v2_version 3 -write_id3v1 1 \"%s\"\n", 
                    info.absoluteFilePath().toStdString().c_str(), new_filename.toStdString().c_str() );
                fprintf(fp, "rm \"%s\"\n", info.absoluteFilePath().toStdString().c_str() );
            }
            else
            {
                qDebug() << info.absoluteFilePath();
                fprintf(fp, "rm \"%s\"\n", info.absoluteFilePath().toStdString().c_str() );
            }
        }
        else if( info.isDir() == true )
        {
            scan_folder( info.absoluteFilePath() );
        }
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
