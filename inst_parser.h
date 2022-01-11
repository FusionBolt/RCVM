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
    class InstParser {
    public:
        explicit InstParser(std::string inst_path = "inst.rcvi", std::string symbol_path = "func.rcsym")
        : _inst_path(std::move(inst_path)), _symbol_path(std::move(symbol_path)) {}

        std::vector<std::unique_ptr<VMInst>> parse_inst() {
            std::vector<std::unique_ptr<VMInst>> inst_list;
            std::ifstream f(_inst_path);
            std::string inst;
            while (std::getline(f, inst)) {
                // std::cout << inst << std::endl;
                auto list = split(inst);
                inst_list.push_back(get_inst(list));
//                for (auto &&i : list) {
//                    std::cout << i << std::endl;
//                }
            }
            return inst_list;
        }

        SymbolTable<FunInfo> parse_symbol()
        {
            std::ifstream f(_symbol_path);
            std::string symbol;
            SymbolTable<FunInfo> table;
            while (std::getline(f, symbol)) {
                auto list = split(symbol);
                table.define(list[0], FunInfo(std::stoi(list[1]), std::stoi(list[2])));
            }
            return table;
        }



    private:
        std::string _inst_path;
        std::string _symbol_path;
    };
}