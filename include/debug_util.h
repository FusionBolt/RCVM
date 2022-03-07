#pragma once

#include <fstream>
#include <sstream>

#include "instructions.hpp"
#include "logger.h"

namespace RCVM {
    void dump_inst(const std::vector<std::shared_ptr<VMInst>> &inst_list, const std::string &path);

#define DEBUG_CHECK(cond, msg) \
    if(cond) \
    { throw std::runtime_error(msg); }

    inline void print(const std::vector<long> &vars)
    {
        std::stringstream s;
        for (int i = 0; i < vars.size(); ++i)
        {
            s << vars[i] << " ";
        }
        STACK_LOG(s.str());
    }

    inline void print_all_frame_data(const std::vector<std::vector<long>> &data)
    {
        STACK_LOG("Print Stack Frame:");
        for (auto &frame_data : data)
        {
            print(frame_data);
        }
        STACK_LOG("End Frame");
    }
}