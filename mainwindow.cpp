#include "mainwindow.h"
#include <iostream>
#include <cassert>
#include <QDebug>

MainWindow::MainWindow(QObject *parent)
    : QObject(parent)
{
    server = new QTcpServer(this);
    server->listen(QHostAddress::Any, 9999);
    connect(server, &QTcpServer::newConnection, this, &MainWindow::acceptConnection);
    game = new Game;
}

MainWindow::~MainWindow()
{
    delete game;
}

void MainWindow::acceptConnection(){
    if(connectionNum >= 2){
        QTcpSocket* tmp = server->nextPendingConnection();
        tmp->close();
        delete tmp;
        return;
    } else {
        socket[connectionNum] = server->nextPendingConnection();
        if(connectionNum){
            std::cout << "Player2 has connected to the server." << std::endl;
            connect(socket[connectionNum], &QTcpSocket::readyRead, this, &MainWindow::replyToClient2);
            connect(socket[connectionNum], &QTcpSocket::disconnected, this, &MainWindow::disconnection2);
            server->pauseAccepting();
        } else {
            std::cout << "Player1 has connected to the server." << std::endl;
            connect(socket[connectionNum], &QTcpSocket::readyRead, this, &MainWindow::replyToClient1);
            connect(socket[connectionNum], &QTcpSocket::disconnected, this, &MainWindow::disconnection1);
        }
        connectionNum++;
    }
}

void MainWindow::disconnection1(){
    disconnection(0);
}

void MainWindow::disconnection2(){
    disconnection(1);
}

void MainWindow::disconnection(int color){
    std::cout << "Player" << (color ? 2 : 1) << " has safely disconnected from the server" << std::endl;
    connectionNum--;
    socket[color]->deleteLater();
    if(!connectionNum){
        delete game;
        game = new Game;
        server->resumeAccepting();
    }
}

void MainWindow::replyToClient1(){
    replyToClient(0);
}

void MainWindow::replyToClient2(){
    replyToClient(1);
}

void MainWindow::replyToClient(int color){
    QByteArray array = socket[color]->readAll();
    QDataStream serverstream(&array, QIODevice::ReadWrite);
    qint8 cmd;
    while(true){
        serverstream >> cmd;
        switch (cmd) {
        case 0:{
            return;
        }
        case 1:{
            std::vector<qint8> info_get;
            info_get.push_back(1);
            QVector<qint8> info_get_real;
            serverstream >> info_get_real;
            for(int i = 0; i < 3; i ++){
                info_get.push_back(info_get_real[i]);
            }
            if(info_get[1] != game->currentPlayer()){
                break;
            }
            std::vector<qint8> info = game->manipulate(info_get);
            QVector<qint8> realinfo;
            realinfo.push_back(info[1]);
            if(info[1]){
                realinfo.push_back(info[2]);
                realinfo.push_back(info[3]);
                realinfo.push_back(info[4]);
            } else {
                if(!info[2]){
                    game->finished() = info[3];
                }
            }
            send(color, 1, realinfo);
            break;
        }
        case 2:{
            std::vector<qint8> info_get;
            QVector<qint8> info_get_real;
            serverstream >> info_get_real;
            info_get.push_back(2);
            info_get.push_back(info_get_real[0]);
            std::vector<qint8> info = game->manipulate(info_get);
            QVector<qint8> realinfo;
            realinfo.push_back(info[1]);
            send(color, 2, realinfo);
            break;
        }
        case 3:{
            std::vector<qint8> info_get;
            QVector<qint8> info_get_real;
            serverstream >> info_get_real;
            info_get.push_back(3);
            info_get.push_back(info_get_real[0]);
            std::vector<qint8> info = game->manipulate(info_get);
            if(!info[1]){
                game->finished() = 2;
            }
            QVector<qint8> realinfo;
            realinfo.push_back(info[1]);
            send(color, 3, realinfo);
            break;
        }
        case 4:{
            std::vector<qint8> info_get;
            QVector<qint8> info_get_real;
            serverstream >> info_get_real;
            info_get.push_back(4);
            info_get.push_back(info_get_real[0]);
            std::vector<qint8> info = game->manipulate(info_get);
            if(!info[1]){
                game->finished() = !info_get[1];
            }
            QVector<qint8> realinfo;
            realinfo.push_back(info[1]);
            send(color, 4, realinfo);
            break;
        }
        case 5:{
            QVector<qint8> info, info_get;
            serverstream >> info_get;
            info.push_back(color);
            send(color, 5, info);
            break;
        }
        case 6:{
            QVector<qint8> info_get;
            serverstream >> info_get;
            std::stringstream ss;
            ss << *game;
            std::string infoStdStr;
            ss >> infoStdStr;
            QString info = QString::fromStdString(infoStdStr);
            send(color, 6, info);
            break;
        }
        case 7:{
            QVector<qint8> info;
            QVector<qint8> info_get;
            serverstream >> info_get;
            info.push_back(game->currentPlayer());
            send(color, 7, info);
            break;
        }
        case 8:{
            QVector<qint8> info;
            QVector<qint8> info_get;
            serverstream >> info_get;
            info.push_back(game->finished());
            send(color, 8, info);
            break;
        }
        case 9:{
            QVector<qint8> info;
            QVector<qint8> info_get;
            serverstream >> info_get;
            qint8 color_, row_, col_;
            std::tie(color_, row_, col_) = game->lastChessMan();
            info << color_ << row_ << col_;
            send(color, 9, info);
            break;
        }
        default:{
            qDebug() << "Unknown command.";
        }
        }
    }
}

void MainWindow::send(int color, qint8 cmd, const QVector<qint8>& info){
    QByteArray block;
    QDataStream serverstream(&block, QIODevice::ReadWrite);
    serverstream << cmd << info;
    socket[color]->write(block);
}

void MainWindow::send(int color, qint8 cmd, const QString& info){
    QByteArray block;
    QDataStream serverstream(&block, QIODevice::ReadWrite);
    serverstream << cmd << info;
    socket[color]->write(block);
}
