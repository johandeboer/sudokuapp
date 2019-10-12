#pragma once

#include <vector>
#include "Sudoku.h"

namespace Sudoku
{
class Cell;

class Rule
{
public:
    Rule();

    Rule(Rule &) = delete;
    Rule & operator = (Rule &) = delete;

    void addCell(std::shared_ptr<Cell> cell);
    const std::vector<std::shared_ptr<Cell>> & cells();
    bool contains(const std::shared_ptr<Cell> & cell);

    virtual std::string name() { return "rule"; }

private:
    std::vector<std::shared_ptr<Cell>> mCells;
};

class HorizontalRule : public Rule
{
public:
    std::string name() override { return "horizontal"; }
};

class VerticalRule : public Rule
{
public:
    std::string name() override { return "vertical"; }
};

class DiagonalRule : public Rule
{
public:
    explicit DiagonalRule(int stride) : Rule(), mStride(stride) {};

    std::string name() override { return "diagonal"; }
    int stride() const { return mStride; }
private:
    int mStride;
};

class BlockRule : public Rule
{
public:
    std::string name() override { return "block"; }
};

}