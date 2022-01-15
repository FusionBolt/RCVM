cmake_minimum_required(VERSION 3.20)
project(RCVM)

set(CMAKE_CXX_STANDARD 20)

# add limit
if (MSVC)
    add_definitions(/D_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS /D_CRT_SECURE_NO_WARNINGS /DNOMINMAX)
    add_compile_options(/wd4267 /wd4251 /wd4244 /FC /utf-8 /W3 /WX)
else()
    add_compile_options(-fvisibility=hidden)
    add_compile_options(-Wno-unused-parameter -Wall -Wextra -pedantic -Werror -Wno-multichar -Wno-missing-field-initializers -Wno-unused-function -Wno-type-limits -Wno-unused-local-typedefs -Wno-sign-compare)
    if (APPLE)
        add_compile_options(-Wno-four-char-constants -Wno-sometimes-uninitialized -Wno-deprecated)
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        add_compile_options(-Wno-uninitialized)
    else ()
        add_compile_options(-Wno-maybe-uninitialized -Wno-deprecated-copy)
        add_link_options(-Wl,--exclude-libs,ALL)
    endif()
endif()

add_executable(RCVM
        main.cpp)
