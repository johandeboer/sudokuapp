#pragma once

#include <vector>
#include <memory>
#include "Element.h"
#include "ILogger.h"

namespace ExactCover
{

using Solution = std::vector<std::shared_ptr<Element>>;

// Implements Knuth's DLX algorithm
// The DLX algorithm is the dancing links version of algorithm X
// https://en.wikipedia.org/wiki/Dancing_Links
class Solver
{
public:
    std::vector<Solution> search(std::shared_ptr<Head> head, const Sudoku::ILogger & logger);

    void log(Solution solution, const Sudoku::ILogger & logger);

private:
    Solution search(
        std::shared_ptr<Head> head,
        unsigned int k,
        Solution solution,
        const Sudoku::ILogger & logger);
    std::shared_ptr<Column> selectColumn(std::shared_ptr<Head> head);

    void cover(std::shared_ptr<Column> column);
    void uncover(std::shared_ptr<Column> column);

    std::vector<Solution> mSolutions;
};

}