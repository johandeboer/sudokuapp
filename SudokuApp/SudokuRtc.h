#pragma once
#include "SudokuRtc.g.h"

namespace winrt::SudokuApp::implementation
{
    struct SudokuRtc : SudokuRtcT<SudokuRtc>
    {
        SudokuRtc();

        hstring Title();
        void Title(hstring const& value);

        int Columns();
        void Columns(int value);

        winrt::event_token PropertyChanged(Windows::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
        void PropertyChanged(winrt::event_token const& token) noexcept;
    private:
        winrt::hstring mTitle;
        int mColumns;

        winrt::event<Windows::UI::Xaml::Data::PropertyChangedEventHandler> mPropertyChanged;
    };
}
