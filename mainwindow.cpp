#include "mainwindow.h"
#include <iostream>

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
        return;
    } else {
        socket[connectionNum] = server->nextPendingConnection();
        if(connectionNum){
            std::cout << "Player2 has connected to the server." << std::endl;
            connect(socket[connectionNum], &QTcpSocket::readyRead, this, &MainWindow::replyToClient2);
        } else {
            std::cout << "Player1 has connected to the server." << std::endl;
            connect(socket[connectionNum], &QTcpSocket::readyRead, this, &MainWindow::replyToClient1);
        }
        connectionNum++;
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
            for(int i = 0; i < 3; i ++){
                serverstream >> cmd;
                info_get.push_back(cmd);
            }
            if(info_get[1] != game->currentPlayer()){
                break;
            }
            std::vector<qint8> info = game->manipulate(info_get);
            std::vector<qint8> realinfo;
            realinfo.push_back(info[0]);
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
            send(color, realinfo);
            break;
        }
        case 2:{
            std::vector<qint8> info_get;
            info_get.push_back(2);
            serverstream >> cmd;
            info_get.push_back(cmd);
            std::vector<qint8> info = game->manipulate(info_get);
            send(color, info);
            break;
        }
        case 3:{
            std::vector<qint8> info_get;
            info_get.push_back(3);
            serverstream >> cmd;
            info_get.push_back(cmd);
            std::vector<qint8> info = game->manipulate(info_get);
            if(!info[1]){
                game->finished() = 2;
            }
            send(color, info);
            break;
        }
        case 4:{
            std::vector<qint8> info_get;
            info_get.push_back(4);
            serverstream >> cmd;
            info_get.push_back(cmd);
            std::vector<qint8> info = game->manipulate(info_get);
            if(!info[1]){
                game->finished() = !info_get[1];
            }
            send(color, info);
            break;
        }
        case 5:{
            std::vector<qint8> info;
            info.push_back(5);
            info.push_back(color);
            send(color, info);
            break;
        }
        case 6:{
            std::stringstream ss;
            ss << *game;
            std::string infoStdStr;
            ss >> infoStdStr;
            QString info = QString::fromStdString(infoStdStr);
            //qDebug() << info;
            send(color, 6, info);
            break;
        }
        case 7:{
            std::vector<qint8> info;
            info.push_back(7);
            info.push_back(game->currentPlayer());
            send(color, info);
            break;
        }
        case 8:{
            std::vector<qint8> info;
            info.push_back(8);
            info.push_back(game->finished());
            send(color, info);
            break;
        }
        case 9:{
            std::vector<qint8> info;
            info.push_back(9);
            qint8 color_, row_, col_;
            std::tie(color_, row_, col_) = game->lastChessMan();
            info.push_back(color_);
            info.push_back(row_);
            info.push_back(col_);
            send(color, info);
            break;
        }
        default:{
            qDebug() << "Unknown command.";
        }
        }
    }
}

void MainWindow::send(int color, const std::vector<qint8>& info){
    QByteArray block;
    QDataStream serverstream(&block, QIODevice::ReadWrite);
    for(qint8 data: info){
        serverstream << data;
    }
    socket[color]->write(block);
}

void MainWindow::send(int color, qint8 cmd, const QString& info){
    QByteArray block;
    QDataStream serverstream(&block, QIODevice::ReadWrite);
    serverstream << cmd;
    serverstream << info;
    socket[color]->write(block);
}
