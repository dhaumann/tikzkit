project(entitysystem)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# find Qt5 modules
find_package(Qt5 "5.2.0" NO_MODULE REQUIRED Core)

add_definitions("-std=c++11")
add_definitions(-DMAKE_ENTITY_SYSTEM)

include_directories(
    .
)

set(entitysystem_SOURCES
    Document.cpp

    Eid.cpp
    Entity.cpp
    ConfigObject.cpp

    HistoryManager.cpp
    HistoryItemFactory.cpp
    HistoryGroup.cpp
    HistoryItem.cpp
    HistoryTransaction.cpp
    CreateEntityHistoryItem.cpp
    DeleteEntityHistoryItem.cpp
    ChangeEntityHistoryItem.cpp
)

add_library(entitysystem SHARED
    ${entitysystem_SOURCES}
)

target_link_libraries(entitysystem
    Qt5::Core
)

# kate: indent-width 4; replace-tabs on;
