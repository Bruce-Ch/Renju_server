//
// Created by pc on 2020/6/4.
//

#ifndef RENJU_CHESSBOARD_H
#define RENJU_CHESSBOARD_H

#include <ostream>
#include <vector>
#include <tuple>
#include "ChessMan.h"

class ChessBoard {
    std::vector<std::vector<ChessMan*>> chessManPtrMatrix;
public:
    ChessBoard(): chessManPtrMatrix(17, std::vector<ChessMan*>(17)){
        for(int i = 0; i < 17; i++){
            for(int j = 0; j < 17; j++){
                if(i == 0 || i == 16 || j == 0 || j == 16){
                    chessManPtrMatrix[i][j] = new ChessMan{3, -1, std::make_pair(i, j)};
                } else {
                    chessManPtrMatrix[i][j] = new ChessMan{2, -1, std::make_pair(i, j)};
                }
            }
        }
    }
    ~ChessBoard();
    bool posIsEmpty(Pos pos);
    ChessMan* getChessManByPos(Pos pos);
    ChessMan* getChessManById(int id);
    ChessMan* getChessManWithDir(Pos pos, int dir); // 以上为0，按顺时针计算
    ChessMan* getChessManWithDir(ChessMan* chessMan, int dir);
    ChessMan* setChessMan(int color, int id, Pos pos);
    ChessMan* setChessMan(ChessMan* chessMan); // 以参数为样板设置，并非将参数直接设置
    void eraseChessMan(ChessMan* chessMan);
    friend std::ostream& operator<<(std::ostream& out, const ChessBoard& chessBoard);

private:
    static void checkOutOfRange(int row, int col);
    static void checkOutOfRange(Pos pos);
    static bool inRange(Pos pos);
    static bool inRange(int row, int col);
};


#endif //RENJU_CHESSBOARD_H
