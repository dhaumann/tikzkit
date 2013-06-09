project(qtikz)

find_package(Qt4 REQUIRED)

include_directories(
    .
    ${CMAKE_SOURCE_DIR}/qtikzcore
    ${CMAKE_SOURCE_DIR}/qtikzgui
    ${CMAKE_SOURCE_DIR}/qtikzgui/widgets
)

set(qtikz_SOURCES
    main.cpp
    DemoWidget.cpp
)

set(qtikz_HEADERS
    DemoWidget.h
)

QT4_WRAP_CPP(qtikz_HEADERS_MOC ${qtikz_HEADERS})

include(${QT_USE_FILE})
add_definitions(
    ${QT_DEFINITIONS}
)

add_executable(qtikz
    ${qtikz_SOURCES}
    ${qtikz_HEADERS_MOC}
)

target_link_libraries(qtikz ${QT_LIBRARIES} qtikzcore qtikzgui)

# kate: indent-width 4; replace-tabs on;
