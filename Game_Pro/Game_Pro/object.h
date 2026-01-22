#pragma once
#include "constant_buffer.h"
#include "command_list.h"
#include "util.h"

namespace game {
class object {
public:

	object() = default;

	virtual ~object() = default;

	virtual void initialize() noexcept {};

	virtual void update() noexcept {};

	virtual void setDrawCommand(const command_list& commandList, UINT slot) noexcept;

public:

	virtual void createDrawBuffer() noexcept = 0;

	virtual void updateDrawBuffer() noexcept = 0;

public:

	[[nodiscard]] UINT64 typeId() const noexcept {
        return typeId_;
    };


	void setTypeId(UINT64 id) noexcept {
        typeId_ = id;
    };

protected:

	template <class T>
    void updateConstantBuffer(const T& data) noexcept {
        std::byte* dst{};
        constantBuffer_.constantBuffer()->Map(0, nullptr, reinterpret_cast<void**>(&dst));
        memcpy_s(dst, sizeof(T), &data, sizeof(T));
        constantBuffer_.constantBuffer()->Unmap(0, nullptr);
    };
    
protected:
    ConstantBuffer constantBuffer_{};
    UINT64         typeId_{};
};

}

