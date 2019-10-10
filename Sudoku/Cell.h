#pragma once

namespace Sudoku
{

class Rule;

class Cell
{
public:
    Cell();
    explicit Cell(unsigned int noDigits);

    int clue();
    void setClue(unsigned int clue);

    void unset(unsigned int digit);

    void addRule(Rule * rule);

    unsigned int digitCount() { return mDigits; }

private:
    unsigned int mDigits;
    uint64_t mBits;
    int mClue = -1;

    std::vector<Rule *> mRules;
};

}