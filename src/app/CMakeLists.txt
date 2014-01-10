project(tikzkit)

find_package(Qt4 REQUIRED)
#find_package(PopplerQt4 REQUIRED)

include_directories(
    .
    ${CMAKE_SOURCE_DIR}/src/core
    ${CMAKE_SOURCE_DIR}/src/core/path
    ${CMAKE_SOURCE_DIR}/src/core/node
    ${CMAKE_SOURCE_DIR}/src/core/style
    ${CMAKE_SOURCE_DIR}/src/ui
    ${CMAKE_SOURCE_DIR}/src/ui/widgets
    #${POPPLER_QT4_INCLUDE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

set(tikzkit_SOURCES
    main.cpp
    DemoWidget.cpp
)

set(tikzkit_HEADERS
    DemoWidget.h
)

QT4_WRAP_CPP(tikzkit_HEADERS_MOC ${tikzkit_HEADERS})

include(${QT_USE_FILE})
add_definitions(
    ${QT_DEFINITIONS}
    #${POPPLER_QT4_DEFINITIONS}
)

set(tikzkit_FORMS MainWindow.ui)
QT4_WRAP_UI(tikzkit_FORMS_HEADERS ${tikzkit_FORMS})

add_executable(tikzkit
    ${tikzkit_SOURCES}
    ${tikzkit_HEADERS_MOC}
    ${tikzkit_FORMS_HEADERS}
)

target_link_libraries(tikzkit
    ${QT_LIBRARIES}
    #${POPPLER_QT4_LIBRARIES}
    qtikzcore
    qtikzui
)

# kate: indent-width 4; replace-tabs on;
