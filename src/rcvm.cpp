#include "rcvm.h"

using namespace RCVM;

void VM::init() {
    auto *kernel = gc.alloc_static_obj(VMGlobalClass);
    _eval_stack.push_pointer(kernel);
    begin_call(VMEntryFun, 0);
    // because pc will not increase if lose this
    // then first inst will be executed twice(first not increase)
    // todo:remove fun label and remove this
    _pc.force_need_incr();
    STATE_LOG("Init:Jump To Main");
    LOG_DEBUG("Current PC:" + std::to_string(_pc.current()));
}

void VM::run() {
    STATE_LOG("VM Start");
    _visitor = std::make_unique<VMInstVisitor>(*this);
    init();
    STATE_LOG("VM Init Finish");
    // todo:check init ok
    while (out_of_code_segment() && !can_stop()) {
        auto inst = _code_segment[_pc];
        INST_LOG(inst->to_string());
        _visitor->accept(*inst);
        STACK_LOG("Current Stack Info:");
        STACK_LOG("") << "depth:" << _eval_stack.depth() << " pc:" << _pc << " inst_list size:"
                      << _code_segment.size();
        print(_eval_stack.current_data());
        print_all_frame_data(_eval_stack.all_frame_data());
        _pc.increase();
        dump_inst(_code_segment.inst_list(), "inst.txt");
    }
    STATE_LOG("VM End");
}

void VM::begin_call(const string &f, size_t argc, bool super) {
    auto *obj = _eval_stack.get_object(argc);
    auto &fun = method_search(obj, f, super);
    if(fun.begin == UndefinedAddr)
    {
        fun.begin = _code_segment.load_method(obj->klass(), f, fun);
    }

    // 1. stack process
    _eval_stack.begin_call(fun.argc, fun.locals, _pc + 1, obj, f);
    // 2. set pc
    _pc.absolute_jump(fun.begin);

    EXEC_LOG("Call " + f + " new PC:" + std::to_string(_pc) + " ret pc:"
             + std::to_string(_eval_stack.current_frame()->ret_addr()));
}

void VM::end_call() {
    _pc.absolute_jump(_eval_stack.end_call());
    _can_stop = _eval_stack.current_frame()->prev() == nullptr;
    EXEC_LOG("Return");
}

bool VM::out_of_code_segment() const
{
    return _pc < _code_segment.size();
}
