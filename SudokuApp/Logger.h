#pragma once

#include "ILogger.h"
#include "winrt/Windows.UI.Xaml.Controls.h"

class Logger : public Sudoku::ILogger
{
public:
    Logger(std::shared_ptr<winrt::Windows::UI::Xaml::Controls::TextBlock> textBlock);

    void log(const std::string & text) const override;
    void operator()(const std::string & text) const override;

private:
    std::shared_ptr < winrt::Windows::UI::Xaml::Controls::TextBlock> mTextBlock;
};