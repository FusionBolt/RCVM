#pragma once
#include "instructions.hpp"
#include "eval_stack.h"
#include "symbol_table.h"
#include "rc_object.h"
#include "gc.h"
#include "logger.h"
#include "debug_util.h"


namespace RCVM
{
    inline void print(const std::vector<long> &vars)
    {
        std::stringstream s;
        for (int i = 0; i < vars.size(); ++i)
        {
            s << vars[i] << " ";
        }
        STACK_LOG(s.str());
    }

    inline void print_all_frame_data(const std::vector<std::vector<long>> &data)
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

        void pc_increase();

        void set_pc(size_t new_pc);

        [[nodiscard]] size_t pc() const { return _pc; }

        EvalStack& eval_stack()
        {
            return _eval_stack;
        }

        void begin_call(const std::string& f, size_t argc, bool super = false);

        void end_call();

        size_t load_method(const std::string& klass, const std::string& name, const FunInfo& f);

        [[nodiscard]] bool can_stop() const { return _can_stop; }

        void set_can_stop() { _can_stop = true; }

        [[nodiscard]] bool pc_need_incr() const { return _pc_need_incr; }

    private:
        void init();

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

        void accept(const VMInst &inst);

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
            _vm.begin_call(_eval_stack.current_method(), inst.argc, true);
        }

    private:
        VM &_vm;
        EvalStack &_eval_stack;
    };


}