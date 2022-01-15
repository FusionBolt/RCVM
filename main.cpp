#include "inst_parser.h"
#include "rcvm.h"
#include "instructions.hpp"

int main()
{
    RCVM::InstParser parser("inst.rcvi", "fun.rcsym");
    auto inst_list = parser.parse_inst();
    auto sym_table = parser.parse_symbol();
    RCVM::VM vm;
    vm.run(std::move(inst_list), std::move(sym_table));
}