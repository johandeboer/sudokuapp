#pragma once

#include "Sudoku.h"

namespace Sudoku
{

class Loader
{
public:
    static Puzzle parseText(std::string text);
};

}