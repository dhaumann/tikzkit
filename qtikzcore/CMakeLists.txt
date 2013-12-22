project(qtikzcore)

find_package(Qt4 REQUIRED)
find_package(QJSON REQUIRED)

include_directories(
    .
    undo/
    visitor/
    path
    path/edge
    ${QJSON_INCLUDE_DIR}
)

set(qtikzcore_SOURCES
    Coord.cpp
    Value.cpp
    Edge.cpp
    Node.cpp
    MetaPos.cpp
    Document.cpp
    Style.cpp
    NodeStyle.cpp
    EdgeStyle.cpp
    StyleManager.cpp

    path/Path.cpp
    path/UndoSetPathStyle.cpp
    path/edge/EdgePath.cpp
    path/edge/UndoConnectEdge.cpp
    path/edge/UndoDisconnectEdge.cpp

    undo/UndoItem.cpp
    undo/UndoCreateNode.cpp
    undo/UndoDeleteNode.cpp
    undo/UndoCreatePath.cpp
    undo/UndoDeletePath.cpp
    undo/UndoCreateEdge.cpp
    undo/UndoDeleteEdge.cpp
    undo/UndoSetNodePos.cpp
    undo/UndoSetNodeText.cpp
    undo/UndoSetNodeStyle.cpp
    undo/UndoSetEdgeAnchor.cpp
    undo/UndoSetEdgePos.cpp
    undo/UndoSetEdgeType.cpp

    visitor/Visitor.cpp
    visitor/SerializeVisitor.cpp
    visitor/DeserializeVisitor.cpp
    visitor/TikzExportVisitor.cpp
    visitor/TikzExport.cpp
)

set(qtikzcore_HEADERS
    Coord.h
    Node.h
    MetaPos.h
    Edge.h
    path/Path.h
    path/edge/EdgePath.h
    Style.h
    NodeStyle.h
    EdgeStyle.h
    Document.h
    tikz.h
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

target_link_libraries(qtikzcore
    ${QT_LIBRARIES}
    ${QJSON_LIBRARIES}
)

# kate: indent-width 4; replace-tabs on;
