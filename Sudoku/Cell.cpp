#include "pch.h"
#include "Cell.h"
#include "Rule.h"

using namespace Sudoku;

Cell::Cell() : Cell(9)
{
}

Cell::Cell(unsigned int noDigits) :
    mDigits(noDigits),
    mBits((2 << noDigits) - 1)
{
}

void Sudoku::Cell::setClue(unsigned int clue)
{
    mClue = clue;

    for (auto * rule : mRules)
    {
        for (auto * cell : rule->cells())
        {
            cell->unset(clue);
        }
    }
}

void Sudoku::Cell::unset(unsigned int digit)
{
    mBits &= ~(2 << (digit - 1));
}

void Sudoku::Cell::addRule(Rule * rule)
{
    mRules.push_back(rule);
}

int Sudoku::Cell::clue()
{
    return mClue;
}
