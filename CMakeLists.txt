#
# The official cmake documentation for Qt5 is available here:
# http://qt-project.org/doc/qt-5/cmake-manual.html
#
cmake_minimum_required(VERSION 2.8.11)

set(CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules
)

add_definitions("-std=c++11")

add_subdirectory(src/core)
add_subdirectory(src/qtpropertybrowser/src)
add_subdirectory(src/ui)
add_subdirectory(src/app)

enable_testing()
add_subdirectory(tests)

# kate: indent-width 4; replace-tabs on;
