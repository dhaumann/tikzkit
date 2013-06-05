cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules
)

add_subdirectory(src/qtikzcore)
add_subdirectory(src/qtikzgui)
add_subdirectory(src/app)

enable_testing()
add_subdirectory(tests)

# kate: indent-width 4; replace-tabs on;

# notes: 
# http://stackoverflow.com/questions/15054117/aligning-qgraphicsitems-to-a-grid-when-dragging-and-dropping