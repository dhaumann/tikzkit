project(qtikzcore)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# find Qt5 modules
find_package(Qt5 "5.2.0" NO_MODULE REQUIRED Core Widgets)

add_definitions("-std=c++11")

include_directories(
    .
    document/
    utils/
    undo/
    visitor/
    style/
    node
    path
    ${CMAKE_SOURCE_DIR}/src/include
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

    properties/Property.cpp
    properties/PropertyInterface.cpp
    properties/StringProperty.cpp
    properties/BoolProperty.cpp
    properties/ColorProperty.cpp
    properties/ValueProperty.cpp
    properties/PosProperty.cpp

    path/Path.cpp
    path/EdgePath.cpp
    path/EllipsePath.cpp

    node/Node.cpp

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
    entitysystem
)

# kate: indent-width 4; replace-tabs on;
