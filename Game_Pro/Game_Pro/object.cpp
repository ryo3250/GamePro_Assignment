#include "object.h"

namespace game 
{
void object::setDrawCommand(const command_list& commandList, UINT slot) noexcept 
{
    commandList.get()->SetGraphicsRootDescriptorTable(
        slot,
        constantBuffer_.getGpuDescriptorHandle());
}
}
