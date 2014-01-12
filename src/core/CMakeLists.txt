project(qtikzcore)

find_package(Qt4 REQUIRED)
find_package(QJSON REQUIRED)

include_directories(
    .
    undo/
    visitor/
    style/
    node
    path
    path/edge
    path/ellipse
    ${QJSON_INCLUDE_DIR}
)

set(qtikzcore_SOURCES
    Value.cpp
    node/Node.cpp
    MetaPos.cpp
    Document.cpp
    style/Style.cpp
    style/NodeStyle.cpp
    style/EdgeStyle.cpp

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
    node/Node.h
    MetaPos.h

    path/Path.h
    path/edge/EdgePath.h
    path/ellipse/EllipsePath.h

    style/Style.h
    style/NodeStyle.h
    style/EdgeStyle.h

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

#
# copy include files
#

set(qtikzcore_EXPORT_HEADERS
    node/Node.h
    MetaPos.h
    path/Path.h
    path/ellipse/EllipsePath.h
    style/Style.h
    style/NodeStyle.h
    style/EdgeStyle.h
    Document.h
    Value.h
    tikz.h
    tikz_export.h
)

foreach(Header IN LISTS qtikzcore_EXPORT_HEADERS)
    get_filename_component(HeaderFilename "${Header}" NAME)
    set(Output "~/out/project/${HeaderFilename}")
    list(APPEND PublishedHeaders "${Output}")
    add_custom_command(
        OUTPUT "${Output}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/src/core/${Header}"
        "${CMAKE_SOURCE_DIR}/src/tikz/core/${HeaderFilename}"
        MAIN_DEPENDENCY "${Header}"
        COMMENT "Publishing ${HeaderFilename}"
        VERBATIM
    )
endforeach()
add_custom_target(
 publish_headers
 ALL
 DEPENDS qtikzcore
 SOURCES ${qtikzcore_EXPORT_HEADERS}
)

# kate: indent-width 4; replace-tabs on;
