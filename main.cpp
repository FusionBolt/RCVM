#include "inst_parser.h"
#include "rcvm.h"
int main()
{
    RCVM::InstParser parser("inst.rcvi");
    auto inst_list = parser.parse();
    RCVM::VM vm;
    vm.run(inst_list);
}