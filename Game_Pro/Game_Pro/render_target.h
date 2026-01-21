#pragma once

#include "device.h"
#include "swap_chain.h"
#include "descriptor_heap.h"
#include <vector>

class render_target final{
public:

	render_target() = default;

	~render_target();

	[[nodiscard]] bool createBackBuffer(const swap_chain& swapChain) noexcept;

	[[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE getCpuDescriptorHandle(UINT index) const noexcept;

	[[nodiscard]] ID3D12Resource* get(UINT index) const noexcept;

private:
        std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> renderTargets_{};
};
