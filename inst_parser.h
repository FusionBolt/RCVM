#pragma once

#include "instruction.h"
#include <string>
#include <utility>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <memory>

namespace RCVM {
    class InstParser {
    public:
        explicit InstParser(std::string inst_path) : _inst_path(std::move(inst_path)) {}

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

        std::vector<std::unique_ptr<VMInst>> parse() {
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

    private:
        std::string _inst_path;

        std::unique_ptr<VMInst> get_inst(const std::vector<std::string> &list) {
            new Add();
            if (list[0] == "Add") {
                return std::make_unique<Add>();
            }
            if (list[0] == "Sub") {
                return std::make_unique<Sub>();
            }
            if (list[0] == "Mul") {
                return std::make_unique<Mul>();
            }
            if (list[0] == "Div") {
                return std::make_unique<Div>();
            }
            if (list[0] == "Label") {
                return std::make_unique<Label>();
            }
            if (list[0] == "DirectJump") {
                return std::make_unique<DirectJump>();
            }
            if (list[0] == "CondJump") {
                return std::make_unique<CondJump>();
            }
            if (list[0] == "Push") {
                return std::make_unique<Push>();
            }
            if (list[0] == "Pop") {
                return std::make_unique<Pop>();
            }
            if (list[0] == "Call") {
                return std::make_unique<Call>();
            }
            if (list[0] == "Return") {
                return std::make_unique<Return>();
            }
            throw std::runtime_error("Not supported instruction:" + list[0]);
        }
    };
}