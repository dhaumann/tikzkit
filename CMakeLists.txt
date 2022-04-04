cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(tikzkit VERSION "0.80")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 "5.15.2" NO_MODULE REQUIRED Core Widgets Svg Test)
add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x050f00)

include(FeatureSummary)

# prepare include folder in build dir: include/tikz/core/*.h
file(GLOB_RECURSE ALL_HEADERS ${CMAKE_SOURCE_DIR}/src/core/*.h)
foreach(header ${ALL_HEADERS})
    file(STRINGS "${header}" lines REGEX "TIKZKITCORE_EXPORT")
    if(lines)
        file(COPY ${header}
             DESTINATION ${CMAKE_BINARY_DIR}/include/tikz/core/)
    endif()
endforeach()

# prepare include folder in build dir: include/tikz/ui/*.h
file(GLOB_RECURSE ALL_HEADERS ${CMAKE_SOURCE_DIR}/src/ui/*.h)
foreach(header ${ALL_HEADERS})
    file(STRINGS "${header}" lines REGEX "TIKZKITUI_EXPORT")
    if(lines)
        file(COPY ${header}
             DESTINATION ${CMAKE_BINARY_DIR}/include/tikz/ui/)
    endif()
endforeach()

# _USE_MATH_DEFINES needed on Windows to define M_PI in <cmath>
add_compile_definitions(_USE_MATH_DEFINES)

add_subdirectory(src/core)
add_subdirectory(src/qtpropertybrowser/src)
add_subdirectory(src/ui)
add_subdirectory(src/app)

enable_testing()
add_subdirectory(tests)

# kate: indent-width 4; replace-tabs on;
