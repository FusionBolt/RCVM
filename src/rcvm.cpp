#include "rcvm.h"

using namespace RCVM;

void VM::init() {
    auto *kernel = gc.alloc_static_obj(VMGlobalClass);
    _eval_stack.push_pointer(kernel);
    begin_call(VMEntryFun, 0);
    // because pc will not increase if lose this
    // then first inst will be executed twice(first not increase)
    _pc_need_incr = true;
    STATE_LOG("Init:Jump To Main");
    LOG_DEBUG("Current PC:" + std::to_string(_pc));
}

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

void VM::begin_call(const string &f, size_t argc, bool super) {
    auto *obj = _eval_stack.get_object(argc);
    auto &fun = method_search(obj, f, super);
    if(fun.begin == UndefinedAddr)
    {
        fun.begin = load_method(obj->klass(), f, fun);
    }

    // 1. stack process
    _eval_stack.begin_call(fun.argc, fun.locals, _pc + 1, obj, f);
    // 2. set pc
    set_pc(fun.begin);

    EXEC_LOG("Call " + f + " new PC:" + std::to_string(_pc) + " ret pc:"
             + std::to_string(_eval_stack.current_frame()->ret_addr()));
}

void VM::end_call() {
    set_pc(_eval_stack.end_call());
    _can_stop = _eval_stack.current_frame()->prev() == nullptr;
    EXEC_LOG("Return");
}

size_t VM::load_method(const string &klass, const string &name, const FunInfo &f) {
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

void VM::pc_increase() {
    if(_pc_need_incr)
    {
        ++_pc;
    }
    else
    {
        _pc_need_incr = true;
    }
}

void VM::set_pc(size_t new_pc) {
    _pc = new_pc;
    _pc_need_incr = false;
}

void VM::relative_pc(int offset) {
    DEBUG_CHECK(static_cast<int>(_pc) + offset < 0,
                "invalid pc, pc:" + std::to_string(_pc) + "offset:" + std::to_string(offset))
    set_pc(_pc + offset);
}
