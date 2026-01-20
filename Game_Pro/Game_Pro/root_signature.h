#pragma once
#include "device.h"

class root_signature final
{
public:
    root_signature() = default;

    ~root_signature() = default;

    [[nodiscard]] bool create() noexcept;

    [[nodiscard]] ID3D12RootSignature* get() const noexcept;

private:
    Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_{};
};
