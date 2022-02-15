#pragma once
#include "instructions.hpp"
#include "eval_stack.h"
#include "symbol_table.hpp"
#include "rc_object.h"
#include "gc.h"
#include "logger.h"
#include "debug_util.h"
#include "exception.h"

namespace RCVM
{
    void print(const std::vector<long> &vars)
    {
        std::stringstream s;
        for (int i = 0; i < vars.size(); ++i)
        {
            s << vars[i] << " ";
        }
        STACK_LOG(s.str());
    }

    void print_all_frame_data(const std::vector<std::vector<long>> &data)
    {
        STACK_LOG("Print Stack Frame:");
        for(auto &frame_data : data)
        {
            print(frame_data);
        }
        STACK_LOG("End Frame");
    }

    class VMInstVisitor;
    class VM
    {
    public:
        constexpr static auto VMEntryFun = "main";
        constexpr static auto VMGlobalClass = "Kernel";
        VM() = default;

        VM(const VM&) = delete;
        VM operator=(const VM&) = delete;

        void run();

        void pc_increase()
        {
            if(_pc_need_incr)
            {
                ++_pc;
            }
            else
            {
                _pc_need_incr = true;
            }
        }

        void set_pc(size_t new_pc)
        {
            _pc = new_pc;
            _pc_need_incr = false;
        }

        [[nodiscard]] size_t pc() const { return _pc; }

        EvalStack& eval_stack()
        {
            return _eval_stack;
        }

        FunInfo &method_search(const RcObject * const obj, const std::string &f)
        {
            return method_search(obj->klass(), f);
        }

        // todo:test
        FunInfo &method_search(const std::string &klass, const std::string &f)
        {
            auto &class_table = global_class_table[klass];
            if(class_table._methods.contains(f))
            {
                return class_table._methods[f];
            }
            else if(class_table._parents.empty())
            {
                throw MethodNotFoundException(f);
            }
            else
            {
                // todo:mixin
                return method_search(class_table._parents[0], f);
            }
        }

        void begin_call(const std::string& f, size_t argc)
        {
            auto *obj = _eval_stack.get_object(argc);
            auto &fun = method_search(obj, f);
            if(fun.begin == UndefinedAddr)
            {
                fun.begin = load_method(obj->klass(), f, fun);
            }

            // 1. stack process
            _eval_stack.begin_call(fun.argc, fun.locals, _pc + 1, obj);
            // 2. set pc
            set_pc(fun.begin);

            EXEC_LOG("Call " + f + " new PC:" + std::to_string(_pc) + " ret pc:"
                + std::to_string(_eval_stack.current_frame()->ret_addr()));
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
            set_pc(_eval_stack.end_call());
            _can_stop = _eval_stack.current_frame()->prev() == nullptr;
            EXEC_LOG("Return");
        }

        [[nodiscard]] bool can_stop() const { return _can_stop; }

        void set_can_stop() { _can_stop = true; }

        [[nodiscard]] bool pc_need_incr() const { return _pc_need_incr; }

    private:
        void init()
        {
            auto *kernel = gc.alloc_static_obj(VMGlobalClass);
            _eval_stack.push_pointer(kernel);
            begin_call(VMEntryFun, 0);
            // because pc will not increase if lose this
            // then first inst will be executed twice(first not increase)
            _pc_need_incr = true;
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
        bool _pc_need_incr = true;
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
                case InstType::PushThis:
                    visit(static_cast<const PushThis&>(inst));break;
                case InstType::InvokeSuper:
                    visit(static_cast<const InvokeSuper&>(inst));break;
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
                _vm.begin_call(inst.target, inst.argc);
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
            EXEC_LOG("GetLocal Pos:" + std::to_string(inst.offset) + " value:" + std::to_string(v));
        }

        void visit(const SetLocal &inst)
        {
            auto v = _eval_stack.pop();
            _eval_stack.set_local(inst.offset, v);
            EXEC_LOG("SetLocal Pos:" + std::to_string(inst.offset) + " value:" + std::to_string(v));
        }

        void visit(const Alloc &inst)
        {
            auto obj = gc.alloc_stack_obj(inst.class_type);
            _eval_stack.push_pointer(obj);
        }

        void visit([[maybe_unused]] const PushThis &inst)
        {
            _eval_stack.push_pointer(_eval_stack.this_ptr());
        }

        void visit([[maybe_unused]] const InvokeSuper &inst)
        {

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
        while (_pc < _inst_list.size() && !can_stop()) {
            auto &inst = _inst_list[_pc];
            INST_LOG(inst->to_string());
            _visitor->accept(*inst);
            STACK_LOG("Current Stack Info:");
            STACK_LOG("") << "depth:" << _eval_stack.depth() << " pc:" << _pc << " inst_list size:"
                          << _inst_list.size();
            print(_eval_stack.current_data());
            print_all_frame_data(_eval_stack.all_frame_data());
            pc_increase();
            dump_inst(_inst_list, "inst.txt");
        }
        STATE_LOG("VM End");
    }
}