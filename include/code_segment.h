#pragma once

#include "instructions.hpp"
#include "symbol_table.h"

namespace RCVM
{
class CodeSegment
{
public:
    size_t load_method(const std::string &klass, const std::string &name,
                       const FunInfo &f);

    std::vector<std::shared_ptr<VMInst>> inst_list() const;

    size_t size() const;

    std::shared_ptr<VMInst> get_inst(size_t i) const;

    std::shared_ptr<VMInst> operator[] (int i) const;
private:
    std::vector<std::shared_ptr<VMInst>> _inst_list;
};
}
