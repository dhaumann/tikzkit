project(qtikzcore)

find_package(Qt4 REQUIRED)
find_package(QJSON REQUIRED)

include_directories(
    .
    undo/
    visitor/
    node
    path
    path/edge
    path/ellipse
    ${QJSON_INCLUDE_DIR}
)

set(qtikzcore_SOURCES
    Coord.cpp
    Value.cpp
    Edge.cpp
    node/Node.cpp
    MetaPos.cpp
    Document.cpp
    Style.cpp
    NodeStyle.cpp
    EdgeStyle.cpp
    StyleManager.cpp

    path/Path.cpp
    path/UndoSetPathStyle.cpp
    path/UndoCreatePath.cpp
    path/UndoDeletePath.cpp

    path/edge/EdgePath.cpp
    path/edge/UndoConnectEdge.cpp
    path/edge/UndoDisconnectEdge.cpp
    path/edge/UndoSetEdgeAnchor.cpp
    path/edge/UndoSetEdgePos.cpp

    path/ellipse/EllipsePath.cpp
    path/ellipse/UndoConnectEllipse.cpp
    path/ellipse/UndoDisconnectEllipse.cpp
    path/ellipse/UndoSetEllipseAnchor.cpp
    path/ellipse/UndoSetEllipsePos.cpp

    undo/UndoItem.cpp

    node/UndoCreateNode.cpp
    node/UndoDeleteNode.cpp
    node/UndoSetNodePos.cpp
    node/UndoSetNodeText.cpp
    node/UndoSetNodeStyle.cpp

    visitor/Visitor.cpp
    visitor/SerializeVisitor.cpp
    visitor/DeserializeVisitor.cpp
    visitor/TikzExportVisitor.cpp
    visitor/TikzExport.cpp
)

set(qtikzcore_HEADERS
    Coord.h
    node/Node.h
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
