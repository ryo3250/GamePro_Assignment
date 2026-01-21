#pragma once
#include "device.h"

class command_queue final
{
public:
	command_queue() = default;

	~command_queue() = default;

	[[nodiscard]] bool create() noexcept;

	[[nodiscard]] ID3D12CommandQueue* get() const noexcept;

private:
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue_{};
};

