//
// Created by pc on 2020/6/5.
//

#ifndef RENJU_GAME_H
#define RENJU_GAME_H

#include <QtGlobal>
#include <stack>
#include <tuple>
#include <string>
#include <ostream>
#include <cassert>
#include "ChessBoard.h"
#include "Manipulation.h"


class Game {
    ChessBoard* chessBoard_ = nullptr;
    std::stack<std::tuple<int, int, int>>* stack_ = nullptr; // 颜色，位置，位置
    int finished_ = -1;
public:
    Game();
    ~Game();
    QVector<qint8> manipulate(const QVector<qint8>& info);
    int currentPlayer() const;
    inline int& finished() {
        return finished_;
    }
    std::tuple<int, int, int> lastChessMan() const; // 若尚无棋子，则返回(-1, 0, 0)
    friend std::ostream& operator<<(std::ostream& out, const Game& game);
};


#endif //RENJU_GAME_H
