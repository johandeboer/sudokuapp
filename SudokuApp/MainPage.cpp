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
#include "../ExactCover/Element.h"
#include "../ExactCover/ExactCover.h"
#include <cassert>
#include <iostream>
#include <sstream>

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

        auto result = loadPuzzle(R"(puzzles\example1.sudoku)");
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

//        testDLX(mPuzzle);

        reduce2(mPuzzle);

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

    void MainPage::testDLX(const std::shared_ptr<Puzzle> & puzzle)
    {
/*
        std::vector<std::string> columnNames = { "A", "B", "C", "D", "E", "F", "G" };
        std::vector<std::string> rowNames = { "1", "2", "3", "4", "5", "6", "7" };
        std::vector<std::vector<int>> matrix = {
            {0, 0, 1, 0, 1, 1, 0},
            {1, 0, 0, 1, 0, 0, 1},
            {0, 1, 1, 0, 0, 1, 0},
            {1, 0, 0, 1, 0, 0, 0},
            {0, 1, 0, 0, 0, 0, 1},
            {0, 0, 0, 1, 1, 0, 1}
        };
*/
/*
        // example from Wikipedia
        // https://en.wikipedia.org/wiki/Knuth%27s_Algorithm_X
        std::vector<std::string> columnNames = { "1", "2", "3", "4", "5", "6", "7" };
        std::vector<std::string> rowNames = { "A", "B", "C", "D", "E", "F", "G" };
        std::vector<std::vector<int>> matrix = {
            {1, 0, 0, 1, 0, 0, 1},
            {1, 0, 0, 1, 0, 0, 0},
            {0, 0, 0, 1, 1, 0, 1},
            {0, 0, 1, 0, 1, 1, 0},
            {0, 1, 1, 0, 0, 1, 1},
            {0, 1, 0, 0, 0, 0, 1}
        };
*/
/*
        auto [matrix, rowNames, columnNames] = reduce(puzzle);

        const auto nRows = matrix.size();
        const auto nColumns = matrix[0].size();

        auto head = ExactCover::Head();
        auto columns = std::vector<ExactCover::Column>();
        auto rows = std::vector<ExactCover::Row>();
        auto elements = std::vector<std::vector<ExactCover::Element>>(nRows);

        // TODO: check
        assert(nColumns > 1);
        assert(nRows > 1);

        for (auto row = 0u; row < nRows; row++)
        for (auto col = 0u; col < nColumns; col++)
        {
            if (col == 0)
            {
                rows.emplace_back(ExactCover::Row(rowNames[row]));
            }
            if (row == 0)
            {
                columns.emplace_back(ExactCover::Column(columnNames[col]));
            }
            elements[row].emplace_back<ExactCover::Element>({});
        }

        for (auto col = 0u; col < columns.size(); col++)
        {
            auto & column = columns[col];

            column.up = &elements[nRows - 1][col];
            column.down = &elements[0][col];
            column.left = col == 0 ? &head : &columns[col - 1];
            column.right = col == nColumns - 1 ? &head : &columns[col + 1];
        }

        head.right = &columns[0];
        head.left = &columns[nColumns - 1];

        for (auto row = 0u; row < nRows; row++)
        for (auto col = 0u; col < nColumns; col++)
        {
            auto & element = elements[row][col];

//            element.rowIndex = row;
//            element.colIndex = col;

            element.column = &columns[col];
            element.row = &rows[row];

            element.up = row == 0 ? &columns[col] : &elements[row - 1][col];
            element.down = row == nRows - 1 ? &columns[col] : &elements[row + 1][col];
            element.left = &elements[row][col == 0 ? nColumns - 1 : col - 1];
            element.right = &elements[row][col == nColumns - 1 ? 0 : col + 1];
        }

        for (auto row = 0u; row < nRows; row++)
        for (auto col = 0u; col < nColumns; col++)
        {
            if (matrix[row][col] == 0)
            {
                auto & element = elements[row][col];
                element.right->left = element.left;
                element.left->right = element.right;
                element.down->up = element.up;
                element.up->down = element.down;
            }
            else
            {
                columns[col].size++;
            }
        }

        auto solver = ExactCover::Solver();
        auto solutions = solver.search(&head, *mLogger);
        for (auto i = 0u; i < solutions.size(); ++i)
        {
            mLogger->log(std::to_string(i + 1).insert(0, "solution #"));
            solver.log(solutions[i], *mLogger);
        }
*/
    }

    void MainPage::reduce2(const std::shared_ptr<Sudoku::Puzzle>& puzzle)
    {
        using namespace ExactCover;

        auto nameCreator = [](std::string prefix1, unsigned int n, std::string prefix2, unsigned int m)
        {
            return std::string(prefix1).append(std::to_string(n)).append(prefix2).append(std::to_string(m));
        };

        auto head = std::shared_ptr<Head>();
        auto rows = std::vector<std::shared_ptr<Row>>();
        auto elements = std::vector< std::shared_ptr<Element>>();

        const auto k = puzzle->digits();
        const auto nCols = k * k * 4;
        const auto maxRows = k * k * k;
        auto columns = std::vector<std::shared_ptr<Column>>();

        columns.reserve(nCols);
        rows.reserve(maxRows);
        elements.reserve(maxRows);

        for (auto i = 0u; i < k*k; ++i)
        {
            const auto row = i / k;
            const auto col = i % k;
            columns.push_back(std::make_shared<Column>(nameCreator("R", row + 1, "C", col + 1)));
            columns.push_back(std::make_shared<Column>(nameCreator("R", row + 1, "#", col + 1)));
            columns.push_back(std::make_shared<Column>(nameCreator("C", row + 1, "#", col + 1)));
            columns.push_back(std::make_shared<Column>(nameCreator("B", row + 1, "#", col + 1)));
        }

        for (const auto & cell : puzzle->grid())
        {
            auto [row, col] = puzzle->toRowColumn(cell);
            for (auto digit = 0u; digit < puzzle->digits(); ++digit)
            {
                if (cell->isSet(digit))
                {
                    rows.push_back(std::make_shared<Row>(nameCreator("R", row + 1, "C", col + 1).append("#").append(std::to_string(digit + 1))));

                    const auto i = (row * k + col) * 4;

                    std::vector<std::shared_ptr<Element>> constraints{};
                    constraints.push_back(std::make_shared<Element>(columns[i + 0], rows.back()));
                    constraints.push_back(std::make_shared<Element>(columns[i + 1], rows.back()));
                    constraints.push_back(std::make_shared<Element>(columns[i + 2], rows.back()));
                    constraints.push_back(std::make_shared<Element>(columns[i + 3], rows.back()));

                    for (auto j = 0u; j < constraints.size(); ++j)
                    {
                        constraints[j]->left = constraints[j == 0 ? constraints.size() - 1 : j - 1];
                        constraints[j]->right = constraints[j == constraints.size() - 1 ? 0 : j + 1];
                    }

                    elements.insert(elements.end(), constraints.begin(), constraints.end());
                }
            }
        }

//        auto columnIsEmpty = [](const auto & column) { return column->size == 0; };
//        columns.erase(std::remove_if(columns.begin(), columns.end(), columnIsEmpty), columns.end());

        columns.push_back(head);

        for (auto i = 0u; i < columns.size(); ++i)
        {
            columns[i]->left = columns[i == 0 ? columns.size() - 1 : i - 1];
            columns[i]->right = columns[i == columns.size() - 1 ? 0 : i + 1];
        }

        auto solver = ExactCover::Solver();
        auto solutions = solver.search(head, *mLogger);
        for (auto i = 0u; i < solutions.size(); ++i)
        {
            mLogger->log(std::to_string(i + 1).insert(0, "solution #"));
            solver.log(solutions[i], *mLogger);
        }
    }

    void MainPage::connectRow(std::vector<std::shared_ptr<ExactCover::Element>> & elements)
    {
        for (auto i = 0u; i < elements.size(); ++i)
        {
            elements[i]->left = elements[i == 0 ? elements.size() - 1 : i - 1];
            elements[i]->right = elements[i == elements.size() - 1 ? 0 : i + 1];
        }
    }

    std::tuple<std::vector<std::vector<int>>, std::vector<std::string>, std::vector<std::string>> MainPage::reduce(const std::shared_ptr<Sudoku::Puzzle>& puzzle)
    {
        const auto k = puzzle->rows();
        const auto l = static_cast<size_t>(std::sqrtf(k));
        const auto nRows = k * k * k;
        const auto nCols = k * k * 4;
        auto matrix = std::vector<std::vector<int>>(nRows);
        auto rowNames = std::vector<std::string>(nRows);
        auto columnNames = std::vector<std::string>(nCols);

        for (auto row = 0u; row < nRows; ++row)
        {
            matrix[row].resize(nCols);
            const auto rowIndex = row / (k*k);
            const auto colIndex = row / k % k;
            const auto blockRowIndex = rowIndex / l;
            const auto blockColIndex = colIndex / l;
            const auto n = row % k;
            rowNames[row] = std::string("R").append(std::to_string(rowIndex + 1)).append("C").append(std::to_string(colIndex + 1)).append("#").append(std::to_string(n + 1));

            const auto cell = puzzle->cell(rowIndex, colIndex);
            if (cell->hasClue())
            {
                continue;
            }

            auto offset = 0u;
            matrix[row][offset + rowIndex*k + colIndex] = 1; // cell constraint
            offset += k*k;
            matrix[row][offset + rowIndex*k + n] = 1; // row constraint
            offset += k*k;
            matrix[row][offset + colIndex*k + n] = 1; // column constraint
            offset += k*k;
            matrix[row][offset + blockRowIndex*k*l + blockColIndex*k + n] = 1; // block constraint
        }

        for (auto col = 0u; col < k*k; ++col)
        {
            const auto m = col / k;
            const auto n = col % k;
            columnNames[col + 0*k*k] = std::string("R").append(std::to_string(m + 1)).append("C").append(std::to_string(n + 1));
            columnNames[col + 1*k*k] = std::string("R").append(std::to_string(m + 1)).append("#").append(std::to_string(n + 1));
            columnNames[col + 2*k*k] = std::string("C").append(std::to_string(m + 1)).append("#").append(std::to_string(n + 1));
            columnNames[col + 3*k*k] = std::string("B").append(std::to_string(m + 1)).append("#").append(std::to_string(n + 1));
        }


        std::stringstream outFile;
        for (auto row = 0u; row < nRows; ++row)
        {
            if (row % k == 0)
            {
                outFile << std::string(nCols + 10, '-') << std::endl;
            }

            outFile << rowNames[row] << " ";

            for (auto col = 0u; col < nCols; ++col)
            {
                if (col % (k*k) == 0)
                {
                    outFile << "|";
                }

                outFile << (matrix[row][col] == 1 ? "1" : " ");
            }
            outFile << std::endl;
        }
        mLogger->log(outFile.str());

        return {std::move(matrix), std::move(rowNames), std::move(columnNames)};
    }
}
