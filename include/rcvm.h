#pragma once
#include "debug_util.h"
#include "eval_stack.h"
#include "gc.h"
#include "instructions.hpp"
#include "logger.h"
#include "pc.h"
#include "rc_object.h"
#include "symbol_table.h"
#include "code_segment.h"

namespace RCVM
{

class VMInstVisitor;
class VM
{
public:
    constexpr static auto VMEntryFun = "main";
    constexpr static auto VMGlobalClass = "Kernel";
    VM() = default;

    VM(const VM &) = delete;
    VM operator=(const VM &) = delete;

    void run();

    [[nodiscard]] PC pc() const { return _pc; }

    EvalStack &eval_stack() { return _eval_stack; }

    void begin_call(const std::string &f, size_t argc, bool super = false);

    void end_call();

    [[nodiscard]] bool can_stop() const { return _can_stop; }

    void set_can_stop() { _can_stop = true; }

    [[nodiscard]] bool out_of_code_segment() const;

private:
    void init();

    friend class VMInstVisitor;
    std::shared_ptr<VMInstVisitor> _visitor;
    CodeSegment _code_segment;
    EvalStack _eval_stack;
    std::string _cur_fun;
    bool _can_stop = false;
    PC _pc;
};

class VMInstVisitor
{
public:
    VMInstVisitor(VM &vm) : _vm(vm), _eval_stack(_vm.eval_stack()) {}

    void accept(const VMInst &inst);

    void visit([[maybe_unused]] const Add &inst)
    {
        _eval_stack.exec(
            [](auto &&a, auto &&b)
            {
                return a + b;
            });
    }

    void visit([[maybe_unused]] const Sub &inst)
    {
        _eval_stack.exec(
            [](auto &&a, auto &&b)
            {
                return a - b;
            });
    }

    void visit([[maybe_unused]] const Mul &inst)
    {
        _eval_stack.exec(
            [](auto &&a, auto &&b)
            {
                return a * b;
            });
    }

    void visit([[maybe_unused]] const Div &inst)
    {
        _eval_stack.exec(
            [](auto &&a, auto &&b)
            {
                return a / b;
            });
    }

    void visit([[maybe_unused]] const Label &inst) {}

    void visit([[maybe_unused]] const DirectJump &inst) { NOT_IMPL }

    void visit([[maybe_unused]] const CondJump &inst) {}

    void visit(const Push &inst) { _eval_stack.push(inst.value); }

    void visit([[maybe_unused]] const Pop &inst) { _eval_stack.pop(); }

    void visit(const Call &inst)
    {
        if (inst.target == "put_i")
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

    void visit([[maybe_unused]] const FunLabel &inst)
    {
        throw std::runtime_error("Inst FunLabel should not be reached PC:"
                                 + std::to_string(_vm.pc().current()));
    }

    void visit([[maybe_unused]] const Return &inst)
    {
        auto result = _eval_stack.pop();
        _vm.end_call();
        _eval_stack.push(result);
    }

    void visit(const GetLocal &inst)
    {
        auto v = _eval_stack.get_local(inst.offset);
        _eval_stack.push(v);
        EXEC_LOG("GetLocal Pos:" + std::to_string(inst.offset)
                 + " value:" + std::to_string(v));
    }

    void visit(const SetLocal &inst)
    {
        auto v = _eval_stack.pop();
        _eval_stack.set_local(inst.offset, v);
        EXEC_LOG("SetLocal Pos:" + std::to_string(inst.offset)
                 + " value:" + std::to_string(v));
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

    void visit(const InvokeSuper &inst)
    {
        _vm.begin_call(_eval_stack.current_method(), inst.argc, true);
    }

    void visit(const SetClassMemberVar &inst)
    {
        auto *obj = _eval_stack.this_ptr();
        obj->set_value(inst.id, _eval_stack.pop());
    }

    void visit(const GetClassMemberVar &inst)
    {
        auto *obj = _eval_stack.this_ptr();
        _eval_stack.push(obj->get_number_field(inst.id));
    }

    void visit([[maybe_unused]] const EQ &inst)
    {
        _eval_stack.exec(BinaryOp::EQ);
    }

    void visit([[maybe_unused]] const GT &inst)
    {
        _eval_stack.exec(BinaryOp::GT);
    }

    void visit([[maybe_unused]] const LT &inst)
    {
        _eval_stack.exec(BinaryOp::LT);
    }

    void visit([[maybe_unused]] const JumpAfterIf &inst)
    {
        throw std::runtime_error(
            "JumpAfterIf should be eliminate in compile time");
    }

    void visit([[maybe_unused]] const JumpFalse &inst)
    {
        auto cond = _eval_stack.pop();
        if (cond == 0)
        {
            _vm.pc().relative_jump(inst.offset);
        }
    }

    void visit(const RelativeJump &inst)
    {
        _vm.pc().relative_jump(inst.offset);
    }

private:
    VM &_vm;
    EvalStack &_eval_stack;
};

} // namespace RCVM