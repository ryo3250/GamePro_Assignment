#pragma once
#include "game_object.h"
namespace game 
{

class player : public game_object{
public:

	player() = default;

	virtual ~player() = default;

public:

	virtual void initialize() noexcept override;

	virtual void update() noexcept override;
};
}

