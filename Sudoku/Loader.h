#pragma once

#include "Sudoku.h"

namespace Sudoku
{

class Loader
{
public:
    static Puzzle load(std::string filename);
    static Puzzle parseText(std::string text);

private:
    static Puzzle makePuzzle(std::string name, unsigned int rows, unsigned int columns);
};

}