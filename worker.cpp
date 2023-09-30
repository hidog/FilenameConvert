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
    {
        codec   =   QTextCodec::codecForName("Big5");
        conv    =   new opencc::SimpleConverter( OPENCC_DEFAULT_CONFIG_SIMP_TO_TRAD );

        convert( src );    
        emit message_sig( QString("finish convert.") );

        delete conv;
        conv    =   nullptr;
    }
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
}






void    Worker::remove( QString path )
{
    // flac, log, m3u, jpg, cue, png, JPG
    // cue, wav, jpg, log, torrent, rar, LOG, bmp, png, JPG, CUE, WAV, flac, txt, tak, PNG

    QDir    dir(path);
    dir.setFilter( QDir::Dirs | QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot );

    QFileInfoList   list   =   dir.entryInfoList();
    QFileInfo       info;
    bool            res;

    int     i;
    for( i = 0; i < list.size(); i++ )
    {
        info    =   list.at(i);
        if( info.isFile() == true )
        {
            if( info.suffix() == QString("log") || 
                info.suffix() == QString("jpg") ||
                info.suffix() == QString("torrent") ||
                info.suffix() == QString("rar") ||
                info.suffix() == QString("bmp") ||
                info.suffix() == QString("png") ||
                info.suffix() == QString("txt") )
            {
                //qDebug() << info.absoluteFilePath();
                emit message_sig( QString("rm file. %1").arg(info.absoluteFilePath()) );

                QFile       file(info.absoluteFilePath());
                file.moveToTrash();
            }
        }
        else if( info.isDir() == true )        
            remove( info.absoluteFilePath() );
    }

    // remove folder if needed.
    list    =   dir.entryInfoList();
    if( list.size() == 0 )
    {
        QString name    =   dir.dirName();
        dir.cdUp();
        res =   dir.rmdir(name);
        if( res == false )
        {
            qDebug() << "error " << path;
            assert(false);
        }

        emit message_sig( QString("rm dir. %1").arg(path) );
    }
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
        scan_list.clear();
        scan_folder( src );        
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
        // 基本的轉換檔名
        rename_file     =   std::bind( &Worker::rename_file_basic, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 );
        rename_folder   =   std::bind( &Worker::rename_folder_basic, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 );

        // 移除全形英文
        //rename_file     =   std::bind( &Worker::rename_file_remove_full, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 );
        //rename_folder   =   std::bind( &Worker::rename_folder_remove_full, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 );

        // 某個檔案會用檔名的方式區別動畫名稱跟曲名, 這個會將動畫名稱新建資料夾
        //rename_file     =   std::bind( &Worker::rename_file_remove_prefix, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 );           
        //rename_folder   =   std::bind( &Worker::rename_folder_remove_prefix, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 );

        try {
            assert( conv == nullptr );
            emit progress_init_sig( 0, scan_list.size() );

            conv            =   new opencc::SimpleConverter( OPENCC_DEFAULT_CONFIG_SIMP_TO_TRAD );
            solved_count    =   0;

            rename( src, dst );

            delete conv;
            conv    =   nullptr;

            //qDebug() << "solved_count = " << solved_count;
            emit message_sig( QString("rename finish. solve %1 files.").arg(solved_count) );

        } catch( std::exception err ) {
            qDebug() << err.what();
            delete conv;
            conv    =   nullptr;
            assert(false);
        }
    }
}





Mode    Worker::get_mode()
{
    return mode;
}




#if 0 // load sub
void    Worker::rename( QString src, QString dst )
{
    QDir    src_dir(src);
    QDir    dst_dir(dst);       
    
    src_dir.setFilter( QDir::Dirs | QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot );
    QFileInfoList   list    =   src_dir.entryInfoList();
    
    std::string     utf8_tc_str, utf8_sub_str;
    
    FILE    *fp =   fopen( "G:\\convert.bat", "w+" );

    //
    //for( auto& info : list )
    for( int i = 0; i < list.size(); i += 2 )
    {
        auto info = list.at(i+1);  // sometimes need exchange with sub
        auto qstr = info.fileName();

        auto sub = list.at(i);
        auto sub_str = sub.fileName();
    
        utf8_tc_str     =   conv->Convert( qstr.toStdString().c_str() );
        utf8_sub_str    =   conv->Convert( sub_str.toStdString().c_str() );

        fprintf( fp, "ffmpeg -i \"%s\" -i \"%s\" -map 0:0 -map 0:1 -map 1:0 -vcodec hevc_nvenc -cq 30 -pix_fmt p010le -acodec copy -scodec copy -disposition:s:0 default \"./output/%s\"\n", 
            utf8_tc_str.c_str(), utf8_sub_str.c_str(), utf8_tc_str.c_str() );
    }

    fclose(fp);
}
#elif 1  // single file, simple convert.
void    Worker::rename( QString src, QString dst )
{
    QDir    src_dir(src);
    QDir    dst_dir(dst);       
    
    src_dir.setFilter( QDir::Dirs | QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot );
    QFileInfoList   list    =   src_dir.entryInfoList();
    
    std::string     utf8_tc_str;
    
    FILE    *fp =   fopen( "G:\\convert.bat", "w+" );

    //
    for( auto& info : list )
    {
        auto qstr = info.fileName();
    
        utf8_tc_str     =   conv->Convert( qstr.toStdString().c_str() );
        fprintf( fp, "ffmpeg -i \"%s\" -vcodec hevc_nvenc -cq 26 -acodec copy \"./output/%s\"\n", 
            utf8_tc_str.c_str(), utf8_tc_str.c_str() );
    }

    fclose(fp);
}
#endif


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
