//
// Created by pc on 2020/6/4.
//

#ifndef RENJU_RULES_H
#define RENJU_RULES_H


#include <list>
#include "ChessBoard.h"

class Rules {
    ChessBoard* chessBoard_;
public:
    explicit Rules(ChessBoard* chessBoard): chessBoard_(chessBoard){}
    int cntThree(ChessMan* chessMan, int dir);
    int cntThree(Pos pos, int color, int dir); // 无论pos位置上当前是何种棋子，将其改为color，再做判断
    int cntFour(ChessMan* chessMan, int dir);
    int cntFour(Pos pos, int color, int dir);
    bool checkFive(ChessMan* chessMan, int dir);
    bool checkFive(Pos pos, int color, int dir);
    bool checkLong(ChessMan* chessMan, int dir);
    bool checkLong(Pos pos, int color, int dir);
    int finished(ChessMan* chessMan); // 0白1黑2未分
    int finished(Pos pos, int color);

private:
    static inline int getReverseDir(int dir) {
        return (dir + 4) % 8;
    }
    std::vector<int> getLine(ChessMan* chessMan, int dir, int window); // 同1异0空2 一个异则都是异
};


#endif //RENJU_RULES_H
