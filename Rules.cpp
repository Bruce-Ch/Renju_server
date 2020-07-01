//
// Created by pc on 2020/6/4.
//

#include <stdexcept>
#include "MyErrors.h"
#include "Rules.h"
using namespace std;

vector<int> Rules::getLine(ChessMan *chessMan, int dir, int window) {//同1异0空2 一个异则都是异
    assert(chessMan && chessMan->getColor() <= 1);
    vector<int> line(2 * window + 1);
    line[window] = 1;
    int color = chessMan->getColor();
    bool front = true;
    ChessMan* chessManFront = chessMan;
    for(int i = 0; i < window; i++){
        if(front){
            chessManFront = chessBoard_->getChessManWithDir(chessManFront, getReverseDir(dir));
            if(!chessManFront){
                front = false;
                line[window - i - 1] = 0;
            } else if(chessManFront->getColor() == 2){
                line[window - i - 1] = 2;
            } else if(chessManFront->getColor() != color){
                front = false;
                line[window - i - 1] = 0;
            } else if(chessManFront->getColor() == color) {
                line[window - i - 1] = 1;
            } else {
                throw wrong_color("There is no color tagged " + to_string(chessManFront->getColor()));
            }
        } else {
            line[window - i - 1] = 0;
        }
    }
    bool back = true;
    ChessMan* chessManBack = chessMan;
    for(int i = 0; i < window; i++){
        if(back){
            chessManBack = chessBoard_->getChessManWithDir(chessManBack, dir);
            if(!chessManBack){
                back = false;
                line[window + i + 1] = 0;
            } else if(chessManBack->getColor() == 2){
                line[window + i + 1] = 2;
            } else if(chessManBack->getColor() != color){
                back = false;
                line[window + i + 1] = 0;
            } else if(chessManBack->getColor() == color) {
                line[window + i + 1] = 1;
            } else {
                throw wrong_color("There is no color tagged " + to_string(chessManBack->getColor()));
            }
        } else {
            line[window + i + 1] = 0;
        }
    }
    return line;
}

int Rules::cntThree(ChessMan *chessMan, int dir) {
    return 0;
}

int Rules::cntThree(Pos pos, int color, int dir) {
    return 0;
}

int Rules::cntFour(ChessMan *chessMan, int dir) {
    return 0;
}

int Rules::cntFour(Pos pos, int color, int dir) {
    return 0;
}

bool Rules::checkFive(ChessMan *chessMan, int dir) {
    assert(chessMan && chessMan->getColor() <= 1);
    vector<int> line = getLine(chessMan, dir, 5);
    int cnt = 1;
    for(int i = 4; i >= 0; i--){
        if(line[i] == 1){
            cnt++;
        } else {
            break;
        }
    }
    for(int i = 6; i < 10; i++){
        if(line[i] == 1){
            cnt++;
        } else {
            break;
        }
    }
    return cnt == 5;
}

bool Rules::checkLong(ChessMan *chessMan, int dir) {
    assert(chessMan && chessMan->getColor() <= 1);
    vector<int> line = getLine(chessMan, dir, 5);
    int cnt = 1;
    for(int i = 4; i >= 0; i--){
        if(line[i]){
            cnt++;
        } else {
            break;
        }
    }
    for(int i = 6; i < 10; i++){
        if(line[i]){
            cnt++;
        } else {
            break;
        }
    }
    return cnt > 5;
}

bool Rules::checkFive(Pos pos, int color, int dir) {
    ChessMan* chessManOld = chessBoard_->getChessManByPos(pos);
    auto* chessManNew = new ChessMan(color, -1, pos);
    chessBoard_->setChessMan(chessManNew);
    bool ret = checkFive(chessManNew, dir);
    chessBoard_->setChessMan(chessManOld);
    return ret;
}

bool Rules::checkLong(Pos pos, int color, int dir) {
    ChessMan* chessManOld = chessBoard_->getChessManByPos(pos);
    auto* chessManNew = new ChessMan(color, -1, pos);
    chessBoard_->setChessMan(chessManNew);
    bool ret = checkLong(chessManNew, dir);
    chessBoard_->setChessMan(chessManOld);
    return ret;
}

int Rules::finished(ChessMan *chessMan) {
    assert(chessMan && chessMan->getColor() <= 1);
    bool res = false;
    for(int i = 0; i < 4; i++){
        if(checkFive(chessMan, i)){
            res = true;
        }
    }
    return (res ? chessMan->getColor() : 2);
}

int Rules::finished(Pos pos, int color) {
    bool res = false;
    for(int i = 0; i < 4; i++){
        if(checkFive(pos, color, i)){
            res = true;
        }
    }
    return (res ? color : 2);
}