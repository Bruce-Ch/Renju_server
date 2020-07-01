//
// Created by pc on 2020/6/4.
//

#ifndef RENJU_CHESSMAN_H
#define RENJU_CHESSMAN_H

#include <map>
#include <utility>
typedef std::pair<int, int> Pos;

class ChessMan {
    int color_; // 0为白，1为黑，2为空，3为墙
    int id_; // 从0开始
    Pos pos_; // 从0到14
public:
    ChessMan(int color, int id, Pos pos):color_(color), id_(id), pos_(std::move(pos)){}
    inline int getColor() const {
        return color_;
    }
    inline int getId() const {
        return id_;
    }
    inline Pos getPos() const {
        return pos_;
    }
};


#endif //RENJU_CHESSMAN_H
