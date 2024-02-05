#ifndef VISUALCONSTANST_H
#define VISUALCONSTANST_H

enum class SENDTYPE{
    CHAR,
    INTEGER,
    FLOAT,
    CLEAR,
    NEXTLINE
};

static const char S_CHAR = 0XA;
static const char S_CLEAR = 0XB;
static const char S_CHANGELINE = 0XC;
static const char S_NUM = 0XD;

static const int MAX_LINES = 2;

#endif // VISUALCONSTANST_H
