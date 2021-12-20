#pragma once
#include "instruction.h"
#include "eval_stack.h"

namespace RCVM
{
    class VMStatus
    {
    public:
        VMStatus() = default;
        EvalStack eval_stack;
        size_t pc = 0;
        size_t call_stack = 1;
    };

    class VMInstVisitor;
    class VM
    {
    public:
        VM() = default;

        void run(std::vector<std::unique_ptr<VMInst>>&& inst_list);

        EvalStack& eval_stack()
        {
            return _status.eval_stack;
        }

    private:
        friend class VMInstVisitor;
        std::unique_ptr<VMInstVisitor> _visitor;
        std::vector<std::unique_ptr<VMInst>> _inst_list;
        VMStatus _status;
    };

    class VMInstVisitor {
    public:
        VMInstVisitor(VM &vm) : _vm(vm), _eval_stack(_vm.eval_stack()) {}

        void accept(const VMInst &inst)
        {
            switch(inst.type)
            {
                case InstType::Add:
                    visit(static_cast<const Add&>(inst));break;
                case InstType::Sub:
                    visit(static_cast<const Sub&>(inst));break;
                case InstType::Mul:
                    visit(static_cast<const Mul&>(inst));break;
                case InstType::Div:
                    visit(static_cast<const Div&>(inst));break;
                case InstType::Label:
                    visit(static_cast<const Label&>(inst));break;
                case InstType::DirectJump:
                    visit(static_cast<const DirectJump&>(inst));break;
                case InstType::CondJump:
                    visit(static_cast<const CondJump&>(inst));break;
                case InstType::Push:
                    visit(static_cast<const Push&>(inst));break;
                case InstType::Pop:
                    visit(static_cast<const Pop&>(inst));break;
                case InstType::Call:
                    visit(static_cast<const Call&>(inst));break;
                case InstType::Return:
                    visit(static_cast<const Return&>(inst));break;
            }
        }

        void visit([[maybe_unused]] const Add &inst) {
            _eval_stack.exec([](auto &a, auto& b) { return a + b; });
        }

        void visit([[maybe_unused]] const Sub &inst) {
            _eval_stack.exec([](auto &a, auto& b) { return a - b; });
        }

        void visit([[maybe_unused]] const Mul &inst) {
            _eval_stack.exec([](auto &a, auto& b) { return a * b; });
        }

        void visit([[maybe_unused]] const Div &inst) {
            _eval_stack.exec([](auto &a, auto& b) { return a / b; });
        }

        void visit([[maybe_unused]] const Label &inst) {}

        void visit([[maybe_unused]] const DirectJump &inst) {}

        void visit([[maybe_unused]] const CondJump &inst) {}

        void visit([[maybe_unused]] const Push &inst) {
            _eval_stack.push(0);
        }

        void visit([[maybe_unused]] const Pop &inst) {
            // todo:pop to stack top
            _eval_stack.pop(0);
        }

        void visit([[maybe_unused]] const Call &inst) {
            _vm._status.call_stack += 1;
        }

        void visit([[maybe_unused]] const Return &inst) {
            _vm._status.call_stack -= 1;
        }

    private:
        VM &_vm;
        EvalStack &_eval_stack;
    };

    void VM::run(std::vector<std::unique_ptr<VMInst>> &&inst_list) {
        _inst_list = std::move(inst_list);
        _visitor = std::make_unique<VMInstVisitor>(*this);
        while(true)
        {
            auto &inst = _inst_list[_status.pc];
            _visitor->accept(*inst);
            _status.pc += 1;
            if(_status.call_stack <= 0)
            {
                break;
            }
        }
    }

}