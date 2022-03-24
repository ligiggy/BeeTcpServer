#include "beetcpserver.h"
#include "beetcpsocket.h"

class BeeTcpServerPrivate
{
public:
    QString m_serverAddress = "127.0.0.1";
    int m_nPort = 9001;


public:
    QList<BeeTcpSocket *> tcpClients;
};


BeeTcpServer::BeeTcpServer(QObject *parent)
    : QTcpServer(parent)
    , d_ptr(new BeeTcpServerPrivate)
{
    qRegisterMetaType<QByteArray>("QByteArray &");
}

BeeTcpServer::~BeeTcpServer()
{

}

bool BeeTcpServer::startListen()
{
    if(this->isListening())
    {
        qDebug() << "is listening";
        return false;
    }

    return this->listen(QHostAddress(d_ptr->m_serverAddress), d_ptr->m_nPort);
}

bool BeeTcpServer::stopListen()
{
    emit closeClient();

    if(this->isListening())
    {
        qDebug() << "clsoe listening";
        this->close();
    }

    return true;
}

void BeeTcpServer::incomingConnection(qintptr handle)
{
    BeeTcpSocket *socket = new BeeTcpSocket();
    socket->setSocketDescriptor(handle);

    connect(this, &BeeTcpServer::sendMsg, socket, &BeeTcpSocket::writeMsg, Qt::QueuedConnection);
    connect(this, &BeeTcpServer::closeClient, socket, &BeeTcpSocket::stop, Qt::BlockingQueuedConnection);
    connect(socket, &BeeTcpSocket::disconnected, this, &BeeTcpServer::disconnectToClient, Qt::QueuedConnection);
    connect(socket, &BeeTcpSocket::receiveMsg, this, &BeeTcpServer::receiveFormClient, Qt::QueuedConnection);


    d_ptr->tcpClients.append(socket);

    socket->start();

    // if you add this func, when you close tcpserver, qt will delete the socket by himself;
    // but this socket has been moved to another thread, it can not be deleted by QTcpServer

    //        addPendingConnection(socket);

    qDebug() << socket->peerAddress() << socket->peerPort() << "connected success";

}

void BeeTcpServer::setTcpServer(QString ip, int port)
{
    d_ptr->m_serverAddress = ip;
    d_ptr->m_nPort = port;
}

void BeeTcpServer::sendToClient(int index, QByteArray &msg)
{

}

void BeeTcpServer::sendToAllClient(QByteArray &msg)
{
    for(int i = 0; i < d_ptr->tcpClients.size(); ++i)
    {
        emit sendMsg(msg);
    }
}

void BeeTcpServer::receiveFormClient(QByteArray &msg)
{
    BeeTcpSocket *socket = static_cast<BeeTcpSocket *>(sender());

    QByteArray byteMsg = (socket->peerAddress().toString() +
                          QString::number(socket->peerPort()) + ":" +
                          QString::fromLocal8Bit(msg)).toLocal8Bit();

    emit receiveMsg(byteMsg);
}

void BeeTcpServer::disconnectToClient()
{
    BeeTcpSocket *socket = static_cast<BeeTcpSocket *>(sender());

    qDebug() << socket->peerAddress() << socket->peerPort() << "had disconnected";

    d_ptr->tcpClients.removeOne(socket);

    socket->close();

    delete socket;

    qDebug() << "tcpClients size" << d_ptr->tcpClients.size();
}

