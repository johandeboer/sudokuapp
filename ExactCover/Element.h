#pragma once

#include <memory>
#include <string>

namespace ExactCover
{

struct Column;
struct Row;

struct Element
{
    Element * left;
    Element * right;
    Element * up;
    Element * down;
    Column * column;
    Row * row;

    unsigned int rowIndex;
    unsigned int colIndex;
};

struct Column : Element
{
    explicit Column(const std::string & name) : Name(name) {}

    unsigned int size = 0u;
    std::string Name;
};

struct Row : Element
{
    explicit Row(const std::string & name) : Name(name) {}
    std::string Name;
};

struct Head : Column
{
    Head() : Column("head") {}
};

}