#include "player.h"
#include "input.h"
#include "shape_container.h"
#include "quad_polygon.h"

#include "game_object_manager.h"
#include "bullet.h"

namespace game 
{
void player::initialize() noexcept 
{
    game_object::initialize();

    auto quadId = shape_container::instance().create<quad_polygon>();
    set({-.2f, 0.0f, 0.1f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {.0f, 1, .0f, 1}, quadId);
}

void player::update() noexcept 
{
    game_object::update();

    constexpr float moveSpeed = 0.05f;

    DirectX::XMFLOAT3 pos{};
    if (input::instance().getKey('W')) {
        pos.z += moveSpeed;
    }
    if (input::instance().getKey('S')) {
        pos.z -= moveSpeed;
    }
    if (input::instance().getKey('A')) {
        pos.x -= moveSpeed;
    }
    if (input::instance().getKey('D')) {
        pos.x += moveSpeed;
    }

    DirectX::XMVECTOR temp = DirectX::XMVectorSet(pos.x, pos.y, pos.z, 0.0f);
    world_.r[3]            = DirectX::XMVectorAdd(world_.r[3], temp);

    if (input::instance().getTrigger('B')) {
        GameObjectManager::instance().createObject<bullet>(handle());
    }
}

}