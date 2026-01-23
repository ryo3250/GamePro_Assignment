#include "camera.h"
#include <cmath>

#include "input.h"

namespace 
{
constexpr float eyeMoveSpeed_ = 0.06f;
constexpr float destTargetToView_ = -5.0f;

struct  const_buffer_data {
    DirectX::XMMATRIX view_{};
    DirectX::XMMATRIX projection_{};
};

}

namespace game {
void camera::initialize() noexcept {
    createDrawBuffer();

    position_ = DirectX::XMFLOAT3(0.0f, 1.5f, destTargetToView_);

    target_ = DirectX::XMFLOAT3(0.0f, 0.0f, 10.0f);

    up_ = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);

    projection_ = DirectX::XMMatrixPerspectiveFovLH(
        DirectX::XM_PIDIV4,  // 視野角45度
        1280.0f / 720.0f,    // アスペクト比
        0.1f,                // ニアクリップ
        100.0f               // ファークリップ
    );
}

void camera::update() noexcept {
    // ビュー行列の計算
    view_ = DirectX::XMMatrixLookAtLH(
        DirectX::XMLoadFloat3(&position_),
        DirectX::XMLoadFloat3(&target_),
        DirectX::XMLoadFloat3(&up_));
}

void camera::createDrawBuffer() noexcept {
    if (!constantBuffer_.create(sizeof(const_buffer_data))) {
        assert(false && "カメラ用コンスタントバッファの作成に失敗しました");
    }
}

void camera::updateDrawBuffer() noexcept 
{
    object::updateConstantBuffer(const_buffer_data{DirectX::XMMatrixTranspose(view_), DirectX::XMMatrixTranspose(projection_)});

}

[[nodiscard]] DirectX::XMMATRIX XM_CALLCONV camera::viewMatrix() const noexcept 
{
    return view_;
}

[[nodiscard]] DirectX::XMMATRIX XM_CALLCONV camera::projection() const noexcept 
{
    return projection_;
}
}  // namespace game
