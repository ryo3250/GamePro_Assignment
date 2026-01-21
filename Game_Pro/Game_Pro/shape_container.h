#pragma once

#include "shape.h"
#include <unordered_map>
#include <memory>

class shape_container final {
public:
    static shape_container& instance() noexcept {
        static shape_container instance;
        return instance;
    }

public:
    template <class T>
    [[nodiscard]] UINT64 create() noexcept {
        const auto id = id::get<T>();
        if (shapes_.find(id) != shapes.end()) {
            return id;
        }

        auto p = std::make_unique<T>();
        if (!p->create()) {
            assert(false && "Œ`ó‚Ì¶¬‚É¸”s‚µ‚Ü‚µ‚½");
            return 0;
        }
        shapes_.emlace(id, std::move(p));
        return id;
    }

    void draw(const command_list& commandList, UINT64 id) noexcept;

private:

    shape_container() = default;

    ~shape_container() = default;

    shape_container(const shape_container&) = delete;
    shape_container& operator=(const shape_container&) = delete;
    shape_container(shape_container&&)                 = delete;
    shape_container operator=(shape_container&&)       = delete;

protected:
    std::unordered_map<UINT64, std::unique_ptr<shape>> shapes_;
};
