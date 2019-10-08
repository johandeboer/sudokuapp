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
        void fillGrid(Sudoku::Puzzle & puzzle);
        void fillClue(const Windows::UI::Xaml::Controls::Grid & grid, Sudoku::Cell * cell, unsigned int row, unsigned int column);
        void fillCell(const Windows::UI::Xaml::Controls::Grid & grid, Sudoku::Cell * cell, unsigned int row, unsigned int column);

        Windows::Foundation::IAsyncAction loadPuzzle(std::string filename);

        Sudoku::Puzzle puzzle = Sudoku::Puzzle(9, 9);
        winrt::SudokuApp::SudokuRtc mSudokuRtc{ nullptr };
    };
}

namespace winrt::SudokuApp::factory_implementation
{
    struct MainPage : MainPageT<MainPage, implementation::MainPage>
    {
    };
}
