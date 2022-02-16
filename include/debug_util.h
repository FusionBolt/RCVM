#pragma once
#include "instructions.hpp"
#include <fstream>

namespace RCVM {
    void dump_inst(const std::vector<std::shared_ptr<VMInst>> &inst_list, const std::string &path);
}