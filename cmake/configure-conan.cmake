function(_SET_CONANSETTING OUT_VAR SET_NAME SET_VALUE)
    set(${OUT_VAR} "${${OUT_VAR}};${SET_NAME}=${SET_VALUE}" PARENT_SCOPE)
endfunction()

set(CMAKE_CXX_STANDARD 20)
_SET_CONANSETTING(CONAN_SETTINGS "compiler.cppstd" ${CMAKE_CXX_STANDARD})
