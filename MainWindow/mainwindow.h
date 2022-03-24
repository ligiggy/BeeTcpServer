#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "beetcpserver.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pb_startListen_clicked();

    void on_pb_send_clicked();

    void on_pb_stopListen_clicked();

private:
    void receiveMsg(QByteArray &msg);

private:
    Ui::MainWindow *ui;

    BeeTcpServer *tcpServer;
};

#endif // MAINWINDOW_H
