#pragma once

#include "ILogger.h"
#include "winrt/Windows.UI.Xaml.Controls.h"

class Logger : public Sudoku::ILogger
{
public:
    Logger(std::shared_ptr<winrt::Windows::UI::Xaml::Controls::TextBlock> textBlock);
    void log(std::string text) override;

private:
    std::shared_ptr < winrt::Windows::UI::Xaml::Controls::TextBlock> mTextBlock;
};