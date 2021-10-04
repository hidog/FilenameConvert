#ifndef WORKER_H
#define WORKER_H

#include <QThread>


class Worker : public QThread
{
    Q_OBJECT

public:
    Worker() = default;
    ~Worker() = default;
    Worker( const Worker& ) = delete;
    Worker( Worker&& ) = delete;
    Worker& operator = ( const Worker& ) = delete;
    Worker& operator = ( Worker&& ) = delete;

    void run() override;

private:


};




#endif