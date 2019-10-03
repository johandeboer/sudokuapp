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
        void fillGrid(Sudoku::Sudoku);

        Sudoku::Sudoku puzzle = Sudoku::Sudoku(9, 9);
        winrt::SudokuApp::SudokuRtc mSudokuRtc{ nullptr };
    };
}

namespace winrt::SudokuApp::factory_implementation
{
    struct MainPage : MainPageT<MainPage, implementation::MainPage>
    {
    };
}
