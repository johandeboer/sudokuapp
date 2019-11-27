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
    explicit Element(std::shared_ptr<Column> theColumn, std::shared_ptr<Row> theRow);

    std::shared_ptr<Element> left;
    std::shared_ptr<Element> right;
    std::shared_ptr<Element> up;
    std::shared_ptr<Element> down;
    std::shared_ptr<Column> column = nullptr;
    std::shared_ptr<Row> row = nullptr;
};

struct Column : Element
{
    Column() : Element()
    { }

    explicit Column(const std::string & name) :
        Element(),
        Name(name)
    {
        column = std::shared_ptr<Column>(this);
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