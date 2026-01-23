#pragma once
#include "game_object.h"

namespace game 
{
class bullet : public game_object {
public:

	bullet() = default;

	using game_object::game_object;

	virtual ~bullet() = default;

public:

	virtual void initialize() noexcept override;

	virtual void update() noexcept override;

public:

	virtual void onHit() noexcept override;

	[[nodiscard]] virtual UINT64 hitTargetTypeId() noexcept override;
};
}


