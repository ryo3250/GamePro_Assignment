#include "enemy.h"
#include "shape_container.h"
#include "triangle_polygon.h"

namespace game 
{
void enemy::initialize() noexcept 
{
    game_object::initialize();

    auto triId = shape_container::instance().create<triangle_polygon>();
    set({0.0f, 0.0f, 30.0f}, {0.0f, 0.0f, 0.0f}, {10.0f, 10.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1}, triId);
}

void enemy::update() noexcept {
}


void enemy::onHit() noexcept 
{
    color_.y *= 0.95f;
    color_.z *= 0.95f;
}
}
