#pragma once

#include <vector>
#include "Cell.h"
#include "Rule.h"
#include "Overlap.h"
#include "ILogger.h"

namespace Sudoku
{

class Rule;
class Cell;
class Overlap;

class Puzzle
{
public:
    Puzzle(std::size_t columns, std::size_t rows, unsigned int digits, ILogger * logger);

    Puzzle(const Puzzle &) = delete;
    Puzzle & operator =(Puzzle const &) = delete;
    Puzzle(const Puzzle &&) = delete;
    Puzzle & operator =(Puzzle const &&) = delete;

    std::size_t columns() const { return mColumns; }
    std::size_t rows() const { return mRows; }
    unsigned int digits() const { return mDigits; }
    const std::vector<std::shared_ptr<Cell>> & grid() const { return mGrid; }
    std::pair<size_t, size_t> toRowColumn(std::shared_ptr<Cell> cell);

    std::string toRowColString(std::shared_ptr<Cell> cell);
    std::string toString(const std::vector<std::shared_ptr<Cell>> & cells);

    const std::vector<std::shared_ptr<Rule>> & rules();
    void addRule(std::shared_ptr<Rule> && rule);

    std::shared_ptr<Cell> cell(size_t index);
    std::shared_ptr<Cell> cell(size_t row, size_t column);
    void removeCell(std::size_t row, std::size_t column);

    unsigned int sweepClues();
    unsigned int sweepUniques();
    unsigned int sweepOverlaps();

private:
    std::pair<std::shared_ptr<Cell>, unsigned int> findUnique(const std::shared_ptr<Rule> & rule);
    unsigned int sweepOverlap(const std::vector<std::shared_ptr<Cell>> & overlap, const std::vector<std::shared_ptr<Cell>> & nonOverlapA, const std::vector<std::shared_ptr<Cell>> & nonOverlapB);

    std::size_t mColumns;
    std::size_t mRows;
    unsigned int mDigits;
    std::vector<std::shared_ptr<Cell>> mGrid;
    std::vector<std::shared_ptr<Rule>> mRules;
    std::vector<Overlap> mOverlaps;
    ILogger * mLogger;
};

}