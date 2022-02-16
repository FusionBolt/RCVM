#include "debug_util.h"

void RCVM::dump_inst(const std::vector<std::shared_ptr<VMInst>> &inst_list, const string &path) {
    std::ofstream f(path);
    for (auto &&inst: inst_list) {
        auto str = inst->to_string();
        f.write(str.c_str(), str.size());
        f.write("\n", 1);
    }
    f.close();
}