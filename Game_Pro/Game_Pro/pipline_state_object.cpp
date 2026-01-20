#include "pipline_state_object.h"
#include <cassert>

[[nodiscard]] bool pipline_state_object::create(const shader& shaer, const root_signature& rootSignature) noexcept {
    D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
    {
            {"POSITION", 0,    DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
            {   "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
    };

    D3D12_DEPTH_STENCIL_DESC depthStateDesc{};
    depthStateDesc.DepthEnable = true;
    depthStateDesc.StencilEnable = false;
    depthStateDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
    depthStateDesc.DepthFunc      = D3D12_COMPARISON_FUNC_LESS;

    D3D12_RENDER_TARGET_BLEND_DESC renderTargetBlendDesc{};
    renderTargetBlendDesc.BlendEnable = true;

    renderTargetBlendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
    renderTargetBlendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
    renderTargetBlendDesc.BlendOp   = D3D12_BLEND_OP_ADD;

    renderTargetBlendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
    renderTargetBlendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;
    renderTargetBlendDesc.BlendOpAlpha   = D3D12_BLEND_OP_ADD;
    
    renderTargetBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

    D3D12_BLEND_DESC blendDesc{};
    for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i) {
        blendDesc.RenderTarget[i] = renderTargetBlendDesc;
    }
}
