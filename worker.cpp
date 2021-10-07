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
            //rename_file     =   std::bind( &Worker::rename_file_basic, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 );
            //rename_folder   =   std::bind( &Worker::rename_folder_basic, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 );

            //rename_file     =   std::bind( &Worker::rename_file_remove_full, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 );
            //rename_folder   =   std::bind( &Worker::rename_folder_remove_full, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 );
            
            rename_file     =   std::bind( &Worker::rename_file_remove_prefix, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 );           
            rename_folder   =   std::bind( &Worker::rename_folder_remove_prefix, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 );

            try {
                assert( conv == nullptr );
                emit progress_init_sig( 0, scan_list.size() );

                conv            =   new opencc::SimpleConverter( OPENCC_DEFAULT_CONFIG_SIMP_TO_TRAD );
                //codec         =   QTextCodec::codecForName("Big5");
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
        //qstr.remove( QChar(' ') );
    
        utf8_tc_str     =   conv->Convert( qstr.toStdString().c_str() );
        //big5_str        =   codec->fromUnicode( QString(utf8_tc_str.c_str()) );
    
        dst_name    =   utf8_tc_str.c_str();
        //qDebug() << "dst_name = " << dst_name;    

        if( info.isFile() == true )        
            rename_file( info, dst_dir, dst_name );        
        else if( info.isDir() == true )
            rename_folder( info, dst_dir, dst_name );
        else
            assert(false);
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
            qDebug() << "error";
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
        qDebug() << "error";
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
