#pragma once

#include "game_object.h"

namespace game 
{
class enemy : public game_object{
public:

	enemy() = default;

	virtual ~enemy() = default;

public:

	virtual void initialize() noexcept override;

	virtual void update() noexcept override;

public:

	virtual void onHit() noexcept override;
};
}

