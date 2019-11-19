#pragma once

#include <string>

namespace Sudoku
{

class ILogger
{
public:
    virtual void log(const std::string & text) const = 0;
    virtual void operator()(const std::string & text) const = 0;
};

}