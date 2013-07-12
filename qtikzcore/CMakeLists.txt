project(qtikzcore)

find_package(Qt4 REQUIRED)

include_directories(
    .
    undo/
)

set(qtikzcore_SOURCES
    Coord.cpp
    Edge.cpp
    Node.cpp
    MetaNode.cpp
    Path.cpp
    Document.cpp
    Style.cpp
    NodeStyle.cpp
    EdgeStyle.cpp
    StyleManager.cpp

    undo/UndoItem.cpp
    undo/UndoCreateNode.cpp
    undo/UndoDeleteNode.cpp
    undo/UndoCreateEdge.cpp
    undo/UndoDeleteEdge.cpp
    undo/UndoDisconnectEdge.cpp
)

set(qtikzcore_HEADERS
    Coord.h
    Node.h
    MetaNode.h
    Edge.h
    Path.h
    Style.h
    NodeStyle.h
    EdgeStyle.h
    Document.h
)

QT4_WRAP_CPP(qtikzcore_HEADERS_MOC ${qtikzcore_HEADERS})

include(${QT_USE_FILE})
add_definitions(
    -DMAKE_TIKZCORE
    ${QT_DEFINITIONS}
)

add_library(qtikzcore SHARED
    ${qtikzcore_SOURCES}
    ${qtikzcore_HEADERS_MOC}
)

target_link_libraries(qtikzcore ${QT_LIBRARIES})

# kate: indent-width 4; replace-tabs on;
