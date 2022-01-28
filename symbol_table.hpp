#pragma once
#include <string>
#include <map>
#include <utility>
#include "compiler_defs.h"

namespace RCVM {
    struct FunInfo {
        FunInfo() : FunInfo(0, 0, UndefinedAddr) {}

        FunInfo(size_t _argc, size_t _locals, size_t _begin = UndefinedAddr) : argc(_argc), locals(_locals),
                                                                               begin(_begin) {}

        FunInfo(const FunInfo &other) = default;

        FunInfo(FunInfo &&other) = default;

        FunInfo &operator=(const FunInfo &other) = default;

        FunInfo &operator=(FunInfo &&other) = default;

        [[nodiscard]] size_t local_count() const { return locals - argc; }

        size_t argc;
        size_t locals;
        size_t begin;
        std::vector<std::shared_ptr<VMInst>> inst_list;
    };

    template<typename T>
    class SymbolTable {
    public:
        SymbolTable() = default;

        SymbolTable(const SymbolTable &other) = default;

        SymbolTable(SymbolTable &&other) = default;

        SymbolTable &operator=(const SymbolTable &other) = default;

        SymbolTable &operator=(SymbolTable &&other) = default;

        void define(const std::string &sym, T value) {
            _table[sym] = value;
        }

        T &operator[](const std::string &sym) {
            return _table[sym];
        }

        bool contains(const std::string &sym) {
            return _table.contains(sym);
        }

    private:
        std::map<std::string, T> _table;
    };

    struct ClassInfo
    {
    public:
        ClassInfo() = default;

        ClassInfo(const std::vector<std::string>& vars, const SymbolTable<FunInfo>& methods)
            : _vars(vars), _methods(methods) {}

        ClassInfo(const ClassInfo &other) = default;

        ClassInfo(ClassInfo &&other) = default;

        ClassInfo &operator=(const ClassInfo &other) = default;

        ClassInfo &operator=(ClassInfo &&other) = default;

        std::vector<std::string> _vars;
        SymbolTable<FunInfo> _methods;
    };
}