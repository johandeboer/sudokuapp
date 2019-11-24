#pragma once

#include "MainPage.g.h"
#include "Sudoku.h"
#include "SudokuRtc.h"
#include "Logger.h"

namespace winrt::SudokuApp::implementation
{
    struct MainPage : MainPageT<MainPage>
    {
        MainPage();

        SudokuApp::SudokuRtc SudokuViewModel();

    private:
        void fillGrid(const std::shared_ptr<Sudoku::Puzzle> & puzzle);
        void addBorders(const Windows::UI::Xaml::Controls::Grid & grid, const std::shared_ptr<Sudoku::Puzzle> & puzzle);
        void addDiagonals(const Windows::UI::Xaml::Controls::Grid & grid, const std::shared_ptr<Sudoku::Puzzle> & puzzle);
        void fillClue(const Windows::UI::Xaml::Controls::Grid & grid, const std::shared_ptr<Sudoku::Cell> & cell, unsigned int row, unsigned int column);
        void fillCell(const Windows::UI::Xaml::Controls::Grid & grid, const std::shared_ptr<Sudoku::Cell> & cell, unsigned int row, unsigned int column);

        void testDLX(const std::shared_ptr<Sudoku::Puzzle> & puzzle);
        void reduce2(const std::shared_ptr<Sudoku::Puzzle>& puzzle);
        std::tuple<std::vector<std::vector<int>>, std::vector<std::string>, std::vector<std::string>> reduce(const std::shared_ptr<Sudoku::Puzzle> & puzzle);

        Windows::Foundation::IAsyncAction loadPuzzle(std::string filename);

        std::shared_ptr<Sudoku::Puzzle> mPuzzle { nullptr };
        winrt::SudokuApp::SudokuRtc mSudokuRtc { nullptr };
        std::shared_ptr<Logger> mLogger { nullptr };
    };
}

namespace winrt::SudokuApp::factory_implementation
{
    struct MainPage : MainPageT<MainPage, implementation::MainPage>
    {
    };
}
