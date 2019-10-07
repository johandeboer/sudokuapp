#pragma once

namespace Sudoku
{

class Cell
{
public:
    Cell();
    explicit Cell(int noDigits);

    int digitCount() { return mDigits; }
    int longSide() { return mSqrt; }
    int shortSide() { return static_cast<int>(std::ceil(mDigits / mSqrt)); }

private:
    int mDigits;
    int mSqrt;
};

}