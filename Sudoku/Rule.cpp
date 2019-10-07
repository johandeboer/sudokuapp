#include "pch.h"
#include "Rule.h"
#include "Cell.h"

using namespace Sudoku;

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
