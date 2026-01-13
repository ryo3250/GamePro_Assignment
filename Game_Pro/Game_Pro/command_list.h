#pragma once

#include "device.h"
#include "command_allocator.h"

class command_list final
{
public:
	command_list() = default;

	~command_list() = default;

	[[nodiscard]] bool create(const command_allocator& commandAllocator) noexcept;

	void reset(const command_allocator& commandAllocator) noexcept;

	[[nodiscard]] ID3D12GraphicsCommandList* get() const noexcept;

private:
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList_{};
};

