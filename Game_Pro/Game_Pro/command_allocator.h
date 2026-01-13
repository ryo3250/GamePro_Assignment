#pragma once

#include "device.h"
class command_allocator final
{
public:

	command_allocator() = default;

	~command_allocator() = default;

	[[nodiscard]] bool create(const D3D12_COMMAND_LIST_TYPE type) noexcept;

	void reset() noexcept;

	[[nodiscard]] ID3D12CommandAllocator* get() const noexcept;

	[[nodiscard]] D3D12_COMMAND_LIST_TYPE getType() const noexcept;

private:
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator_{};
	D3D12_COMMAND_LIST_TYPE type_{};
};

