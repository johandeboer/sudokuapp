#include "pch.h"
#include "MainPage.h"
#include "MainPage.g.cpp"
#include "winrt/Windows.UI.Xaml.Controls.h"
#include "winrt/Windows.Storage.h"
#include "winrt/Windows.UI.Text.h"
#include "winrt/Windows.UI.Xaml.Media.h"
#include "winrt/Windows.UI.Xaml.Shapes.h"
#include "Cell.h"
#include "Rule.h"
#include "Loader.h"
#include <chrono>
#include <iostream>

using namespace winrt;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Shapes;
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
        auto textBlock = std::make_shared<TextBlock>(logTextBlock());
        mLogger = std::make_shared<Logger>(textBlock);
        auto result = loadPuzzle(R"(puzzles\extreme1.sudoku)");
    }

    IAsyncAction MainPage::loadPuzzle(std::string filename)
    {
        mLogger->log("Loading " + filename);

        winrt::apartment_context ui_thread;
        co_await winrt::resume_background();

        auto file = co_await KnownFolders::DocumentsLibrary().GetFileAsync(winrt::to_hstring(filename));
        auto text = co_await Windows::Storage::FileIO::ReadTextAsync(file);

        co_await ui_thread; 

        mPuzzle = Loader::parseText(winrt::to_string(text), mLogger.get());

        auto start = std::chrono::system_clock::now();
        auto totalChanges = 0u;
        auto sweepCount = 0u;

        while (true) {
            mLogger->log("sweep " + std::to_string(sweepCount));

            auto changes = mPuzzle->sweepUniques(); /// FIXME: sweep -> scan(ning)
            changes += mPuzzle->sweepOverlaps();
            totalChanges += changes;

            if (changes == 0)
            {
                break;
            }
            sweepCount++;
        }

        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;
        mLogger->log(std::to_string(totalChanges) + " steps in " + std::to_string(elapsed_seconds.count() * 1000.0) + "ms");

        fillGrid(mPuzzle);
    }

    SudokuApp::SudokuRtc MainPage::SudokuViewModel()
    {
        return mSudokuRtc;
    }

    void MainPage::fillGrid(const std::shared_ptr<Sudoku::Puzzle> & puzzle)
    {
        auto grid = MainPage::sudokuGrid();

        for (auto i = 0u; i < puzzle->columns(); i++)
        {
            ColumnDefinition column{};
            grid.ColumnDefinitions().Append({});
            RowDefinition row{};
            grid.RowDefinitions().Append({});
        }

        addDiagonals(grid, puzzle);

        for (auto row = 0u; row < puzzle->rows(); row++)
        {
            for (auto column = 0u; column < puzzle->columns(); column++)
            {
                auto cell = puzzle->cell(row, column);
                if (cell == nullptr)
                {
                    continue;
                }

                if (cell->hasClue())
                {
                    fillClue(grid, cell, row, column);
                }
                else
                {
                    fillCell(grid, cell, row, column);
                }
            }
        }

        addBorders(grid, puzzle);
    }

    void MainPage::addDiagonals(const Grid & grid, const std::shared_ptr<Sudoku::Puzzle> & puzzle)
    {
        SolidColorBrush brush { Windows::UI::Colors::LightGray() };
        auto width = grid.ActualWidth() / puzzle->columns();
        auto height = grid.ActualHeight() / puzzle->rows();

        for (const auto & rule : puzzle->rules())
        {
            if (auto diagonalRule = std::dynamic_pointer_cast<DiagonalRule>(rule); diagonalRule != nullptr)
            {
                for (const auto & cell : diagonalRule->cells())
                {
                    auto line = Line();
                    line.X1(diagonalRule->stride() > puzzle->columns() ? 0 : width);
                    line.X2(diagonalRule->stride() > puzzle->columns() ? width : 0);
                    line.Y2(height);
                    line.Stroke(brush);
                    auto [row, column] = puzzle->toRowColumn(cell);
                    Grid::SetColumn(line, static_cast<int>(column));
                    Grid::SetRow(line, static_cast<int>(row));
                    grid.Children().Append(line);
                }
            }
        }
    }

    void MainPage::addBorders(const Grid & grid, const std::shared_ptr<Sudoku::Puzzle> & puzzle)
    {
        auto brush = SolidColorBrush(Windows::UI::Colors::Black());
        for (const auto & rule : puzzle->rules())
        {
            if (auto blockRule = std::dynamic_pointer_cast<BlockRule>(rule); blockRule != nullptr)
            {
                for (const auto & cell : blockRule->cells())
                {
                    auto [row, column] = puzzle->toRowColumn(cell);

                    auto border = Border();
                    Grid::SetColumn(border, static_cast<int>(column));
                    Grid::SetRow(border, static_cast<int>(row));

                    auto top = row == 0 || !rule->contains(puzzle->cell(row - 1, column)) || puzzle->cell(row - 1, column) == nullptr;
                    auto left = column == 0 || !rule->contains(puzzle->cell(row, column - 1)) || puzzle->cell(row, column - 1) == nullptr;
                    auto right = column == puzzle->columns() - 1 || puzzle->cell(row, column + 1) == nullptr;
                    auto bottom = row == puzzle->rows() - 1 || puzzle->cell(row + 1, column) == nullptr;

                    border.BorderThickness(ThicknessHelper::FromLengths(left ? 1 : 0, top ? 1 : 0, right ? 1 : 0, bottom ? 1 : 0));
                    border.BorderBrush(brush);
                    grid.Children().Append(border);
                }
            }
        }
    }

    void MainPage::fillClue(const Grid & grid, const std::shared_ptr<Cell> & cell, unsigned int row, unsigned int column)
    {
        auto text = TextBlock();
        text.Text(winrt::to_hstring(std::to_string(cell->clue())));
        text.TextAlignment(TextAlignment::Center);
        text.FontSize(4.0*11.0);
        text.FontWeight(FontWeights::Bold());
        Grid::SetColumn(text, column);
        Grid::SetRow(text, row);
        grid.Children().Append(text);
    }

    void MainPage::fillCell(const Grid & grid, const std::shared_ptr<Cell> & cell, unsigned int row, unsigned int column)
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
        for (auto i = 1u; i <= cell->digitCount(); i++)
        {
            if (cell->isSet(i))
            {
                auto text = TextBlock();
                text.Text(winrt::to_hstring(std::to_string(i)));
                text.Foreground(SolidColorBrush(Windows::UI::Colors::DimGray()));
                text.FontSize(10.0);

                Grid::SetRow(text, r);
                Grid::SetColumn(text, c);

                cellGrid.Margin(ThicknessHelper::FromUniformLength(10));
                cellGrid.Children().Append(text);
            }

            if (++c >= longSide)
            {
                c = 0;
                ++r;
            }
        }

        grid.Children().Append(cellGrid);
    }
}