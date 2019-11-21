#pragma once

#include <vector>
#include <memory>
#include "Element.h"
#include "ILogger.h"

namespace ExactCover
{

// Implements Knuth's DLX algorithm
// The DLX algorithm is the dancing links version of algorithm X
// https://en.wikipedia.org/wiki/Dancing_Links
class Solver
{
public:
    std::vector<std::vector<Element *>> search(Head * head, const Sudoku::ILogger & logger);

    void log(std::vector<Element *> solution, const Sudoku::ILogger & logger);

private:
    std::vector<Element *> search(Head * head, unsigned int k, std::vector<Element *> solution, const Sudoku::ILogger & logger);
    Column * selectColumn(Head * head);

    void cover(Column * column);
    void uncover(Column * column);

    std::vector<std::vector<Element *>> mSolutions;
};

}