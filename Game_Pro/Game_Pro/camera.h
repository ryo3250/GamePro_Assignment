#pragma once

#include"object.h"
#include <DirectXMath.h>


namespace game
{
class camera final: public object{
public:

	camera() = default;

	virtual ~camera() = default;

public:

	virtual void initialize() noexcept override;

	virtual void update() noexcept override;

public:

	virtual void createDrawBuffer() noexcept override;

	virtual void updateDrawBuffer() noexcept override;

public:

	[[nodiscard]] DirectX::XMMATRIX XM_CALLCONV viewMatrix() const noexcept;

	[[nodiscard]] DirectX::XMMATRIX XM_CALLCONV projection() const noexcept;

private:
    DirectX::XMMATRIX view_{};
    DirectX::XMMATRIX projection_{};

	DirectX::XMFLOAT3 position_{};
    DirectX::XMFLOAT3 target_{};
    DirectX::XMFLOAT3 up_{};
};

}


