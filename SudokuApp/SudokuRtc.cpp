#include "pch.h"
#include "SudokuRtc.h"
#include "SudokuRtc.g.cpp"

namespace winrt::SudokuApp::implementation
{
    SudokuRtc::SudokuRtc()
    {
        mTitle = L"title";
        mColumns = 9;
    }

    hstring SudokuRtc::Title()
    {
        return mTitle;
    }

    void SudokuRtc::Title(hstring const& value)
    {
        if (mTitle != value)
        {
            mTitle = value;
            mPropertyChanged(*this, Windows::UI::Xaml::Data::PropertyChangedEventArgs{ L"Title" });
        }
    }

    int SudokuRtc::Columns()
    {
        return mColumns;
    }

    void SudokuRtc::Columns(int value)
    {
        if (value != mColumns)
        {
            mColumns = value;
            mPropertyChanged(*this, Windows::UI::Xaml::Data::PropertyChangedEventArgs{ L"Columns" });
        }
    }

    winrt::event_token SudokuRtc::PropertyChanged(Windows::UI::Xaml::Data::PropertyChangedEventHandler const& handler)
    {
        return mPropertyChanged.add(handler);
    }

    void SudokuRtc::PropertyChanged(winrt::event_token const& token) noexcept
    {
        mPropertyChanged.remove(token);
    }
}
