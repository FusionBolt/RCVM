#include "symbol_table.h"
#include "rc_object.h"
#include "exception.h"

namespace RCVM {
    std::string get_parent_class(const RcObject *obj) {
        return global_class_table[obj->klass()]._parent;
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
        }
        if(class_table._parent.empty()) {
            throw MethodNotFoundException(f);
        }
        else {
            return method_search(class_table._parent, f);
        }
    }

    std::set<std::string> find_all_var(const string &klass) {
        auto parents = global_class_table[klass]._parent;
        auto &vars = global_class_table[klass]._vars;
        auto result = std::set(vars.begin(), vars.end());
        if(parents.empty())
        {
            return result;
        }
        else
        {
            result.merge(find_all_var(parents));
            return result;
        }
    }

    size_t class_vars_size(const string &klass) {
        return global_class_table[klass]._vars.size();
        // slow impl, only used for dynamic class
        // should create info when load symbol table
        // return find_all_var(klass).size();
    }
}