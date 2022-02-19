#include "vm_parser.h"

RCVM::SymbolTable<RCVM::ClassInfo> RCVM::VMParser::parse() {
    std::ifstream f(_path);
    if(!f.good())
    {
        throw std::runtime_error("no input file:" + _path);
    }
    std::string str;
    SymbolTable<ClassInfo> class_table;
    while (std::getline(f, str)) {
        // 1. name and parents
        auto klass = split(str);
        auto class_name = klass[0];
        auto parent = klass.size() == 1 ? "" : klass[1];
        // 2. member vars
        std::getline(f, str);
        auto member_vars = split(str);
        // 3. functions
        std::getline(f, str);
        SymbolTable<FunInfo> fun_table;
        while(!str.empty())
        {
            // 3.1 info
            auto fun_info = split(str);
            auto name = fun_info[0];
            auto args = std::stoi(fun_info[1]);
            auto local_vars = std::stoi(fun_info[2]);
            // 3.2 add to class_table
            fun_table.define(name, FunInfo(args, local_vars));
            // 3.3 define
            std::getline(f, str);
            auto &inst_list = fun_table[name].inst_list;
            while(std::getline(f, str) && !str.empty())
            {
                auto list = split(str);
                inst_list.push_back(get_inst(list));
            }
            std::getline(f, str);
        }
        ClassInfo class_info(member_vars, fun_table, parent);
        class_table.define(class_name, class_info);
    }
    f.close();
    return class_table;
}
