#include "gc.h"

RCVM::RcObject *RCVM::GC::new_obj(const string &klass, RCVM::Flag flag) {
    if(!global_class_table.contains(klass))
    {
        throw std::runtime_error("Class " + klass + " Not Exist");
    }
    // todo: how to save number and obj?
    auto pointer = new RcObject(klass, flag, global_class_table[klass]._vars.size());
    if(pointer == nullptr)
    {
        throw std::runtime_error("alloc nullptr");
        //run();
    }
    _obj_list.insert(pointer);
    // compute klass size
    return pointer;
}
