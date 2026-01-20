#pragma once

#include "device.h"
#include "shader.h"
#include "root_signature.h"

class pipline_state_object final {
public:

	pipline_state_object() = default;

	~pipline_state_object() = default;


	[[nodiscrd]] bool create(const Shader& shader, const root_signature& rootSignature) noexcept;

	[[nodiscrd]] ID3D12PipelineState* get() const noexcept;

private:
        Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState_{};
};
