#include "pch.h"
#include "Cell.h"

using namespace Sudoku;

Cell::Cell() : Cell(9)
{
}

Cell::Cell(unsigned int noDigits) :
    mDigits(noDigits),
    mSqrt(static_cast<unsigned int>(std::ceil(std::sqrt(noDigits))))
{
}

void Sudoku::Cell::setClue(int clue)
{
    mClue = clue;
}

int Sudoku::Cell::clue()
{
    return mClue;
}
