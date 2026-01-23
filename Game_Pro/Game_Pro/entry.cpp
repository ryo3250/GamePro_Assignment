#include "window.h"
#include "input.h"

#include "device.h"
#include "DXGI.h"
#include "command_allocator.h"
#include "command_list.h"
#include "command_queue.h"
#include "swap_chain.h"
#include "descriptor_heap.h"
#include "render_target.h"
#include "fence.h"
#include "root_signature.h"
#include "shader.h"
#include "pipline_state_object.h"
#include "constant_buffer.h"
#include "depth_buffer.h"

#include "triangle_polygon.h"
#include "quad_polygon.h"
#include "shape_container.h"

#include "object.h"
#include "camera.h"
#include "game_object_manager.h"
#include "player.h"
#include "enemy.h"

#include <memory>
#include <vector>
#include <cassert>

namespace {
constexpr UINT sceneShaderSlot_ = 0;  // シーン共通用シェーダースロット
}  // namespace

class Application final {
public:
    //---------------------------------------------------------------------------------
    /**
     * @brief    コンストラクタ
     */
    Application() = default;

    //---------------------------------------------------------------------------------
    /**
     * @brief    デストラクタ
     */
    ~Application() = default;

    //---------------------------------------------------------------------------------
    /**
     * @brief	アプリケーションの初期化
     * @return	初期化の成否
     */
    [[nodiscard]] bool initialize(HINSTANCE instance) noexcept {
        // ウィンドウの生成
        if (S_OK != window::instance().create(instance, 1280, 720, "MyApp")) {
            assert(false && "ウィンドウの生成に失敗しました");
            return false;
        }

        // デバイスの生成
        if (!device::instance().create()) {
            assert(false && "デバイスの作成に失敗しました");
            return false;
        }

        // コマンドキューの生成
        if (!commandQueueInstance_.create()) {
            assert(false && "コマンドキューの作成に失敗しました");
            return false;
        }

        // スワップチェインの生成
        if (!swapChainInstance_.create(commandQueueInstance_)) {
            assert(false && "スワップチェインの作成に失敗しました");
            return false;
        }

        // ディスクリプタヒープの生成
        if (!descriptor_heap_container::instance().create(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, swapChainInstance_.getDesc().BufferCount)) {
            assert(false && "ディスクリプタヒープの作成に失敗しました");
            return false;
        }

        // 定数バッファ用ディスクリプタヒープの生成
        if (!descriptor_heap_container::instance().create(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 5, true)) {
            assert(false && "定数バッファ用ディスクリプタヒープの作成に失敗しました");
            return false;
        }

        // デプスバッファ用ディスクリプタヒープの作成
        if (!descriptor_heap_container::instance().create(D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1)) {
            assert(false && "デプスバッファ用ディスクリプタヒープの作成に失敗しました");
            return false;
        }

        // レンダーターゲットの生成
        if (!renderTargetInstance_.createBackBuffer(swapChainInstance_)) {
            assert(false && "レンダーターゲットの作成に失敗しました");
            return false;
        }

        // デプスバッファの生成
        if (!depthBufferInstance_.create()) {
            assert(false && "デプスバッファの作成に失敗しました");
            return false;
        }

        // コマンドアロケータの生成
        if (!commandAllocatorInstance_[0].create(D3D12_COMMAND_LIST_TYPE_DIRECT)) {
            assert(false && "コマンドアロケータの作成に失敗しました");
            return false;
        }
        if (!commandAllocatorInstance_[1].create(D3D12_COMMAND_LIST_TYPE_DIRECT)) {
            assert(false && "コマンドアロケータの作成に失敗しました");
            return false;
        }

        // コマンドリストの生成
        if (!commandListInstance_.create(commandAllocatorInstance_[0])) {
            assert(false && "コマンドリストの作成に失敗しました");
            return false;
        }

        // フェンスの生成
        if (!fenceInstance_.create()) {
            assert(false && "フェンスの作成に失敗しました");
            return false;
        }

        // ルートシグネチャの生成
        if (!rootSignatureInstance_.create()) {
            assert(false && "ルートシグネチャの作成に失敗しました");
            return false;
        }
        // シェーダーの生成
        if (!shaderInstance_.create()) {
            assert(false && "シェーダーの作成に失敗しました");
            return false;
        }
        // パイプラインステートオブジェクトの生成
        if (!piplineStateObjectInstance_.create(shaderInstance_, rootSignatureInstance_)) {
            assert(false && "パイプラインステートオブジェクトの作成に失敗しました");
            return false;
        }

        // カメラの作成
        camera_ = std::make_unique<game::camera>();
        camera_->initialize();

        // ゲームオブジェクトの生成
        game::GameObjectManager::instance().createObject<game::player>();
        game::GameObjectManager::instance().createObject<game::enemy>();

        return true;
    }

    //---------------------------------------------------------------------------------
    /**
     * @brief	アプリケーションループ
     */
    void loop() noexcept {
        while (window::instance().messageLoop()) {
            // 更新処理 /////////////////////////////////////////////////////////////////////////

            // カメラの更新
            camera_->update();

            // ゲームオブジェクトの更新
            game::GameObjectManager::instance().update();

            // ゲームオブジェクトの後更新
            game::GameObjectManager::instance().postUpdate();

            // 描画処理 /////////////////////////////////////////////////////////////////////////

            // 現在のバックバッファインデックスを取得
            const auto backBufferIndex = swapChainInstance_.get()->GetCurrentBackBufferIndex();

            // 以前のフレームの GPU の処理が完了しているか確認して待機する
            if (frameFenceValue_[backBufferIndex] != 0) {
                fenceInstance_.wait(frameFenceValue_[backBufferIndex]);
            }

            // ディスクリプタヒープの解放予約分を解放
            descriptor_heap_container::instance().applyPendingFree();

            // コマンドアロケータリセット
            commandAllocatorInstance_[backBufferIndex].reset();
            // コマンドリストリセット
            commandListInstance_.reset(commandAllocatorInstance_[backBufferIndex]);

            // リソースバリアでレンダーターゲットを Present から RenderTarget へ変更
            auto pToRT = resourceBarrier(renderTargetInstance_.get(backBufferIndex), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
            commandListInstance_.get()->ResourceBarrier(1, &pToRT);

            // レンダーターゲットの設定
            D3D12_CPU_DESCRIPTOR_HANDLE handles[] = {renderTargetInstance_.getCpuDescriptorHandle(backBufferIndex)};
            // デプスバッファのハンドルも設定
            D3D12_CPU_DESCRIPTOR_HANDLE depthHandle = depthBufferInstance_.getCpuDescriptorHandle();
            commandListInstance_.get()->OMSetRenderTargets(1, handles, false, &depthHandle);

            // レンダーターゲットのクリア
            const float clearColor[] = {0.2f, 0.2f, 0.2f, 1.0f};  // クリア
            commandListInstance_.get()->ClearRenderTargetView(handles[0], clearColor, 0, nullptr);
            // デプスバッファのクリア
            commandListInstance_.get()->ClearDepthStencilView(depthHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

            // ルートシグネチャの設定
            commandListInstance_.get()->SetGraphicsRootSignature(rootSignatureInstance_.get());

            // ビューポートの設定
            const auto [w, h] = window::instance().size();
            D3D12_VIEWPORT viewport{};
            viewport.TopLeftX = 0.0f;
            viewport.TopLeftY = 0.0f;
            viewport.Width    = static_cast<float>(w);
            viewport.Height   = static_cast<float>(h);
            viewport.MinDepth = 0.0f;
            viewport.MaxDepth = 1.0f;
            commandListInstance_.get()->RSSetViewports(1, &viewport);

            // シザー矩形の設定
            D3D12_RECT scissorRect{};
            scissorRect.left   = 0;
            scissorRect.top    = 0;
            scissorRect.right  = w;
            scissorRect.bottom = h;
            commandListInstance_.get()->RSSetScissorRects(1, &scissorRect);

            // コンスタントバッファ用ディスクリプタヒープの設定
            ID3D12DescriptorHeap* p[] = {descriptor_heap_container::instance().get(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)};
            commandListInstance_.get()->SetDescriptorHeaps(1, p);

            // パイプラインステートの設定
            commandListInstance_.get()->SetPipelineState(piplineStateObjectInstance_.get());

            // カメラのコンスタントバッファへデータ転送
            camera_->updateDrawBuffer();
            camera_->setDrawCommand(commandListInstance_, sceneShaderSlot_);

            // ゲームオブジェクトの描画
            game::GameObjectManager::instance().draw(commandListInstance_);

            //-------------------------------------------------

            // リソースバリアでレンダーターゲットを RenderTarget から Present へ変更
            auto rtToP = resourceBarrier(renderTargetInstance_.get(backBufferIndex), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
            commandListInstance_.get()->ResourceBarrier(1, &rtToP);

            // コマンドリストをクローズ
            commandListInstance_.get()->Close();

            // コマンドキューにコマンドリストを送信
            ID3D12CommandList* ppCommandLists[] = {commandListInstance_.get()};
            commandQueueInstance_.get()->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

            // プレゼント
            swapChainInstance_.get()->Present(1, 0);

            // フェンスにフェンス値を設定
            commandQueueInstance_.get()->Signal(fenceInstance_.get(), nextFenceValue_);
            frameFenceValue_[backBufferIndex] = nextFenceValue_;
            nextFenceValue_++;
        }

        // ループを抜けるとウィンドウを閉じる
        game::GameObjectManager::instance().clear();
    }

    //---------------------------------------------------------------------------------
    /**
     * @brief	リソースにバリアを設定する
     * @param	commandList	コマンドリスト
     * @param	resource	バリアを張るリソース
     * @param	from		変更前のリソースステート
     * @param	to			変更後のリソースステート
     */
    D3D12_RESOURCE_BARRIER resourceBarrier(ID3D12Resource* resource, D3D12_RESOURCE_STATES from, D3D12_RESOURCE_STATES to) noexcept {
        D3D12_RESOURCE_BARRIER barrier{};
        barrier.Type                   = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        barrier.Flags                  = D3D12_RESOURCE_BARRIER_FLAG_NONE;
        barrier.Transition.pResource   = resource;
        barrier.Transition.StateBefore = from;
        barrier.Transition.StateAfter  = to;
        barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

        return barrier;
    }

private:
    command_queue     commandQueueInstance_{};         /// コマンドキューインスタンス
    swap_chain        swapChainInstance_{};            /// スワップチェインインスタンス
    render_target     renderTargetInstance_{};         /// レンダーターゲットインスタンス
    depth_buffer      depthBufferInstance_{};          /// デプスバッファインスタンス
    command_allocator commandAllocatorInstance_[2]{};  /// コマンドアロケータインスタンス
    command_list      commandListInstance_{};          /// コマンドリストインスタンス

    fence  fenceInstance_{};       /// フェンスインスタンス
    UINT64 frameFenceValue_[2]{};  /// 現在のフレームのフェンス値
    UINT64 nextFenceValue_ = 1;    /// 次のフレームのフェンス値

    root_signature      rootSignatureInstance_{};       /// ルートシグネチャインスタンス
    Shader             shaderInstance_{};              /// シェーダーインスタンス
    pipline_state_object piplineStateObjectInstance_{};  /// パイプラインステートオブジェクトインスタンス

    std::unique_ptr<game::camera> camera_{};  /// カメラ
};

//---------------------------------------------------------------------------------
/**
 * @brief	エントリー関数
 */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // アプリケーションクラスのインスタンスを生成
    Application app;

    if (!app.initialize(hInstance)) {
        assert(false && "アプリケーションの初期化に失敗しました");
    }

    // アプリケーションループを開始
    app.loop();

    return 0;
}
