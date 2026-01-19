#pragma once

#include "device.h"
#include "descriptor_heap.h"

//---------------------------------------------------------------------------------
/**
 * @brief	コンスタントバッファクラス
 */
class ConstantBuffer final {
public:
    //---------------------------------------------------------------------------------
    /**
     * @brief    コンストラクタ
     */
    ConstantBuffer() = default;

    //---------------------------------------------------------------------------------
    /**
     * @brief    デストラクタ
     */
    ~ConstantBuffer();

    //---------------------------------------------------------------------------------
    /**
     * @brief	コンスタントバッファの作成
     * @param	bufferSize		コンスタントバッファのサイズ
     * @return	生成の成否
     */
    [[nodiscard]] bool create(UINT bufferSize) noexcept;

    //---------------------------------------------------------------------------------
    /**
     * @brief	コンスタントバッファを取得する
     * @return	コンスタントバッファのポインタ
     */
    [[nodiscard]] ID3D12Resource* constantBuffer() const noexcept;

    //---------------------------------------------------------------------------------
    /**
     * @brief	GPU 用ディスクリプタハンドルを取得する
     * @return	GPU 用ディスクリプタハンドル
     */
    [[nodiscard]] D3D12_GPU_DESCRIPTOR_HANDLE getGpuDescriptorHandle() const noexcept;

private:
    Microsoft::WRL::ComPtr<ID3D12Resource> constantBuffer_{};   /// コンスタントバッファ
    UINT                                   descriptorIndex_{};  /// ディスクリプタインデックス
    D3D12_GPU_DESCRIPTOR_HANDLE            gpuHandle_{};        /// GPU 用ディスクリプタハンドル
};
