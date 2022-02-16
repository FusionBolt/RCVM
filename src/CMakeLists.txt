cmake_minimum_required(VERSION 3.20)
add_library(RCVM
        symbol_table.cpp
        debug_util.cpp
        eval_stack.cpp
        gc.cpp
        vm_parser.cpp
        rcvm.cpp
        vm_accept.cpp)
target_include_directories(RCVM PUBLIC ../include)
target_link_libraries(RCVM CONAN_PKG::glog)