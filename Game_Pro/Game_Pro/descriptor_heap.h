#pragma once
#include<d3d12.h>
#include<unordered_map>
#include<optional>
#include<memory>

class DescriptorHeap;

class descriptor_heap_container final
{
public:

	static descriptor_heap_container& instance() noexcept 
	{
		static descriptor_heap_container instance;
		return instance;
	}


	[[nodiscard]] bool create(D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptors, bool shaderVisible = false) noexcept;

	void applyPendingFree() noexcept;

	[[nodiscard]] ID3D12DescriptorHeap* get(D3D12_DESCRIPTOR_HEAP_TYPE type) const noexcept;

	[[nodiscard]] std::optional<UINT> allocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE type) noexcept;

	void releaseDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE type, UINT descriptorIndex) noexcept;

private:

	descriptor_heap_container();

	~descriptor_heap_container();

	descriptor_heap_container(const descriptor_heap_container& r) = delete;
	descriptor_heap_container& operator=(const descriptor_heap_container& r) = delete;
	descriptor_heap_container(descriptor_heap_container&& r) = delete;
	descriptor_heap_container& operator=(descriptor_heap_container&& r) = delete;

private:
	std::unordered_map<UINT, std::unique_ptr<DescriptorHeap>> map_{};
};

