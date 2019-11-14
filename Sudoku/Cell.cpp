#include "pch.h"
#include "Cell.h"
#include "Rule.h"

using namespace Sudoku;

Cell::Cell() : Cell(9)
{
}

Cell::Cell(unsigned int noDigits) :
    mDigits(noDigits),
    mBits((1 << static_cast<decltype(mBits)>(noDigits)) - 1)
{
}

int Cell::clue()
{
    return mClue;
}

void Cell::setClue(unsigned int clue)
{
    mClue = clue;
    mBits = 0;

    for (const auto & rule : mRules)
    {
        for (const auto & cell : rule->cells())
        {
            assert(cell.get() == this || (cell.get() != this && cell->clue() != clue));
            cell->unset(clue);
        }
    }
}

bool Sudoku::Cell::hasClue()
{
    return mClue != -1;
}

// FIXME: not used?!?
void Cell::unset(unsigned int digit)
{
    mBits &= ~(1 << (digit - 1));
}

// candidate
bool Cell::isSet(unsigned int digit)
{
    auto mask = 1 << (static_cast<uint64_t>(digit) - 1);
    return (mBits & mask) != 0;
}

// TODO: rename?
std::optional<unsigned int> Sudoku::Cell::isSolved()
{
    uint64_t clueBits = 1;
    auto i = 1u;

    while (i <= mDigits)
    {
        if (mBits == clueBits)
        {
            return std::optional<unsigned int>(i);
        }

        clueBits = clueBits << 1;
        i++;
    }

    return std::nullopt;
}

const std::vector<std::shared_ptr<Rule>> & Sudoku::Cell::rules()
{
    return mRules;
}

void Cell::addRule(const std::shared_ptr<Rule> & rule)
{
    mRules.push_back(rule);
}
