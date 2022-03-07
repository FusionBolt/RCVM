#include "code_segment.h"
#include "logger.h"

size_t RCVM::CodeSegment::load_method(const string &klass, const string &name,
                                       const RCVM::FunInfo &f)
{
    EXEC_LOG("Load Function:" + name + " in Class:" + klass);
    // 1. get start
    auto start = static_cast<int>(_inst_list.size());
    // todo:need reset inst list in f? it can be replaced with unique_ptr?
    // 2. load inst to inst_list
    for(auto &&inst : f.inst_list)
    {
        _inst_list.push_back(inst);
    }
    return start;
}

std::vector<std::shared_ptr<VMInst>> RCVM::CodeSegment::inst_list() const
{
    return _inst_list;
}

size_t RCVM::CodeSegment::size() const
{
    return _inst_list.size();
}

std::shared_ptr<VMInst> RCVM::CodeSegment::get_inst(size_t i) const
{
    return _inst_list[i];
}

std::shared_ptr<VMInst> RCVM::CodeSegment::operator[](int i) const
{
    return get_inst(i);
}