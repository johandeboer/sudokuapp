#pragma once

#include <memory>
#include <string>

namespace ExactCover
{

struct Column;
struct Row;

struct Element
{
    Element();
    explicit Element(ExactCover::Column * theColumn, ExactCover::Row * theRow);

    Element * left;
    Element * right;
    Element * up;
    Element * down;
    Column * column = nullptr;
    Row * row = nullptr;

//    unsigned int rowIndex;
//    unsigned int colIndex;
};

struct Column : Element
{
    Column() : Element()
    { }

    explicit Column(const std::string & name) :
        Element(),
        Name(name)
    {
        column = this;
    }

    unsigned int size = 0u;
    std::string Name;
};

struct Row : Element
{
    explicit Row(const std::string & name) :
        Element(),
        Name(name)
    { }

    std::string Name;
};

struct Head : Column
{
    Head() :
        Column("head")
    { }
};

}