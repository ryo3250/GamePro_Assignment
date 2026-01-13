#include "command_allocator.h"
#include <cassert>

[[nodiscard]] bool command_allocator::create(const D3D12_COMMAND_LIST_TYPE type)noexcept 
{
	type_ = type;

	const auto hr = device::instance().get()->CreateCommandAllocator(type_, IID_PPV_ARGS(&commandAllocator_));
	if (FAILED(hr)) 
	{
		assert(false && "コマンドアロケータの作成に失敗しました");
		return false;
	}

	return true;
}

void command_allocator::reset() noexcept 
{
	if (!commandAllocator_) 
	{
		assert(false && "コマンドアロケータが未作成です");
	}

	commandAllocator_->Reset();
}

[[nodiscard]] ID3D12CommandAllocator* command_allocator::get() const noexcept 
{
	if (!commandAllocator_) 
	{
		assert(false && "コマンドアロケータが未作成です");
	}
	return commandAllocator_.Get();
}

[[nodiscard]] D3D12_COMMAND_LIST_TYPE command_allocator::getType() const noexcept 
{
	if (!commandAllocator_) 
	{
		assert(false && "コマンドリストのタイプが未設定です");
	}

	return type_;
}
