#include "fence.h"
#include <cassert>

[[nodiscard]] bool fence::create() noexcept 
{
    HRESULT hr  = device::instance().get()->CreateFence(0, D3D12_FENCE_FLAG_NONE,IID_PPV_ARGS(&fence_));
    if (FAILED(hr)) {
        assert(false && "フェンスの作成に失敗しました");
        return false;
    }

    waitGpuEvent_ = CreateEvent(nullptr, false, false, "WAIT_GPU");
    if (!waitGpuEvent_) {
        assert(false && "GPU　同期用のイベントハンドルの作成に失敗しました");
        return false;
    }

    return true;
}

void fence::wait(UINT64 fenceValue) const noexcept 
{
    if (!fence_) {
        assert(false && "フェンスが未作成です");
        return;
    }

    if (fence_->GetCompletedValue() < fenceValue) {
        fence_->SetEventOnCompletion(fenceValue, waitGpuEvent_);
        WaitForSingleObject(waitGpuEvent_, INFINITE);
    }
}

[[nodiscard]] ID3D12Fence* fence::get() const noexcept 
{
    if (!fence_) {
        assert(false && "フェンスが未作成です");
    }
    return fence_.Get();
}