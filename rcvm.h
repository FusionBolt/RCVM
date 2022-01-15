#pragma once
#include "instructions.hpp"
#include "eval_stack.h"
#include "symbol_table.hpp"

// todo:log option for enable dump pc, stack pointer...
#define LOG_DEBUG(info) std::cout << info << std::endl;

namespace RCVM
{
    using FunctionSymbolTable = SymbolTable<FunInfo>;

    class VMInstVisitor;
    class VM
    {
    public:
        constexpr static auto VMEntryFun = "main";
        VM() = default;

        void run(std::vector<std::unique_ptr<VMInst>>&& inst_list, FunctionSymbolTable&& sym_table);

        EvalStack& eval_stack()
        {
            return _eval_stack;
        }

        [[nodiscard]] const FunctionSymbolTable& sym_table() const
        {
            return _sym_table;
        }

        [[nodiscard]] FunctionSymbolTable& sym_table()
        {
            return _sym_table;
        }

        void begin_call(const std::string& f)
        {
            if(!_sym_table.contains(f))
            {
                // todo: find definition
                throw std::runtime_error("Target Function" + f + "Not Found");
            }
            _pc = _sym_table[f].begin;
            LOG_DEBUG("Call " + f + " PC:" + std::to_string(_pc))
            _eval_stack.begin_call(_pc + 1);
        }

        void end_call()
        {
            _pc = _eval_stack.end_call();
            LOG_DEBUG("Return")
        }

        bool can_stop() const { return _eval_stack.empty(); }
    private:
        void init()
        {
            begin_call(VMEntryFun);
            LOG_DEBUG("Init:Jump To Main")
            LOG_DEBUG("Current PC:" + std::to_string(_pc))
        }

        friend class VMInstVisitor;
        std::unique_ptr<VMInstVisitor> _visitor;
        std::vector<std::unique_ptr<VMInst>> _inst_list;
        FunctionSymbolTable _sym_table;
        size_t _pc = 0;
        EvalStack _eval_stack;
        std::string _cur_fun;
        bool _can_stop = false;
    };

    class VMInstVisitor {
    public:
        VMInstVisitor(VM &vm) : _vm(vm), _eval_stack(_vm.eval_stack()), _sym_table(_vm.sym_table()) {}

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
                case InstType::DefineFun:
                    visit(static_cast<const DefineFun&>(inst));break;
                case InstType::Return:
                    visit(static_cast<const Return&>(inst));break;
                case InstType::Addr:
                    break;
                case InstType::FunEnd:
                    break;
                case InstType::GetLocal:
                    visit(static_cast<const GetLocal&>(inst));break;
                case InstType::LocalVarOperator:
                    break;
                case InstType::SetLocal:
                    visit(static_cast<const SetLocal&>(inst));break;
                case InstType::UnsetAddr:
                    break;
                case InstType::Print:
                    break;
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
            _vm.begin_call(inst.target);
            // set value for stack frame
        }

        void visit(const DefineFun &inst) {
            auto entry = _vm._pc;
            _vm.sym_table()[inst.name].begin = entry;
            while(_vm._inst_list[entry]->type != InstType::FunEnd)
            {
                ++entry;
            }
            // pc increment per inst
            _vm._pc = entry - 1;
        }

        void visit(const Return &inst) {
            _vm.end_call();
        }

        void visit(const GetLocal &inst)
        {

        }

        void visit(const SetLocal &inst)
        {

        }


    private:
        VM &_vm;
        EvalStack &_eval_stack;
        FunctionSymbolTable &_sym_table;
    };

    void VM::run(std::vector<std::unique_ptr<VMInst>> &&inst_list, FunctionSymbolTable&& sym_table) {
        _sym_table = std::move(sym_table);
        _inst_list = std::move(inst_list);
        LOG_DEBUG("VM Start")
        _visitor = std::make_unique<VMInstVisitor>(*this);
        init();
        while(_pc < _inst_list.size() && !can_stop())
        {
            _pc++;
            auto &inst = _inst_list[_pc];
            _visitor->accept(*inst);
        }
        LOG_DEBUG("VM End")
    }
}