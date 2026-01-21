#include "swap_chain.h"
#include "DXGI.h"
#include "window.h"

#include<cassert>

[[nodiscard]] bool swap_chain::create(const command_queue& commandQueue) noexcept 
{
    const auto [w, h] = window::instance().size();

    swapChainDesc_ = {};
    swapChainDesc_.BufferCount = 2;
    swapChainDesc_.Width       = w;
    swapChainDesc_.Height      = h;
    swapChainDesc_.Format      = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc_.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc_.SwapEffect  = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc_.SampleDesc.Count = 1;

    Microsoft::WRL::ComPtr<IDXGISwapChain1> tempSwapChain{};
    {
        const auto hr = device::instance().dxgi().factory()->CreateSwapChainForHwnd(
            commandQueue.get(), window::instance().handle(),
            &swapChainDesc_, nullptr, nullptr, tempSwapChain.GetAddressOf());
        if (FAILED(hr)) {
            assert(false && "スワップチェインの作成に失敗");
            return false;
        }
    }

    {
        const auto hr = tempSwapChain->QueryInterface(IID_PPV_ARGS(&swapChain_));
        if (FAILED(hr)) {
            assert(false && "スワップチェインのアップグレードに失敗");
            return false;
        }
    }

    return true;
}

[[nodiscard]] IDXGISwapChain3* swap_chain::get() const noexcept 
{
    if (!swapChain_) {
        assert(false && "スワップチェインが未生成です");
    }
    return swapChain_.Get();
}

[[nodiscard]] const DXGI_SWAP_CHAIN_DESC1& swap_chain::getDesc() const noexcept 
{
    if (!swapChain_) {
        assert(false && "スワップチェインが未生成です");
    }
    return swapChainDesc_;
}