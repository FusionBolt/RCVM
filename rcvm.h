#pragma once
#include "instruction.h"

namespace RCVM
{
    class VM
    {
    public:
        VM() = default;

        void run(const std::vector<std::unique_ptr<VMInst>>& inst_list)
        {
            InstVisitor visitor;
            for(const auto& inst : inst_list)
            {
                visitor.visit(*inst);
            }
        }

    private:

    };
}