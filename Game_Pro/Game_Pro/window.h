#pragma once

#include <Windows.h>
#include <string>

class window final {
public:

    static window& instance() noexcept 
    {
        static window instance;
        return instance;
    }

    [[nodiscard]] HRESULT create(HINSTANCE instance, int width, int height, std::string_view name) noexcept;

    [[nodiscard]] bool messageLoop() const noexcept;

    [[nodiscard]] HWND handle() const noexcept;

    [[nodiscard]] std::pair<int, int> size() const noexcept;

private:
    window() = default;

    ~window() = default;

    window(const window& r)            = delete;
    window& operator=(const window& r) = delete;
    window(window&& r)                 = delete;
    window& operator=(window&& r)      = delete;

private:
    HWND handle_{};
    int  witdh_{};
    int  height_{};
};
