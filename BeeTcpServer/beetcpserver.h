#ifndef BEETCPSERVER_H
#define BEETCPSERVER_H

#include <QTcpServer>
#include <QTcpSocket>

class BeeTcpServerPrivate;

class BeeTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    BeeTcpServer(QObject *parent = nullptr);
    ~BeeTcpServer();

public:
    void incomingConnection(qintptr handle);

public:
    void setTcpServer(QString ip, int port);

    bool startListen();
    bool stopListen();

    void sendToClient(int index, QByteArray &msg);
    void sendToAllClient(QByteArray &msg);

    void receiveFormClient(QByteArray &msg);

signals:
    void sendMsg(QByteArray &msg);

    void closeClient();

    void receiveMsg(QByteArray &msg);

private:
    void disconnectToClient();

private:
    QScopedPointer<BeeTcpServerPrivate> d_ptr;
};

#endif // BEETCPSERVER_H
