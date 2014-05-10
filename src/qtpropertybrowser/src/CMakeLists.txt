project(qtpropertybrowser)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# find Qt5 modules
find_package(Qt5 "5.2.0" NO_MODULE REQUIRED Core Widgets)

add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE)

# include_directories(
#     .
# )

# contains(TEMPLATE, lib):contains(CONFIG, shared):DEFINES += QT_QTPROPERTYBROWSER_EXPORT
# else:qtpropertybrowser-uselib:DEFINES += QT_QTPROPERTYBROWSER_IMPORT

set(qtpropertybrowser_SOURCES
    qtpropertybrowser.cpp
    qtpropertymanager.cpp
    qteditorfactory.cpp
    qtvariantproperty.cpp
    qttreepropertybrowser.cpp
    qtbuttonpropertybrowser.cpp
    qtgroupboxpropertybrowser.cpp
    qtpropertybrowserutils.cpp
)

set(qtpropertybrowser_RESOURCES qtpropertybrowser.qrc)
qt5_add_resources(qtpropertybrowser_RESOURCES_RCC ${qtpropertybrowser_RESOURCES})

add_library(qtpropertybrowser STATIC
    ${qtpropertybrowser_SOURCES}
    ${qtpropertybrowser_RESOURCES_RCC}
    ${qtpropertybrowser_FORMS_HEADERS}
)

target_link_libraries(qtpropertybrowser
    Qt5::Core
    Qt5::Widgets
)

# kate: indent-width 4; replace-tabs on;
