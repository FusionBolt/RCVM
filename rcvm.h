#pragma once
#include "instructions.hpp"
#include "eval_stack.h"
#include "symbol_table.hpp"
#include "rc_object.h"
#include "gc.h"
#include "logger.h"

namespace RCVM
{
    void print(const std::vector<long>& vars)
    {
        LOG_DEBUG("Print Current Stack:");
        for (int i = 0; i < vars.size(); ++i)
        {
            std::cout << vars[i] << " ";
        }
        std::cout << std::endl;
        LOG_DEBUG("End Print");
    }

    class VMInstVisitor;
    class VM
    {
    public:
        constexpr static auto VMEntryFun = "main";
        constexpr static auto VMGlobalClass = "Kernel";
        VM() = default;

        void run();

        EvalStack& eval_stack()
        {
            return _eval_stack;
        }

        void begin_call(const std::string& klass, const std::string& f)
        {
            if(!global_class_table.contains(klass) || !global_class_table[klass]._methods.contains(f))
            {
                throw std::runtime_error("Target Function:" + f + " Not Found");
            }

            // todo: find definition
            auto &fun = global_class_table[klass]._methods[f];
            if(fun.begin == UndefinedAddr)
            {
                fun.begin = load_method(klass, f, fun);
            }
            // todo: var args
            // 1. stack process
            _eval_stack.begin_call(fun.argc, fun.locals, _pc);
            // 2. set pc
            _pc = fun.begin;
            EXEC_LOG("Call " + f + " PC:" + std::to_string(_pc));
        }

        size_t load_method(const std::string& klass, const std::string& name, const FunInfo& f)
        {
            EXEC_LOG("Load Function:" + name + " in Class:" + klass);
            // 1. get start
            auto start = static_cast<int>(_inst_list.size());
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
            EXEC_LOG("Return");
        }

        bool can_stop() const { return _eval_stack.empty(); }
    private:
        void init()
        {
            auto *kernel = gc.alloc_static_obj(VMGlobalClass);
            _eval_stack.push_pointer(kernel);
            begin_call(VMGlobalClass, VMEntryFun);
            STATE_LOG("Init:Jump To Main");
            LOG_DEBUG("Current PC:" + std::to_string(_pc));
        }

        friend class VMInstVisitor;
        std::shared_ptr<VMInstVisitor> _visitor;
        std::vector<std::shared_ptr<VMInst>> _inst_list;
        size_t _pc = 0;
        EvalStack _eval_stack;
        std::string _cur_fun;
        bool _can_stop = false;
    };

    class VMInstVisitor {
    public:
        VMInstVisitor(VM &vm) : _vm(vm), _eval_stack(_vm.eval_stack()){}

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
                    visit(static_cast<const Alloc&>(inst));break;
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

        void visit(const Alloc &inst)
        {
            auto obj = gc.alloc_stack_obj(inst.class_type);
            _eval_stack.push_pointer(obj);
        }

    private:
        VM &_vm;
        EvalStack &_eval_stack;
    };

    void VM::run() {
        STATE_LOG("VM Start");
        _visitor = std::make_unique<VMInstVisitor>(*this);
        init();
        STATE_LOG("VM Init Finish");
        // todo:check init ok
        while(_pc < _inst_list.size() && !can_stop())
        {
            auto &inst = _inst_list[_pc];
            INST_LOG(inst->to_string());
            _visitor->accept(*inst);
            STACK_LOG("Current Stack:");
            // print(_eval_stack.current_data());
            _pc++;
        }
        STATE_LOG("VM End");
    }
}