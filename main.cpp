#include "vm_parser.h"
#include "rcvm.h"
#include "instructions.hpp"

int main()
{
    RCVM::VMParser parser("../class_table.rckls");
    auto class_table = parser.parse();
    RCVM::init_class_symbol_table(std::move(class_table));
    RCVM::VM vm;
    vm.run();
}