#include "pch.h"
#include "Sudoku.h"
#include "Cell.h"
#include "Rule.h"

using namespace Sudoku;

Puzzle::Puzzle(unsigned int columns, unsigned int rows) :
    mColumns(columns),
    mRows(rows)
{
    auto cells = rows * columns;
    for (auto i = 0u; i < cells; i++)
    {
        mGrid.emplace_back(Cell(columns));
    }
}

void Puzzle::addRule(const Rule & rule)
{
    mRules.push_back(std::move(rule));
}

Cell * Puzzle::cell(unsigned int index)
{
    return &mGrid[index];
}

Cell * Puzzle::cell(unsigned int row, unsigned int column)
{
    return &mGrid[row * mColumns + column];
}