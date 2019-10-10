#include "pch.h"
#include "Rule.h"
#include "Cell.h"

using namespace Sudoku;

Rule::Rule()
{
}

void Rule::addCell(std::shared_ptr<Cell> cell)
{
    mCells.push_back(cell);
}

const std::vector< std::shared_ptr<Cell>> & Rule::cells()
{
    return mCells;
}

bool Sudoku::Rule::contains(const std::shared_ptr<Cell> & cell)
{
    auto iter = std::find(mCells.begin(), mCells.end(), cell);
    return iter != mCells.end();
}
