#include "pc.h"

#include "debug_util.h"

void RCVM::PC::absolute_jump(size_t new_pc)
{
    _inst_addr = new_pc;
    _need_increase = false;
}

void RCVM::PC::relative_jump(int offset)
{
    DEBUG_CHECK(static_cast<int>(_inst_addr) + offset < 0,
                "invalid pc, pc:" + std::to_string(_inst_addr)
                    + "offset:" + std::to_string(offset))
    absolute_jump(_inst_addr + offset);
}

void RCVM::PC::increase()
{
    if (_need_increase)
    {
        ++_inst_addr;
    }
    else
    {
        _need_increase = true;
    }
}

size_t RCVM::PC::current() const
{
    return _inst_addr;
}
void RCVM::PC::force_need_incr()
{
    _need_increase = true;
}
