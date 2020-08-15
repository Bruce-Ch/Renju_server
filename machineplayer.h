#ifndef MACHINEPLAYER_H
#define MACHINEPLAYER_H

#include <QObject>
#include <QVector>

class MachinePlayer : public QObject
{
    Q_OBJECT
public:
    MachinePlayer() {}
    virtual ~MachinePlayer() {}

public slots:
    void goNext(const QString& info, int color);

signals:
    void resultReady(QVector<int> result);

private:
    int chessboard[15][15] = {{0}};
    int u = 0, v = 0;

    int judge(int chessboard1[15][15],int x,int y);
    int score_table(int number, int empty);
    int count_score(int n[], int chessColor);
    int score(int a,int chessboard1[15][15]);
    int minMax(int chessboard1[15][15],int deep,int a,int Alpha,int Beta);

};

#endif // MACHINEPLAYER_H
