#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>

#include <sstream>
#include "Game.h"

class MainWindow : public QObject
{
    Q_OBJECT

public:
    MainWindow(QObject *parent = nullptr);
    ~MainWindow();

private:
    QTcpServer* server;
    QTcpSocket* socket[2];
    Game* game;
    int connectionNum = 0;

private slots:
    void acceptConnection();
    void replyToClient1();
    void replyToClient2();

private:
    void send(int color, const std::vector<qint8>& info);
    void send(int color, qint8 cmd, const QString& info);
    void replyToClient(int color);
};
#endif // MAINWINDOW_H