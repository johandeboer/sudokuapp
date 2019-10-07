#include "pch.h"
#include "Rule.h"
#include "Cell.h"

using namespace Sudoku;

Rule Rule::createLine(Puzzle & puzzle, unsigned int startIndex, unsigned int count, unsigned int stride)
{
    auto line = Rule();
    for (auto i = 0u; i < count; ++i)
    {
        line.addCell(puzzle.cell(startIndex + i * stride));
    }
    return line;
}

Rule Rule::createBlock(Puzzle & puzzle, unsigned int row, unsigned int column, unsigned int size)
{
    auto block = Rule();
    for (auto i = 0u; i < size; ++i)
    for (auto j = 0u; j < size; ++j)
    {
        block.addCell(puzzle.cell(row + i, column + j));
    }
    return block;
}

Rule::Rule()
{
}

void Rule::setCells(std::vector<Cell *> cells) 
{
    mCells = cells;
}

void Rule::addCell(Cell * cell)
{
    mCells.push_back(cell);
}
