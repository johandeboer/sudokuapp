#pragma once

#include <vector>
#include <memory>
#include "Element.h"
#include "ILogger.h"

namespace ExactCover
{

using Solution = std::vector<Element *>;

// Implements Knuth's DLX algorithm
// The DLX algorithm is the dancing links version of algorithm X
// https://en.wikipedia.org/wiki/Dancing_Links
class Solver
{
public:
    std::vector<Solution> search(Head * head, const Sudoku::ILogger & logger);
    static void log(const Solution & solution, const Sudoku::ILogger & logger);

private:
    Solution search(
        Head * head,
        unsigned int k,
        Solution solution,
        const Sudoku::ILogger & logger);
    static Column * selectColumn(Head * head);
    static void cover(Column * column);
    static void uncover(Column * column);

    std::vector<Solution> mSolutions;
};

}