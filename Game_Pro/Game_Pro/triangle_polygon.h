#pragma once

#include "shape.h"

class triangle_polygon final : public shape {
public:

	triangle_polygon() = default;

	~triangle_polygon() = default;

private:

	[[nodiscard]] virtual bool createVertexBuffer() noexcept override;

	[[nodiscard]] virtual bool createIndexBuffer() noexcept override;
};
