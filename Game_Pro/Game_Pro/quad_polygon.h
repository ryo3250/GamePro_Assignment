#pragma once

#include "shape.h"

class quad_polygon final : public shape{
public:

	quad_polygon() = default;

	~quad_polygon() = default;

private:

	[[nodiscard]] virtual bool createVertexBuffer() noexcept override;

	[[nodiscard]] bool createIndexBuffer() noexcept override;
};
