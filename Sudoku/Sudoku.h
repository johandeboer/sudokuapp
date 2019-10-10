#pragma once

#include <vector>
#include "Cell.h"
#include "Rule.h"

namespace Sudoku
{

class Rule;
class Cell;

class Puzzle
{
public:
    Puzzle(std::size_t columns, std::size_t rows, unsigned int digits);

    Puzzle(const Puzzle &) = delete;
    Puzzle & operator =(Puzzle const &) = delete;
    Puzzle(const Puzzle &&) = delete;
    Puzzle & operator =(Puzzle const &&) = delete;

    std::size_t columns() const { return mColumns; }
    std::size_t rows() const { return mRows; }
    unsigned int digits() const { return mDigits; }
    std::vector<std::shared_ptr<Cell>> grid() const { return mGrid; }
    std::pair<unsigned int, unsigned int> getCoords(std::shared_ptr<Cell> cell);

    const std::vector<std::shared_ptr<Rule>> & rules();
    void addRule(std::shared_ptr<Rule> && rule);

    std::shared_ptr<Cell> cell(size_t index);
    std::shared_ptr<Cell> cell(size_t row, size_t column);

    unsigned int sweepClues();
    unsigned int sweepUniques();

private:
    std::pair<std::shared_ptr<Cell>, unsigned int> findUnique(const std::shared_ptr<Rule> & rule);

    std::size_t mColumns;
    std::size_t mRows;
    unsigned int mDigits;
    std::vector<std::shared_ptr<Cell>> mGrid;
    std::vector<std::shared_ptr<Rule>> mRules;
};

}