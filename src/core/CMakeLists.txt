project(tikzkitcore)

find_package(Qt5 "5.2.0" NO_MODULE REQUIRED Core Widgets)

add_library(tikzkitcore SHARED
    tikz.cpp

    document/Document.cpp
    document/UndoCreateEntity.cpp
    document/UndoDeleteEntity.cpp
    document/UndoSetProperty.cpp

    style/Style.cpp

    utils/Value.cpp
    utils/Pos.cpp
    utils/MetaPos.cpp
    utils/MetaPos_p.cpp
    utils/Uid.cpp
    utils/Entity.cpp
    utils/ConfigObject.cpp
    utils/PropertyManager.cpp

    properties/Property.cpp
    properties/PropertyInterface.cpp
    properties/StringProperty.cpp
    properties/BoolProperty.cpp
    properties/ColorProperty.cpp
    properties/ValueProperty.cpp
    properties/PosProperty.cpp

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
    node/UndoSetNodePos.cpp

    visitor/Visitor.cpp
    visitor/SerializeVisitor.cpp
    visitor/DeserializeVisitor.cpp
    visitor/TikzExportVisitor.cpp
    visitor/TikzExport.cpp
)

target_compile_features(tikzkitcore PRIVATE cxx_std_14)
target_compile_options(tikzkitcore PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)

target_link_libraries(tikzkitcore
    Qt5::Core
    Qt5::Widgets
)

target_include_directories(tikzkitcore
    PRIVATE
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

# kate: indent-width 4; replace-tabs on;
