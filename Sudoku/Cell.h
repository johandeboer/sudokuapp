#pragma once

namespace Sudoku
{

class Rule;

class Cell
{
public:
    Cell();
    explicit Cell(unsigned int noDigits);

    Cell(Cell &) = delete;
    Cell & operator = (Cell &) = delete;

    int clue();
    void setClue(unsigned int clue);
    bool hasClue();

    void unset(unsigned int digit);
    bool isSet(unsigned int digit);
    std::optional<unsigned int> isSolved();

    const std::vector<std::shared_ptr<Rule>> & rules();
    void addRule(const std::shared_ptr<Rule> & rule);

    unsigned int digitCount() { return mDigits; }

private:
    unsigned int mDigits;
    uint64_t mBits; // FIXME: candidates?
    int mClue = -1;

    std::vector<std::shared_ptr<Rule>> mRules;
};

}