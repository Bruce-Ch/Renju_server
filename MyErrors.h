//
// Created by pc on 2020/6/8.
//

#ifndef RENJU_MYERRORS_H
#define RENJU_MYERRORS_H

#include <exception>
#include <stdexcept>

class bad_direction: public std::invalid_argument{
public:
    using std::invalid_argument::invalid_argument;
};

class wrong_color: public std::invalid_argument{
public:
    using std::invalid_argument::invalid_argument;
};

class bad_pos: public std::invalid_argument{
public:
    using std::invalid_argument::invalid_argument;
};
#endif //RENJU_MYERRORS_H
