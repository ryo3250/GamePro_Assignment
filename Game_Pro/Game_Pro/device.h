#pragma once

#include "DXGI.h"
#include <d3d12.h>
#include <wrl/client.h>
#include "./util.h"


class device final
{
public:

	static device& instance() noexcept 
	{
		static device instance;
		return instance;
	}

	[[nodiscard]] bool create() noexcept;

	[[nodiscard]] ID3D12Device* get() const noexcept;

	[[nodiscard]] const DXGI& dxgi() const noexcept;

private:

	device() = default;

	~device() = default;

	device(const device& r) = delete;
	device& operator = (const device& r) = delete;
	device(device&& r) = delete;
	device& operator = (device&& r) = delete;

private:
	DXGI dxgiInstance_{};
	Microsoft::WRL::ComPtr<ID3D12Device> device_{};

};

