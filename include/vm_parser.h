#pragma once

#include "instructions.hpp"
#include "symbol_table.h"

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


    inline std::vector<std::string> split(const std::string &s, char delim = ' ') {
        std::vector<std::string> elems;
        split(s, delim, std::back_inserter(elems));
        return elems;
    }

    class VMParser {
    public:
        explicit VMParser(std::string path)
                : _path(std::move(path)) {}

        SymbolTable<ClassInfo> parse();

    private:
        std::string _path;
    };
}