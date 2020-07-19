#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>
#include <QQueue>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

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
    bool verified[2] = {false, false};
    QQueue<QVector<qint8>> subcmds[2];
    QQueue<qint8> cmds[2];
    Game* game;
    int connectionNum = 0;

private slots:
    void acceptConnection();
    void disconnection1();
    void disconnection2();
    void replyToClient1();
    void replyToClient2();

private:
    void send(int color, qint8 cmd, const QVector<qint8>& info);
    void send(int color, qint8 cmd, const QString& info);
    void replyToClient(int color);
    void replyImplement(int color, QDataStream& stream);
    void disconnection(int color);
    bool versionVerify(const QVector<qint8>& version);
    int versionVerify(const QString& version);
    QVector<qint8> formManipulate(const QMap<QString, QString>& form);
};
#endif // MAINWINDOW_H
