#include "shape_container.h"

void shape_container::draw(const command_list& commandList, UINT64 id) noexcept 
{
    auto it = shapes_.find(id);
    if (it == shapes_.end()) {
        return;
    }

    it->second->draw(commandList);
}