#pragma once

namespace Sudoku
{

class Cell
{
public:
    Cell();
    explicit Cell(unsigned int noDigits);

    unsigned int digitCount() { return mDigits; }
    unsigned int longSide() { return mSqrt; }
    unsigned int shortSide() { return static_cast<unsigned int>(std::ceil(mDigits / mSqrt)); }

private:
    unsigned int mDigits;
    unsigned int mSqrt;
};

}