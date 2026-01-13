#include "command_list.h"
#include <cassert>

[[nodiscard]] bool command_list::create(const command_allocator& commandAllocator) noexcept
{
	const auto hr = device::instance().get()->CreateCommandList(
		0, commandAllocator.getType(), commandAllocator.get(),
		nullptr, IID_PPV_ARGS(&commandList_));
	if (FAILED(hr)) 
	{
		assert(false && "コマンドリストの作成に失敗しました");
		return false;
	}

	commandList_->Close();
	return true;
}

void command_list::reset(const command_allocator& commandAllocator) noexcept
{
	if (!commandList_) 
	{
		assert(false && "コマンドリストが未作成です");
	}

	commandList_->Reset(commandAllocator.get(), nullptr);
}

[[nodiscard]] ID3D12GraphicsCommandList* command_list::get() const noexcept 
{
	if (!commandList_) 
	{
		assert(false && "コマンドリストが未作成です");
	}

	return commandList_.Get();
}