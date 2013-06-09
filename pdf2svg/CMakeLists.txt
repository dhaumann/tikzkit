cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake-modules
)

project(pdf2svg)

find_package(Qt4 COMPONENTS QtCore QtGui QtSvg REQUIRED)
find_package(PopplerQt4 REQUIRED)

add_definitions(
    ${QT_DEFINITIONS}
    ${POPPLER_QT4_DEFINITIONS}
)

include(${QT_USE_FILE})

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${POPPLER_QT4_INCLUDE_DIR}
)

set(pdf2svg_SOURCES
    pdf2svg.cpp
)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )

add_executable(pdf2svg
    ${pdf2svg_SOURCES}
)

target_link_libraries(pdf2svg ${QT_LIBRARIES} ${POPPLER_QT4_LIBRARIES})

# kate: indent-width 4; replace-tabs on;
