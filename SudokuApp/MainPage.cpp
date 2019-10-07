#include "pch.h"
#include "MainPage.h"
#include "MainPage.g.cpp"
#include <winrt\Windows.UI.Xaml.Controls.h>
#include "Cell.h"
#include "Rule.h"

using namespace winrt;
using namespace Windows::UI::Xaml;
using namespace winrt::Windows::UI::Xaml::Controls;
using namespace Sudoku;

namespace winrt::SudokuApp::implementation
{
    MainPage::MainPage()
    {
        mSudokuRtc = winrt::make<winrt::SudokuApp::implementation::SudokuRtc>();
        InitializeComponent();
        fillGrid(puzzle);
    }

    SudokuApp::SudokuRtc MainPage::SudokuViewModel()
    {
        return mSudokuRtc;
    }

    void MainPage::fillGrid(Sudoku::Puzzle & puzzle)
    {
        auto grid = MainPage::sudokuGrid();

        for (auto i = 0u; i < puzzle.columns(); i++)
        {
            grid.ColumnDefinitions().Append({});
            grid.RowDefinitions().Append({});
        }

        for (auto row = 0u; row < puzzle.rows(); row++)
        {
            for (auto column = 0u; column < puzzle.columns(); column++)
            {
                auto cell = puzzle.cell(row, column);
                fillCell(grid, cell, row, column);
            }
        }
    }

    void MainPage::fillCell(const Grid & grid, Cell * cell, unsigned int row, unsigned int column)
    {
        auto cellGrid = Grid();

        Grid::SetColumn(cellGrid, column);
        Grid::SetRow(cellGrid, row);

        for (auto i = 0u; i < cell->longSide(); i++)
        {
            cellGrid.ColumnDefinitions().Append({});
        }
        for (auto i = 0u; i < cell->shortSide(); i++)
        {
            cellGrid.RowDefinitions().Append({});
        }

        auto r = 0u;
        auto c = 0u;
        for (auto i = 0u; i < cell->digitCount(); i++)
        {
            auto text = TextBlock();

            Grid::SetRow(text, r);
            Grid::SetColumn(text, c);

            text.Text(winrt::to_hstring(std::to_string(i)));
            cellGrid.Margin(ThicknessHelper::FromUniformLength(15));
            cellGrid.Children().Append(text);

            if (++c >= cell->longSide())
            {
                c = 0;
                ++r;
            }
        }

        grid.Children().Append(cellGrid);
    }
}