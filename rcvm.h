#pragma once
#include "instructions.hpp"
#include "eval_stack.h"
#include "symbol_table.hpp"

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

        [[nodiscard]] const SymbolTable<FunInfo>& sym_table() const
        {
            return _sym_table;
        }

    private:
        friend class VMInstVisitor;
        std::unique_ptr<VMInstVisitor> _visitor;
        std::vector<std::unique_ptr<VMInst>> _inst_list;
        SymbolTable<FunInfo> _sym_table;
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
                    visit(static_cast<const Push&>(inst));
                case InstType::Pop:
                    visit(static_cast<const Pop&>(inst));break;
                case InstType::Call:
                    visit(static_cast<const Call&>(inst));break;
                case InstType::DefineFun:
                    visit(static_cast<const DefineFun&>(inst));break;
                case InstType::Return:
                    visit(static_cast<const Return&>(inst));break;
            }
        }

        void visit(const Add &inst) {
            _eval_stack.exec([](auto &a, auto& b) { return a + b; });
        }

        void visit(const Sub &inst) {
            _eval_stack.exec([](auto &a, auto& b) { return a - b; });
        }

        void visit(const Mul &inst) {
            _eval_stack.exec([](auto &a, auto& b) { return a * b; });
        }

        void visit(const Div &inst) {
            _eval_stack.exec([](auto &a, auto& b) { return a / b; });
        }

        void visit(const Label &inst) {}

        void visit(const DirectJump &inst) {}

        void visit(const CondJump &inst) {}

        void visit(const Push &inst) {
            _eval_stack.push(0);
        }

        void visit(const Pop &inst) {
            // todo:pop to stack top
            _eval_stack.pop(0);
        }

        void visit(const Call &inst) {
            _vm._status.call_stack += 1;
        }

        void visit(const DefineFun &inst) {
            auto entry = _vm._status.pc + 1;
            while(_vm._inst_list[entry]->type != InstType::FunEnd)
            {
                ++entry;
            }
            // pc increment per inst
            _vm._status.pc = entry - 1;
        }

        void visit(const Return &inst) {
            _vm._status.call_stack -= 1;
        }

    private:
        VM &_vm;
        EvalStack &_eval_stack;
    };

    void VM::run(std::vector<std::unique_ptr<VMInst>> &&inst_list) {
        _inst_list = std::move(inst_list);
        _visitor = std::make_unique<VMInstVisitor>(*this);
        while(_status.pc < _inst_list.size())
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