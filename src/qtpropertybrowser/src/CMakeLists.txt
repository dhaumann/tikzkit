project(qtpropertybrowser)

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

target_compile_features(qtpropertybrowser PRIVATE cxx_std_14)

target_link_libraries(qtpropertybrowser
    Qt5::Core
    Qt5::Widgets
)

# kate: indent-width 4; replace-tabs on;
