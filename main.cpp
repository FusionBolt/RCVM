#include "vm_parser.h"
#include "rcvm.h"
#include "instructions.hpp"

int main()
{
    RCVM::VMParser parser("class_table.rckls");
    auto class_table = parser.parse();
    RCVM::VM vm;
    vm.run(std::move(class_table));
}