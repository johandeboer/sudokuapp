#pragma once

#include <memory>
#include <string>

namespace ExactCover
{

struct Column;
struct Row;

struct Element
{
    Element() {}
    explicit Element(size_t row, size_t col);

    Element * left = nullptr;
    Element * right = nullptr;
    Element * up = nullptr;
    Element * down = nullptr;
    Column * column = nullptr;
    Row * row = nullptr;

    size_t rowIndex = 0;
    size_t colIndex = 0;
};

struct Column : Element
{
    explicit Column(const std::string & name) :
        Name(name)
    {
    }

    void addElement(Element * element);

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
    unsigned int value;
};

struct Head : Column
{
    Head() : Column("head")
    { }
};

}