//
// Created by pc on 2020/6/4.
//

#include <tuple>
#include <cassert>
#include "ChessBoard.h"
#include "MyErrors.h"
using namespace std;

ChessBoard::~ChessBoard() {
    for(const auto& chessManPtrRow: chessManPtrMatrix){
        for(auto chessManPtr: chessManPtrRow){
            delete chessManPtr;
        }
    }
}

void ChessBoard::checkOutOfRange(int row, int col) {
    if(row > 15 || row < 1 || col > 15 || col < 1){
        throw out_of_range("Out of range in ChessBoard: row = " + to_string(row) + " col = " + to_string(col));
    }
}

void ChessBoard::checkOutOfRange(Pos pos) {
    checkOutOfRange(pos.first, pos.second);
}

bool ChessBoard::inRange(Pos pos) {
    return inRange(pos.first, pos.second);
}

bool ChessBoard::inRange(int row, int col) {
    return (row >= 1 && row <= 15 && col >= 1 && col <= 15);
}

bool ChessBoard::posIsEmpty(Pos pos) {
    checkOutOfRange(pos);
    return chessManPtrMatrix[pos.first][pos.second]->getColor() == 2;
}

ChessMan * ChessBoard::getChessManById(int id) {
    for(const auto& chessManPtrRow: chessManPtrMatrix){
        for(auto chessManPtr: chessManPtrRow){
            if(chessManPtr && chessManPtr->getId() == id){
                return chessManPtr;
            }
        }
    }
    return nullptr;
}

ChessMan * ChessBoard::getChessManByPos(Pos pos) {
    checkOutOfRange(pos);
    return chessManPtrMatrix[pos.first][pos.second];
}

ChessMan * ChessBoard::getChessManWithDir(Pos pos, int dir) {
    int row, col;
    switch (dir){
        case 0:
            row = pos.first - 1;
            col = pos.second;
            break;
        case 1:
            row = pos.first - 1;
            col = pos.second + 1;
            break;
        case 2:
            row = pos.first;
            col = pos.second + 1;
            break;
        case 3:
            row = pos.first + 1;
            col = pos.second + 1;
            break;
        case 4:
            row = pos.first + 1;
            col = pos.second;
            break;
        case 5:
            row = pos.first + 1;
            col = pos.second - 1;
            break;
        case 6:
            row = pos.first;
            col = pos.second - 1;
            break;
        case 7:
            row = pos.first - 1;
            col = pos.second - 1;
            break;
        default:
            throw bad_direction("Input direction is " + to_string(dir));
    }
    return (inRange(row, col) ? chessManPtrMatrix[row][col] : nullptr);
}

ChessMan * ChessBoard::getChessManWithDir(ChessMan *chessMan, int dir) {
    return getChessManWithDir(chessMan->getPos(), dir);
}

ChessMan* ChessBoard::setChessMan(int color, int id, Pos pos) {
    checkOutOfRange(pos);
    auto* tmp = new ChessMan{color, id, pos};
    delete chessManPtrMatrix[pos.first][pos.second];
    chessManPtrMatrix[pos.first][pos.second] = tmp;
    return tmp;
}

ChessMan* ChessBoard::setChessMan(ChessMan *chessMan) {
    return setChessMan(chessMan->getColor(), chessMan->getId(), chessMan->getPos());
}

void ChessBoard::eraseChessMan(ChessMan *chessMan) {
    if(!chessMan) { return; }
    int row, col;
    tie(row, col) = chessMan->getPos();
    assert(chessManPtrMatrix[row][col] == chessMan);
    auto* tmp = new ChessMan{2, -1, make_pair(row, col)};
    delete chessManPtrMatrix[row][col];
    chessManPtrMatrix[row][col] = tmp;
}

ostream& operator<<(ostream& out, const ChessBoard& chessBoard){
    for(const auto& chessManPtrRow: chessBoard.chessManPtrMatrix){
        for(auto chessManPtr: chessManPtrRow){
            if(chessManPtr->getColor() <= 1){
                out << (chessManPtr->getColor() ? 'X' : 'O');
            } else if(chessManPtr->getColor() == 2) {
                out << "*";
            }
        }
    }
    return out;
}
