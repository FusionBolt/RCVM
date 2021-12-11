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
    protected:
        VMInst() = default;
    };

    struct Add : VMInst {
        Add()
        {
            type = InstType::Add;
        }
    };
    struct Sub : VMInst {
        Sub()
        {
            type = InstType::Sub;
        }
    };
    struct Mul : VMInst {
        Mul()
        {
            type = InstType::Mul;
        }
    };
    struct Div : VMInst {
        Div()
        {
            type = InstType::Div;
        }
    };
    struct Label : VMInst {
        Label()
        {
            type = InstType::Label;
        }
    };
    struct DirectJump : VMInst {
        DirectJump()
        {
            type = InstType::DirectJump;
        }
    };
    struct CondJump : VMInst {
        CondJump()
        {
            type = InstType::CondJump;
        }
    };
    struct Push : VMInst {
        Push()
        {
            type = InstType::Push;
        }
    };
    struct Pop : VMInst {
        Pop()
        {
            type = InstType::Pop;
        }
    };
    struct Call : VMInst {
        Call()
        {
            type = InstType::Call;
        }
    };
    struct Return : VMInst {
        Return()
        {
            type = InstType::Return;
        }
    };
}
