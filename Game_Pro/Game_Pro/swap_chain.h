#pragma once

#include "command_queue.h"

class swap_chain final {
public:
    swap_chain() = default;

    ~swap_chain() = default;

    [[nodiscard]] bool create(const command_queue& commandQueue) noexcept;

    [[nodiscard]] IDXGISwapChain3* get() const noexcept;

    [[nodiscard]] const DXGI_SWAP_CHAIN_DESC1& getDesc() const noexcept;

private:
    Microsoft::WRL::ComPtr<IDXGISwapChain3> sawapChain_{};
    DXGI_SWAP_CHAIN_DESC1                   swapChainDesc_{};
};
