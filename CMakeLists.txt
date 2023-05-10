cmake_minimum_required(VERSION 3.12)
project(gop2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
    message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
    file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/0.18.1/conan.cmake"
            "${CMAKE_BINARY_DIR}/conan.cmake"
            TLS_VERIFY ON)
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_cmake_autodetect(settings CPPSTD 17)

conan_cmake_configure(REQUIRES fmt/9.1.0
        GENERATORS cmake_find_package)

conan_cmake_install(PATH_OR_REFERENCE .
        BUILD missing
        REMOTE conancenter
        SETTINGS ${settings})

set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR} ${CMAKE_MODULE_PATH})
set(CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR} ${CMAKE_PREFIX_PATH})

find_package(fmt REQUIRED)

file(GLOB SRC_FILES CONFIGURE_DEPENDS 
    "*.h"
    "*.cpp"
    "comm/*.h"
    "comm/*.cpp"
    "pltl/*.h"
    "pltl/*.cpp"
)

add_executable(gop2 ${SRC_FILES})
target_link_libraries(gop2 PRIVATE fmt::fmt)