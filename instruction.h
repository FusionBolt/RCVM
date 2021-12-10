#pragma once

#include <memory>
#include <iostream>

namespace RCVM {
    enum class InstType {
        Add,
        Sub,
        Mul,
        Div,
        Label,
        DirectJump,
        CondJump,
        Push,
        Pop,
        Call,
        Return
    };

    struct VMInst {
        InstType type;
        VMInst() = default;
    };

    struct Add : VMInst {
        Add() = default;
    };
    struct Sub : VMInst {
        Sub() = default;
    };
    struct Mul : VMInst {
        Mul() = default;
    };
    struct Div : VMInst {
        Div() = default;
    };
    struct Label : VMInst {
        Label() = default;
    };
    struct DirectJump : VMInst {
        DirectJump() = default;
    };
    struct CondJump : VMInst {
        CondJump() = default;
    };
    struct Push : VMInst {
        Push() = default;
    };
    struct Pop : VMInst {
        Pop() = default;
    };
    struct Call : VMInst {
        Call() = default;
    };
    struct Return : VMInst {
        Return() = default;
    };
}
