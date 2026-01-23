#pragma once
#include <Windows.h>
#include <cstdint>
#include <array>

class input final
{
public:

	static input& instance() noexcept 
	{
		static input instance;
		return instance;
	}

	[[nodiscard]] bool getKey(uint16_t sKey) const noexcept;

	[[nodiscard]] bool getTrigger(uint16_t sKey) const noexcept;

	void updateKeyState(void* pState) noexcept;

	void updatePrevKeyState() noexcept;

private:

	input() = default;

	~input() = default;

	input(const input& r)            = delete;
    input& operator=(const input& r) = delete;
    input(input&& r)                 = delete;
    input& operator=(input&& r)      = delete;


private:
        std::array<byte, 256> keyState_{};      /// ハードから設定されたキー情報配列
        std::array<byte, 256> prevKeyState_{};  /// ハードから設定されたキー情報配列
};

