enable_testing()

message(STATUS "Test")
macro(add_test_exec name)
    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name} PRIVATE
            RCVM
            CONAN_PKG::gtest
            CONAN_PKG::glog)
    add_test(NAME ${name} COMMAND ${name})
endmacro()
file(GLOB TEST_NAMES CONFIGURE_DEPENDS test_*.cpp)

foreach(test_name ${TEST_NAMES})
    get_filename_component(tname ${test_name} NAME_WE)
    add_test_exec(${tname})
endforeach()