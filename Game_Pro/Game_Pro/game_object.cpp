#include "game_object.h"
#include "shape_container.h"
#include <cmath>

namespace 
{
struct ConstBufferData {
    DirectX::XMMATRIX world_{};
    DirectX::XMFLOAT4 color_{};
};
}

namespace game {
void game_object::initialize() noexcept {
    createDrawBuffer();
}

void game_object::setDrawCommand(const command_list& commandList, UINT slot) noexcept {
    object::setDrawCommand(commandList, slot);
    shape_container::instance().draw(commandList, shapeId_);
}

void game_object::createDrawBuffer() noexcept {
    if (!constantBuffer_.create(sizeof(ConstBufferData))) {
        assert(false && "GameObject コンスタントバッファの作成に失敗しました");
    }
}

void game_object::updateDrawBuffer() noexcept {
    object::updateConstantBuffer(ConstBufferData{DirectX::XMMatrixTranspose(world_), color_});
}

void game_object::setHandle(UINT64 handle) noexcept {
    handle_ = handle;
}

[[nodiscard]] UINT64 game_object::handle() const noexcept {
    return handle_;
}

void game_object::set(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, DirectX::XMFLOAT3 scale, DirectX::XMFLOAT4 color, UINT64 shapeId) noexcept {
    // ワールド行列の計算
    DirectX::XMMATRIX matScale    = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
    DirectX::XMMATRIX rotation    = DirectX::XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z);
    DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
    world_                        = matScale * rotation * translation;

    color_ = color;

    shapeId_ = shapeId;

    radius_ = (scale.x + scale.y + scale.z) / (3.0f * 2.0f);
}

[[nodiscard]] DirectX::XMMATRIX game_object::world() const noexcept 
{
    return world_;
}

[[nodiscard]] DirectX::XMFLOAT4 game_object::color() const noexcept 
{
    return color_;
}
}  // namespace game