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

    void addCell(Cell * cell);
    const std::vector<Cell *> & cells();

    virtual std::string name() { return "rule"; }

protected:
    void setCells(std::vector<Cell *> cells);

private:
    std::vector<Cell *> mCells;
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

class BlockRule : public Rule
{
public:
    std::string name() override { return "block"; }
};

}