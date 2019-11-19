#include "pch.h"
#include "Logger.h"

using namespace winrt::Windows::UI::Xaml::Controls;

Logger::Logger(std::shared_ptr<TextBlock> textBlock) : mTextBlock(textBlock)
{
}

void Logger::log(const std::string & text) const
{
    auto newText = mTextBlock->Text() + winrt::to_hstring(text + "\n");
    mTextBlock->Text(newText);
}

void Logger::operator()(const std::string & text) const
{
    log(text);
}