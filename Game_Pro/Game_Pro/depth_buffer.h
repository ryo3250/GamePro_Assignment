#pragma once
#include "device.h"

class depth_buffer final{
public:

	depth_buffer() = default;

	~depth_buffer();

	[[nodiscard]] bool create() noexcept;

	[[nodiscard]] ID3D12Resource* depthBuffer() const noexcept;

	[[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE getCpuDescriptorHandle() const noexcept;

private:
        Microsoft::WRL::ComPtr<ID3D12Resource> depthBuffer_;
    D3D12_CPU_DESCRIPTOR_HANDLE            handle_{};
};
