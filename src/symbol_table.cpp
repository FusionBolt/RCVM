#include "symbol_table.h"
#include "rc_object.h"
#include "exception.h"

namespace RCVM {
    std::string get_parent_class(const RcObject *obj) {
        return global_class_table[obj->klass()]._parents[0];
    }

    FunInfo &method_search(const RcObject *obj, const string &f, bool super) {
        auto klass = super ? get_parent_class(obj) : obj->klass();
        return method_search(klass, f);
    }

    // todo:test
    FunInfo &method_search(const string &klass, const string &f) {
        auto &class_table = global_class_table[klass];
        if (class_table._methods.contains(f)) {
            return class_table._methods[f];
        } else if (class_table._parents.empty()) {
            throw MethodNotFoundException(f);
        } else {
            // todo:mixin
            return method_search(class_table._parents[0], f);
        }
    }
}