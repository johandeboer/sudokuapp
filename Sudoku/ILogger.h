#pragma once

#include <string>

namespace Sudoku
{

class ILogger
{
public:
    virtual void log(std::string text) = 0;
};

}