#include "bullet.h"
#include "enemy.h"

#include "shape_container.h"
#include "quad_polygon.h"

#include "game_object_manager.h"

namespace game 
{
void bullet::initialize() noexcept {
    game_object::initialize();

    auto quadId = shape_container::instance().create<quad_polygon>();

    DirectX::XMFLOAT3 parentPos{};
    if (auto parent = GameObjectManager::instance().gameObject(parent_)) {
        DirectX::XMStoreFloat3(&parentPos, parent.value()->world().r[3]);
    }
    set(parentPos, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 0.3f}, quadId);
}

void bullet::update() noexcept 
{
    game_object::update();

    constexpr float moveSpeed = 0.3f;

    DirectX::XMFLOAT3 pos{};
    pos.z += moveSpeed;

    world_ = DirectX::XMMatrixMultiply(world_, DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z));

    GameObjectManager::instance().registerHit(handle());
}


void bullet::onHit() noexcept 
{
    GameObjectManager::instance().registerDelete(handle());
}

[[nodiscard]] UINT64 bullet::hitTargetTypeId() noexcept 
{
    return id::get<enemy>();
}

}
