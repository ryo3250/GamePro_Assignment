#pragma once
#include "device.h"
#include "command_list.h"
#include <DirectXMath.h>

class shape {
public:
    struct ConstBufferData {
        DirectX::XMMATRIX world_{};
        DirectX::XMFLOAT4 color_{};
    };

public:

    shape() = default;

    virtual ~shape() = default;

    [[nodiscard]] bool create() noexcept;

    void draw(const command_list& commandList) noexcept;

protected:

    [[nodiscard]] virtual bool createVertexBuffer() noexcept = 0;

    [[nodiscard]] virtual bool createIndexBuffer() noexcept = 0;

protected:
    Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuffer_{};
    Microsoft::WRL::ComPtr<ID3D12Resource> indexBuffer_{};
    D3D12_VERTEX_BUFFER_VIEW               vertexBufferView_{};
    D3D12_INDEX_BUFFER_VIEW                indexBufferView_{};
    D3D_PRIMITIVE_TOPOLOGY                 topology_{};
    UINT                                   indexCount_{};
};
