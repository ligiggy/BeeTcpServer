#include "beetcpsocket.h"

#include <QThread>
#include <QHostAddress>
#include <QDebug>

class BeeTcpSocketPrivate
{
public:
    QThread *m_proxyThread;
};


BeeTcpSocket::BeeTcpSocket(QObject *parent)
    : QTcpSocket(parent)
    , d_ptr(new BeeTcpSocketPrivate)
{
    d_ptr->m_proxyThread = new QThread();

    connect(this, &BeeTcpSocket::readyRead, this, &BeeTcpSocket::readMsg, Qt::DirectConnection);
}

BeeTcpSocket::~BeeTcpSocket()
{
    qDebug() << "delete";
}

bool BeeTcpSocket::setSocketDescriptor(qintptr socketDescriptor,
                                       QAbstractSocket::SocketState state,
                                       QIODevice::OpenMode openMode)
{
    return QTcpSocket::setSocketDescriptor(socketDescriptor, state, openMode);
}

void BeeTcpSocket::start()
{
    this->moveToThread(d_ptr->m_proxyThread);
    d_ptr->m_proxyThread->start();
}

void BeeTcpSocket::stop()
{
    this->close();

    d_ptr->m_proxyThread->quit();

    qDebug() << "thread finished";
}

void BeeTcpSocket::readMsg()
{
    QByteArray msg = this->readAll();

    emit receiveMsg(msg);

    qDebug() << this->peerAddress() << this->peerPort() << msg;
}

void BeeTcpSocket::writeMsg(QByteArray &msg)
{
    this->write(msg);
}


