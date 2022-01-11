#pragma once
#include <string>
#include <map>

struct FunInfo
{
    FunInfo(): FunInfo(0, 0, 0) {}
    FunInfo(int argc, int locals, int begin = 0): _argc(argc), _locals(locals), _begin(begin) {}

    FunInfo(const FunInfo& other) = default;
    FunInfo(FunInfo&& other) = default;
    FunInfo& operator=(const FunInfo& other) = default;
    FunInfo& operator=(FunInfo&& other) = default;

    int _begin;
    int _argc;
    int _locals;
};

template<typename T>
class SymbolTable
{
public:
    SymbolTable() = default;

    void define(std::string sym, T value)
    {
        _table[sym] = value;
    }
private:
    std::map<std::string, T> _table;
};