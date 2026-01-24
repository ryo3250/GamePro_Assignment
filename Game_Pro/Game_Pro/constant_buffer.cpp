#include "constant_buffer.h"
#include <cassert>

namespace {
constexpr auto heapType_ = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
}

//---------------------------------------------------------------------------------
/**
 * @brief    デストラクタ
 */
ConstantBuffer::~ConstantBuffer() {
    // ディスクリプタヒープの解放も必要
    descriptor_heap_container::instance().releaseDescriptor(heapType_, descriptorIndex_);
}

//---------------------------------------------------------------------------------
/**
 * @brief	コンスタントバッファの作成
 * @param	bufferSize		コンスタントバッファのサイズ
 * @return	生成の成否
 */
[[nodiscard]] bool ConstantBuffer::create(UINT bufferSize) noexcept {
    // アライメント済みサイズの計算
    const auto size = (bufferSize + 255) & ~255;

    // バッファリソースの作成
    D3D12_HEAP_PROPERTIES heapProps{};
    heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
    D3D12_RESOURCE_DESC resourceDesc{};
    resourceDesc.Dimension        = D3D12_RESOURCE_DIMENSION_BUFFER;
    resourceDesc.Width            = size;
    resourceDesc.Height           = 1;
    resourceDesc.DepthOrArraySize = 1;
    resourceDesc.MipLevels        = 1;
    resourceDesc.Format           = DXGI_FORMAT_UNKNOWN;
    resourceDesc.SampleDesc.Count = 1;
    resourceDesc.Layout           = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    const auto res = device::instance().get()->CreateCommittedResource(
        &heapProps,
        D3D12_HEAP_FLAG_NONE,
        &resourceDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&constantBuffer_));
    if (FAILED(res)) {
        assert(false && "コンスタントバッファの作成に失敗しました");
        return false;
    }

    // ビューの作成
    const auto descriptorIndex = descriptor_heap_container::instance().allocateDescriptor(heapType_);
    if (!descriptorIndex.has_value()) {
        assert(false && "コンスタントバッファのディスクリプタ確保に失敗しました");
        return false;
    }

    // ディスクリプタインデックスを保存
    descriptorIndex_ = descriptorIndex.value();
    // ヒープ取得
    auto heap = descriptor_heap_container::instance().get(heapType_);

    // コンスタントバッファビューの設定
    D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
    cbvDesc.BufferLocation = constantBuffer_->GetGPUVirtualAddress();
    cbvDesc.SizeInBytes    = size;

    // ディスクリプタのサイズを取得
    UINT cbvDescriptorSize = device::instance().get()->GetDescriptorHandleIncrementSize(heapType_);

    // ディスクリプタヒープの開始ハンドルを取得
    D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = heap->GetCPUDescriptorHandleForHeapStart();
    // 指定されたインデックス分ハンドルを進める
    cpuHandle.ptr += descriptorIndex_ * cbvDescriptorSize;

    // コンスタントバッファビューとハンドルを関連付ける
    device::instance().get()->CreateConstantBufferView(&cbvDesc, cpuHandle);

    // GPU 用ディスクリプタハンドルを保存
    gpuHandle_ = heap->GetGPUDescriptorHandleForHeapStart();
    // 指定されたインデックス分ハンドルを進める
    gpuHandle_.ptr += descriptorIndex_ * cbvDescriptorSize;

    return true;
}

//---------------------------------------------------------------------------------
/**
 * @brief	コンスタントバッファを取得する
 * @return	コンスタントバッファのポインタ
 */
[[nodiscard]] ID3D12Resource* ConstantBuffer::constantBuffer() const noexcept {
    assert(constantBuffer_ && "コンスタントバッファが未作成です");
    return constantBuffer_.Get();
}

//---------------------------------------------------------------------------------
/**
 * @brief	GPU 用ディスクリプタハンドルを取得する
 * @return	GPU 用ディスクリプタハンドル
 */
[[nodiscard]] D3D12_GPU_DESCRIPTOR_HANDLE ConstantBuffer::getGpuDescriptorHandle() const noexcept {
    assert(constantBuffer_ && "コンスタントバッファが未作成です");
    return gpuHandle_;
}