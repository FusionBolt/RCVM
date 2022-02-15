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
    template<typename Out>
    void split(const std::string &s, char delim, Out result) {
        std::stringstream ss;
        ss.str(s);
        std::string item;
        while (std::getline(ss, item, delim)) {
            *(result++) = item;
        }
    }


    std::vector<std::string> split(const std::string &s, char delim = ' ') {
        std::vector<std::string> elems;
        split(s, delim, std::back_inserter(elems));
        return elems;
    }

    class VMParser {
    public:
        explicit VMParser(std::string path)
                : _path(std::move(path)) {}

        SymbolTable<ClassInfo> parse() {
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
                std::vector<std::string> parents;
                for (int i = 1; i < klass.size(); ++i) {
                    parents.push_back(klass[i]);
                }
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
                ClassInfo class_info(member_vars, fun_table, parents);
                class_table.define(class_name, class_info);
            }
            f.close();
            return class_table;
        }

    private:

        std::string _path;
    };
}