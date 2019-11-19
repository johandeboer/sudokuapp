#pragma once

#include <memory>
#include <string>

namespace ExactCover
{

struct Column;

struct Element
{
    Element * left;
    Element * right;
    Element * up;
    Element * down;
    Column * column;

    unsigned int row;
    unsigned int col;
};

struct Column : Element
{
    unsigned int size; // () { return mCount; };
    std::string name;

//private:
//    unsigned int mCount;
};

struct Head : Column
{
    Head() { name = "head"; }
};

}