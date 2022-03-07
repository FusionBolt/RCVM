#pragma once

namespace RCVM
{
    constexpr auto UndefinedAddr = 0;
    constexpr auto InitMethodName = "Init";
    using Pointer = unsigned char*;
    using ConstPointer = unsigned char const*;

#define NOT_IMPL throw std::runtime_error("DirectJump Not Implement");
}