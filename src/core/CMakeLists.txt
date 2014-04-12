project(qtikzcore)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# find Qt5 modules
find_package(Qt5 "5.2.0" NO_MODULE REQUIRED Core Widgets)

include_directories(
    .
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
    Value.cpp
    node/Node.cpp
    MetaPos.cpp
    MetaPos2.cpp
    MetaPos2_p.cpp
    Document.cpp
    style/Style.cpp
    style/NodeStyle.cpp
    style/EdgeStyle.cpp

    path/Path.cpp
    path/UndoSetPathStyle.cpp
    path/UndoCreatePath.cpp
    path/UndoDeletePath.cpp

    path/edge/EdgePath.cpp
    path/edge/UndoSetEdgePos.cpp

    path/ellipse/EllipsePath.cpp
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

# generate the moc-file for tikz.h ourselves.
# NOTE: The moc file may not have the file ending ".moc", as including
# the .moc file in tikz.cpp will create an empty tikz.moc file.
qt5_generate_moc(tikz.h tikzmoc)

add_library(qtikzcore SHARED
    ${qtikzcore_SOURCES}
    tikzmoc
)

target_link_libraries(qtikzcore
    Qt5::Core
    Qt5::Widgets
)

# kate: indent-width 4; replace-tabs on;
