//
// Created by pc on 2020/6/5.
//

#include "Game.h"
#include <iostream>
using namespace std;

Game::Game() {
    chessBoard_ = new ChessBoard;
    stack_ = new stack<tuple<int, int, int>>;
}

Game::~Game() {
    delete chessBoard_;
    delete stack_;
}

QVector<qint8> Game::manipulate(const QVector<qint8>& info) {
    /*
     * 1: 落子，由1方发来信息，位置在(2, 3)处
     * 2: 悔棋，由1方发来信息
     * 3: 求和，由1方发来信息
     * 4: 认输，由1方发来信息
     */
    QVector<qint8> res;
    Manipulation* manipulation;
    switch (info[0]) {
        case 1:
            manipulation = new Go {chessBoard_, stack_, info[1], make_pair(info[2], info[3]), static_cast<int>(stack_->size())};
            break;
        case 2:
            manipulation = new Retract {chessBoard_, stack_, info[1]};
            break;
        case 3:
            manipulation = new SueForPeace {chessBoard_, stack_, info[1]};
            break;
        case 4:
            manipulation = new Abort {chessBoard_, stack_, info[1]};
    }
    res = manipulation->main();
    delete manipulation;
    return res;
}

int Game::currentPlayer() const{
    if(stack_->empty()){
        return 1;
    }
    return get<0>(stack_->top()) ^ 1;
}

std::ostream& operator<<(std::ostream& out, const Game& game){
    out << *(game.chessBoard_);
    return out;
}

std::tuple<int, int, int> Game::lastChessMan() const {
    if(stack_->empty()){
        return std::make_tuple(-1, 0 ,0);
    }
    return stack_->top();
}

std::string Game::output(){
    return chessBoard_->output();
}
