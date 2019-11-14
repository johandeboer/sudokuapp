#pragma once

#include "Sudoku.h"

namespace Sudoku
{

class Loader
{
public:
    static std::shared_ptr<Puzzle> parseText(std::string text, ILogger * logger);
};

}