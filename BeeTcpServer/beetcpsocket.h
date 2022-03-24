#ifndef BEETCPSOCKET_H
#define BEETCPSOCKET_H

#include <QTcpSocket>

class BeeTcpSocketPrivate;
class BeeTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    BeeTcpSocket(QObject *parent = nullptr);
    ~BeeTcpSocket();

    bool setSocketDescriptor(qintptr socketDescriptor,
                             SocketState state = ConnectedState,
                             OpenMode openMode = ReadWrite);

public:
    void start();
    void stop();

    void writeMsg(QByteArray &msg);

private:
    void readMsg();

signals:
    void receiveMsg(QByteArray &msg);

private:
    QScopedPointer<BeeTcpSocketPrivate> d_ptr;
};

#endif // BEETCPSOCKET_H
