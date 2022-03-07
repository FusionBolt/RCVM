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
        add_compile_options(-Wno-four-char-constants -Wno-sometimes-uninitialized -Wno-deprecated -Wno-unused-private-field)
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        add_compile_options(-Wno-uninitialized)
    else ()
        add_compile_options(-Wno-maybe-uninitialized -Wno-deprecated-copy)
        add_link_options(-Wl,--exclude-libs,ALL)
    endif()
endif()

include(cmake/configure-conan.cmake)
include(cmake/conan.cmake)

if (NOT CONAN_EXPORTED)
    conan_check()
endif ()

if (CONAN_EXPORTED)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
else ()
    if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
        message(STATUS "Downloading conan.cmake")
        file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/master/conan.cmake"
                "${CMAKE_BINARY_DIR}/conan.cmake" SHOW_PROGRESS
                STATUS _download_status)
        list(GET _download_status 0 _download_status_code)
        list(GET _download_status 1 _download_status_msg)
        if(NOT _download_status_code EQUAL 0)
            file(REMOVE "${CMAKE_BINARY_DIR}/conan.cmake")
            message(FATAL_ERROR "Failed to download conan.cmake, status code is ${_download_status_code}, msg is ${_download_status_msg}")
        endif()
    endif()

    include(${CMAKE_SOURCE_DIR}/cmake/conan.cmake)
    conan_cmake_run(CONANFILE conanfile.py
            BASIC_SETUP CMAKE_TARGETS
            OPTIONS ${CONAN_OPTS}
            SETTINGS ${CONAN_SETTINGS}
            BUILD missing)
endif ()

if(RCVM_TEST)
    add_subdirectory(test)
endif()

add_subdirectory(src)
add_executable(Run src/main.cpp src/code_segment.cpp include/code_segment.h)
target_link_libraries(Run RCVM)