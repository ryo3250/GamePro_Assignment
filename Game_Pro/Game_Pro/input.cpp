#include "input.h"

[[nodiscard]] bool input::getKey(uint16_t sKey) const noexcept
{
	return (keyState_[sKey] & 0x80) != 0;
}

[[nodiscard]] bool input::getTrigger(uint16_t sKey) const noexcept {
    return ((keyState_[sKey] & 0x80) && !(prevKeyState_[sKey] & 0x80));
}

void input::updateKeyState(void* pState) noexcept {
    memcpy_s(keyState_.data(), keyState_.size(), pState, keyState_.size());
}

void input::updatePrevKeyState() noexcept {
    memcpy_s(prevKeyState_.data(), prevKeyState_.size(), keyState_.data(), keyState_.size());
}