#ifndef TESTTHREAD_H
#define TESTTHREAD_H

#include <QThread>

class TestThread : public QThread
{
public:
    TestThread(QObject *parent = Q_NULLPTR);
    ~TestThread();

public:
    void setStop();


    void run();
};

#endif // TESTTHREAD_H
