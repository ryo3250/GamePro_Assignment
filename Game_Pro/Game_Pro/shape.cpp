#include "shape.h"
#include <cassert>

[[nodiscard]] bool shape::create() noexcept 
{
    if (!createVertexBuffer()) {
        return false;
    }

    if (!createIndexBuffer()) {
        return false;
    }
    
    return true;
}

void shape::draw(const command_list& commandList) noexcept 
{
    commandList.get()->IASetVertexBuffers(0, 1, &vertexBufferView_);
    commandList.get()->IASetIndexBuffer(&indexBufferView_);
    commandList.get()->IASetPrimitiveTopology(topology_);
    commandList.get()->DrawIndexedInstanced(indexCount_, 1, 0, 0, 0);
}