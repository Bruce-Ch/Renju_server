//
// Created by pc on 2020/6/5.
//

#include "Manipulation.h"
using namespace std;

/*
 * 最优先返回命令码
 */

QVector<qint8> Go::main() {
    QVector<qint8> res;
    res.push_back(1);
    /*
     * 成功返回0，
     *      导致游戏结束则后附0，后附游戏胜利者（0白1黑）
     *      否则后附1
     * 不成功返回1，依次附颜色，位置，位置
     */
    if(chessBoard_->posIsEmpty(pos_)){
        res.push_back(0);
        ChessMan* chessMan = chessBoard_->setChessMan(color_, id_, pos_);
        stack_->push(make_tuple(color_, pos_.first, pos_.second));
        Rules rules{chessBoard_};
        int finished = rules.finished(chessMan);
        if(finished != 2){
            res.push_back(0);
            res.push_back(finished);
        } else {
            res.push_back(1);
        }
    } else {
        res.push_back(1);
        res.push_back(color_);
        res.push_back(pos_.first);
        res.push_back(pos_.second);
    }
    return res;
}

QVector<qint8> Retract::main() {
    QVector<qint8> res;
    res.push_back(2);
    /*
     * 成功返回0
     * 不成功返回1
     * 由于栈已经为空导致不能继续悔棋返回2
     */
    if(stack_->empty()){
        res.push_back(2);
        return res;
    }
    tuple<int, int, int> info = stack_->top();
    if(get<0>(info) == color_){
        info = stack_->top();
        chessBoard_->eraseChessMan(chessBoard_->getChessManByPos(make_pair(get<1>(info), get<2>(info))));
        stack_->pop();
    } else {
        if(stack_->size() < 2){
            res.push_back(2);
            return res;
        }
        for(int i = 0; i < 2; i++){
            info = stack_->top();
            chessBoard_->eraseChessMan(chessBoard_->getChessManByPos(make_pair(get<1>(info), get<2>(info))));
            stack_->pop();
        }
    }
    res.push_back(0);
    return res;
}

QVector<qint8> SueForPeace::main() {
    QVector<qint8> res;
    res.push_back(3);
    /*
     * 成功返回0
     * 不成功返回1
     */
    res.push_back(0);
    return res;
}

QVector<qint8> Abort::main(){
    QVector<qint8> res;
    res.push_back(4);
    /*
     * 成功返回0
     * 不成功返回1
     */
     res.push_back(0);
     return res;
}
