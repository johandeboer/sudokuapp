#include "pch.h"
#include "Logger.h"

using namespace winrt::Windows::UI::Xaml::Controls;

Logger::Logger(std::shared_ptr<TextBlock> textBlock) : mTextBlock(textBlock)
{
}

void Logger::log(std::string text)
{
    auto newText = mTextBlock->Text() + winrt::to_hstring(text + "\n");
    mTextBlock->Text(newText);
}
