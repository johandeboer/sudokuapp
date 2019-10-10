#pragma once

#include "MainPage.g.h"
#include "Sudoku.h"
#include "SudokuRtc.h"

namespace winrt::SudokuApp::implementation
{
    struct MainPage : MainPageT<MainPage>
    {
        MainPage();

        SudokuApp::SudokuRtc SudokuViewModel();

    private:
        void fillGrid(const std::shared_ptr<Sudoku::Puzzle> & puzzle);
        void addBorders(const Windows::UI::Xaml::Controls::Grid & grid, const std::shared_ptr<Sudoku::Puzzle> & puzzle);
        void fillClue(const Windows::UI::Xaml::Controls::Grid & grid, const std::shared_ptr<Sudoku::Cell> & cell, unsigned int row, unsigned int column);
        void fillCell(const Windows::UI::Xaml::Controls::Grid & grid, const std::shared_ptr<Sudoku::Cell> & cell, unsigned int row, unsigned int column);

        Windows::Foundation::IAsyncAction loadPuzzle(std::string filename);

        std::shared_ptr<Sudoku::Puzzle> mPuzzle { nullptr };
        winrt::SudokuApp::SudokuRtc mSudokuRtc { nullptr };
    };
}

namespace winrt::SudokuApp::factory_implementation
{
    struct MainPage : MainPageT<MainPage, implementation::MainPage>
    {
    };
}
