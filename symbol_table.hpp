#pragma once
#include <string>
#include <map>

struct FunInfo
{
    FunInfo(): FunInfo(0, 0, 0) {}
    FunInfo(size_t _argc, size_t _locals, size_t _begin): argc(_argc), locals(_locals), begin(_begin) {}

    FunInfo(const FunInfo& other) = default;
    FunInfo(FunInfo&& other) = default;
    FunInfo& operator=(const FunInfo& other) = default;
    FunInfo& operator=(FunInfo&& other) = default;

    [[nodiscard]] size_t local_count() const { return locals - argc; }
    size_t argc;
    size_t locals;
    size_t begin;
};

template<typename T>
class SymbolTable
{
public:
    SymbolTable() = default;

    void define(const std::string& sym, T value)
    {
        _table[sym] = value;
    }

    T& operator[] (const std::string& sym)
    {
        return _table[sym];
    }

    bool contains(const std::string& sym)
    {
        return _table.contains(sym);
    }

private:
    std::map<std::string, T> _table;
};