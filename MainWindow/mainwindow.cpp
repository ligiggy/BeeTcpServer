#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pb_startListen_clicked()
{
    tcpServer = new BeeTcpServer();
    tcpServer->setTcpServer(ui->le_ip->text(), ui->le_port->text().toInt());
    connect(tcpServer, &BeeTcpServer::receiveMsg, this, &MainWindow::receiveMsg, Qt::QueuedConnection);

    tcpServer->startListen();
}

void MainWindow::on_pb_send_clicked()
{
    QByteArray array = ui->le_sendMsg->text().toLocal8Bit();
    tcpServer->sendMsg(array);

    ui->te_readMsg->append("Send Msg: " + QString::fromLocal8Bit(array));
}

void MainWindow::on_pb_stopListen_clicked()
{
    tcpServer->stopListen();

    delete tcpServer;
    tcpServer = nullptr;
}

void MainWindow::receiveMsg(QByteArray &msg)
{
    ui->te_readMsg->append("Receive Msg: " + QString::fromLocal8Bit(msg));
}
