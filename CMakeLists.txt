#
# The official cmake documentation for Qt5 is available here:
# http://qt-project.org/doc/qt-5/cmake-manual.html
#
cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules
)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# prepare include folder in build dir: include/tikz/core/*.h
file(GLOB_RECURSE ALL_HEADERS ${CMAKE_SOURCE_DIR}/src/core/*.h)
foreach(header ${ALL_HEADERS})
    file(STRINGS "${header}" lines REGEX "TIKZCORE_EXPORT")
    if(lines)
        file(COPY ${header}
             DESTINATION ${CMAKE_BINARY_DIR}/include/tikz/core/)
    endif()
endforeach()

# prepare include folder in build dir: include/tikz/ui/*.h
file(GLOB_RECURSE ALL_HEADERS ${CMAKE_SOURCE_DIR}/src/ui/*.h)
foreach(header ${ALL_HEADERS})
    file(STRINGS "${header}" lines REGEX "TIKZUI_EXPORT")
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
