project(qtikzgui)

find_package(Qt4 REQUIRED)

include_directories(
    .
    ${CMAKE_SOURCE_DIR}/src/qtikzcore
)

set(qtikzgui_SOURCES
#     TikzItem.cpp
    NodeItem.cpp
#     EdgeItem.cpp
    TikzScene.cpp
)

set(qtikzgui_HEADERS
#     TikzItem.h
    NodeItem.h
#     EdgeItem.h
    TikzScene.h
)

QT4_WRAP_CPP(qtikzgui_HEADERS_MOC ${qtikzgui_HEADERS})

include(${QT_USE_FILE})
add_definitions(
    -DMAKE_TIKZGUI
    ${QT_DEFINITIONS}
)

add_library(qtikzgui SHARED
    ${qtikzgui_SOURCES}
    ${qtikzgui_HEADERS_MOC}
)

target_link_libraries(qtikzgui ${QT_LIBRARIES} qtikzcore)

# kate: indent-width 4; replace-tabs on;
