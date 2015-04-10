project(qtikzcore)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# find Qt5 modules
find_package(Qt5 "5.2.0" NO_MODULE REQUIRED Core Widgets)

include_directories(
    .
    document/
    utils/
    undo/
    visitor/
    style/
    node
    path
    path/edge
    path/ellipse
)

set(qtikzcore_SOURCES
    tikz.cpp

    document/Document.cpp

    style/Style.cpp
    style/NodeStyle.cpp
    style/EdgeStyle.cpp

    utils/Value.cpp
    utils/Pos.cpp
    utils/MetaPos.cpp
    utils/MetaPos_p.cpp
    utils/Uid.cpp
    utils/Entity.cpp
    utils/ConfigObject.cpp

    path/Path.cpp
    path/UndoSetPathStyle.cpp
    path/UndoCreatePath.cpp
    path/UndoDeletePath.cpp

    path/edge/EdgePath.cpp
    path/edge/UndoSetEdgePos.cpp

    path/ellipse/EllipsePath.cpp
    path/ellipse/UndoSetEllipsePos.cpp

    undo/UndoManager.cpp
    undo/UndoFactory.cpp
    undo/UndoGroup.cpp
    undo/UndoItem.cpp
    undo/Transaction.cpp

    node/Node.cpp
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

add_library(qtikzcore SHARED
    ${qtikzcore_SOURCES}
)

target_link_libraries(qtikzcore
    Qt5::Core
    Qt5::Widgets
)

# kate: indent-width 4; replace-tabs on;
