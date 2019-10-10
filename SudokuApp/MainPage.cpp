#include "pch.h"
#include "MainPage.h"
#include "MainPage.g.cpp"
#include <winrt/Windows.UI.Xaml.Controls.h>
#include "winrt/Windows.Storage.h"
#include "winrt/Windows.UI.Text.h"
#include "winrt/Windows.UI.Xaml.Media.h"
#include "Cell.h"
#include "Rule.h"
#include "Loader.h"

using namespace winrt;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Text;
using namespace Windows::Storage;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::Foundation;
using namespace Sudoku;

namespace winrt::SudokuApp::implementation
{
    MainPage::MainPage()
    {
        mSudokuRtc = winrt::make<winrt::SudokuApp::implementation::SudokuRtc>();
        InitializeComponent();

        auto result = loadPuzzle(R"(puzzles\example1.sudoku)");
    }

    IAsyncAction MainPage::loadPuzzle(std::string filename)
    {
        winrt::apartment_context ui_thread;
        co_await winrt::resume_background();

        auto file = co_await KnownFolders::DocumentsLibrary().GetFileAsync(winrt::to_hstring(filename));
        auto text = co_await Windows::Storage::FileIO::ReadTextAsync(file);

        co_await ui_thread; 

        puzzle = Loader::parseText(winrt::to_string(text));
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
                if (cell->clue() == -1)
                {
                    fillCell(grid, cell, row, column);
                }
                else
                {
                    fillClue(grid, cell, row, column);
                }
            }
        }
    }

    void MainPage::fillClue(const Grid & grid, Cell * cell, unsigned int row, unsigned int column)
    {
        auto text = TextBlock();
        text.Text(winrt::to_hstring(std::to_string(cell->clue())));
        text.TextAlignment(TextAlignment::Center);
        text.FontSize(5*11);
        text.FontWeight(FontWeights::Bold());
        Grid::SetColumn(text, column);
        Grid::SetRow(text, row);
        grid.Children().Append(text);
    }

    void MainPage::fillCell(const Grid & grid, Cell * cell, unsigned int row, unsigned int column)
    {
        auto longSide = static_cast<unsigned int>(std::ceil(std::sqrt(cell->digitCount())));
        auto shortSide = static_cast<unsigned int>(std::ceil(cell->digitCount() / longSide));

        auto cellGrid = Grid();

        Grid::SetColumn(cellGrid, column);
        Grid::SetRow(cellGrid, row);

        for (auto i = 0u; i < longSide; i++)
        {
            cellGrid.ColumnDefinitions().Append({});
        }
        for (auto i = 0u; i < shortSide; i++)
        {
            cellGrid.RowDefinitions().Append({});
        }

        auto r = 0u;
        auto c = 0u;
        for (auto i = 0u; i < cell->digitCount(); i++)
        {
            auto text = TextBlock();
            text.Text(winrt::to_hstring(std::to_string(i)));
            text.Foreground(SolidColorBrush(Windows::UI::Colors::DimGray()));

            Grid::SetRow(text, r);
            Grid::SetColumn(text, c);

            cellGrid.Margin(ThicknessHelper::FromUniformLength(10));
            cellGrid.Children().Append(text);

            if (++c >= longSide)
            {
                c = 0;
                ++r;
            }
        }

        grid.Children().Append(cellGrid);
    }
}