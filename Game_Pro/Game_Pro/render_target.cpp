#include "render_target.h"
#include <cassert>

namespace {
constexpr auto heapType_ = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
}

render_target::~render_target() 
{
    renderTargets_.clear();
}

[[nodiscard]] bool render_target::createBackBuffer(const swap_chain& swapChain) noexcept 
{
    const auto& desc = swapChain.getDesc();

    renderTargets_.resize(desc.BufferCount);

    auto heap = descriptor_heap_container::instance().get(heapType_);                                                       

    auto handle = heap->GetCPUDescriptorHandleForHeapStart();

    for (uint8_t i = 0; i < desc.BufferCount; ++i) {
        const auto hr = swapChain.get()->GetBuffer(i, IID_PPV_ARGS(&renderTargets_[i]));
        if (FAILED(hr)) {
            assert(false && "バックバッファの取得に失敗しました");
            return false;
        }

        device::instance().get()->CreateRenderTargetView(renderTargets_[i].Get(), nullptr,handle);

        handle.ptr += device::instance().get()->GetDescriptorHandleIncrementSize(heapType_);


    }
    return true;
}

[[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE render_target::getCpuDescriptorHandle(UINT index) const noexcept 
{
    if (index >= renderTargets_.size() || !renderTargets_[index]) {
        assert(false && "不正なレンダーターゲットです");
    }

    auto heap = descriptor_heap_container::instance().get(heapType_);

    auto handle = heap->GetCPUDescriptorHandleForHeapStart();

    handle.ptr += index * device::instance().get()->GetDescriptorHandleIncrementSize(heapType_);
    return handle;
}

[[nodiscard]] ID3D12Resource* render_target::get(UINT index) const noexcept {
    if (index >= renderTargets_.size() || !renderTargets_[index]) {
        assert(false && "不正なレンダーターゲットです");
        return nullptr;
    }

    return renderTargets_[index].Get();
}
