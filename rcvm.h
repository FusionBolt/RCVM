#pragma once
#include "instructions.hpp"
#include "eval_stack.h"
#include "symbol_table.hpp"

// todo:log option for enable dump pc, stack pointer...
#ifdef DEBUG
#define LOG_DEBUG(info) std::cout << info << std::endl;
#else
#define LOG_DEBUG(info)
#endif

namespace RCVM
{
    void print(const std::vector<int>& vars)
    {
        LOG_DEBUG("Print Current Stack:")
        for (int i = 0; i < vars.size(); ++i)
        {
            std::cout << vars[i] << " ";
        }
        std::cout << std::endl;
        LOG_DEBUG("End Print")
    }

    using FunctionSymbolTable = SymbolTable<FunInfo>;
    using ClassSymbolTable = SymbolTable<ClassInfo>;

    class VMInstVisitor;
    class VM
    {
    public:
        constexpr static auto VMEntryFun = "main";
        constexpr static auto VMGlobalClass = "Kernel";
        VM() = default;

        void run(ClassSymbolTable&& table);

        EvalStack& eval_stack()
        {
            return _eval_stack;
        }

        [[nodiscard]] const ClassSymbolTable & sym_table() const
        {
            return _sym_table;
        }

        [[nodiscard]] ClassSymbolTable& sym_table()
        {
            return _sym_table;
        }

        void begin_call(const std::string& klass, const std::string& f)
        {
            if(!_sym_table.contains(klass) || !_sym_table[klass]._methods.contains(f))
            {
                throw std::runtime_error("Target Function" + f + "Not Found");
            }

            // todo: find definition
            auto &fun = _sym_table[klass]._methods[f];
            if(fun.begin == UndefinedAddr)
            {
                fun.begin = load_method(fun);
            }
            // todo: var args
            // 1. stack process
            _eval_stack.begin_call(fun.argc, fun.locals, _pc);
            // 2. set pc
            _pc = fun.begin;
            LOG_DEBUG("Call " + f + " PC:" + std::to_string(_pc))
        }

        size_t load_method(const FunInfo& f)
        {
            // 1. get start
            auto start = std::max<int>(0, static_cast<int>(_inst_list.size() - 1));
            // todo:need reset inst list in f? it can be replaced with unique_ptr?
            // 2. load inst to inst_list
            for(auto &&inst : f.inst_list)
            {
                _inst_list.push_back(inst);
            }
            return start;
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
            begin_call(VMGlobalClass, VMEntryFun);
            LOG_DEBUG("Init:Jump To Main")
            LOG_DEBUG("Current PC:" + std::to_string(_pc))
        }

        friend class VMInstVisitor;
        std::shared_ptr<VMInstVisitor> _visitor;
        std::vector<std::shared_ptr<VMInst>> _inst_list;
        ClassSymbolTable _sym_table;
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
                case InstType::FunLabel:
                    visit(static_cast<const FunLabel&>(inst));break;
                case InstType::Return:
                    visit(static_cast<const Return&>(inst));break;
                case InstType::Addr:
                    break;
                case InstType::GetLocal:
                    visit(static_cast<const GetLocal&>(inst));break;
                case InstType::LocalVarOperator:
                    break;
                case InstType::SetLocal:
                    visit(static_cast<const SetLocal&>(inst));break;
                case InstType::UnsetAddr:
                    break;
                case InstType::Alloc:
                    break;
            }
        }

        void visit([[maybe_unused]] const Add &inst) {
            _eval_stack.exec([](auto &&a, auto&& b) { return a + b; });
        }

        void visit([[maybe_unused]] const Sub &inst) {
            _eval_stack.exec([](auto &&a, auto&& b) { return a - b; });
        }

        void visit([[maybe_unused]] const Mul &inst) {
            _eval_stack.exec([](auto &&a, auto&& b) { return a * b; });
        }

        void visit([[maybe_unused]] const Div &inst) {
            _eval_stack.exec([](auto &&a, auto&& b) { return a / b; });
        }

        void visit([[maybe_unused]] const Label &inst) {}

        void visit([[maybe_unused]] const DirectJump &inst) {}

        void visit([[maybe_unused]] const CondJump &inst) {}

        void visit(const Push &inst) {
            _eval_stack.push(inst.value);
        }

        void visit([[maybe_unused]] const Pop &inst) {
            _eval_stack.pop();
        }

        void visit(const Call &inst) {
            if(inst.target == "put_i")
            {
                auto value = _eval_stack.pop();
                std::cout << "put_i value:" << value << std::endl;
            }
            else
            {
                _vm.begin_call(inst.klass, inst.target);
            }
            // set value for stack frame
        }

        void visit([[maybe_unused]] const FunLabel &inst) {
            throw std::runtime_error("Inst FunLabel should not be reached PC:" + std::to_string(_vm._pc));
        }

        void visit([[maybe_unused]] const Return &inst) {
            auto result = _eval_stack.pop();
            _vm.end_call();
            _eval_stack.push(result);
        }

        void visit(const GetLocal &inst)
        {
            auto v = _eval_stack.get_local(inst.offset);
            _eval_stack.push(v);
            LOG_DEBUG("GetLocal Pos:" + std::to_string(inst.offset) + " value:" + std::to_string(v));
        }

        void visit(const SetLocal &inst)
        {
            auto v = _eval_stack.pop();
            _eval_stack.set_local(inst.offset, v);
            LOG_DEBUG("SetLocal Pos:" + std::to_string(inst.offset) + " value:" + std::to_string(v));
        }

    private:
        VM &_vm;
        EvalStack &_eval_stack;
        ClassSymbolTable &_sym_table;
    };

    void VM::run(ClassSymbolTable&& sym_table) {
        _sym_table = sym_table;
        LOG_DEBUG("VM Start")
        _visitor = std::make_unique<VMInstVisitor>(*this);
        init();
        LOG_DEBUG("VM Init Finish")
        while(_pc < _inst_list.size() && !can_stop())
        {
            _pc++;
            auto &inst = _inst_list[_pc];
            _visitor->accept(*inst);
            print(_eval_stack.current_data());
        }
        LOG_DEBUG("VM End")
    }
}