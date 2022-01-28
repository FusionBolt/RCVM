#pragma once

#include "instructions.hpp"
#include "symbol_table.hpp"

#include <string>
#include <utility>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <iostream>
#include <memory>

namespace RCVM {
    std::vector<std::string> split(std::string_view str, char c = ' ') {
        size_t index = 0;
        std::vector<std::string> str_list;
        for (size_t i = 0; i < str.size(); ++i) {
            if (str[i] == c) {
                auto slice_str = str.substr(index, i);
                str_list.emplace_back(slice_str);
                while (str[i + 1] == c) {
                    ++i;
                }
                index = i + 1;
            }
        }
        if (index != str.size()) {
            auto slice_str = str.substr(index, str.size());
            str_list.emplace_back(slice_str);
        }
        return str_list;
    }

    class VMParser {
    public:
        explicit VMParser(std::string path)
                : _path(std::move(path)) {}

        SymbolTable<ClassInfo> parse() {
            std::ifstream f(_path);
            std::string str;
            SymbolTable<ClassInfo> class_table;
            while (std::getline(f, str)) {
                // 1. class name
                auto class_name = str;
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
                ClassInfo class_info(member_vars, fun_table);
                class_table.define(class_name, class_info);
            }
            return class_table;
        }

    private:

        std::string _path;
    };
}