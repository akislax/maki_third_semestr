#pragma once
#include <iostream>
#include <sstream>
#include <string>

// Перехват вывода cout - то же самое, что CaptureStdout/GetCapturedStdout в Google Test.
// startCapture(): все, что дальше печатается через cout, не выводится на экран, а запоминается.
// stopCapture(): возвращает запомненное и снова включает обычный вывод.

inline std::stringstream& captureBuffer()
{
    static std::stringstream buffer;
    return buffer;
}

inline std::streambuf*& oldCoutBuffer()
{
    static std::streambuf* old = nullptr;
    return old;
}

inline void startCapture()
{
    captureBuffer().str("");
    captureBuffer().clear();
    oldCoutBuffer() = std::cout.rdbuf(captureBuffer().rdbuf());
}

inline std::string stopCapture()
{
    std::cout.rdbuf(oldCoutBuffer());
    return captureBuffer().str();
}
