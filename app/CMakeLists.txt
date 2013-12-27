project(qtikz)

find_package(Qt4 REQUIRED)
#find_package(PopplerQt4 REQUIRED)

include_directories(
    .
    ${CMAKE_SOURCE_DIR}/qtikzcore
    ${CMAKE_SOURCE_DIR}/qtikzcore/path
    ${CMAKE_SOURCE_DIR}/qtikzcore/node
    ${CMAKE_SOURCE_DIR}/qtikzcore/style
    ${CMAKE_SOURCE_DIR}/qtikzgui
    ${CMAKE_SOURCE_DIR}/qtikzgui/widgets
    #${POPPLER_QT4_INCLUDE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

set(qtikz_SOURCES
    main.cpp
    DemoWidget.cpp
)

set(qtikz_HEADERS
    DemoWidget.h
)

QT4_WRAP_CPP(qtikz_HEADERS_MOC ${qtikz_HEADERS})

include(${QT_USE_FILE})
add_definitions(
    ${QT_DEFINITIONS}
    #${POPPLER_QT4_DEFINITIONS}
)

set(qtikz_FORMS MainWindow.ui)
QT4_WRAP_UI(qtikz_FORMS_HEADERS ${qtikz_FORMS})

add_executable(qtikz
    ${qtikz_SOURCES}
    ${qtikz_HEADERS_MOC}
    ${qtikz_FORMS_HEADERS}
)

target_link_libraries(qtikz
    ${QT_LIBRARIES}
    #${POPPLER_QT4_LIBRARIES}
    qtikzcore
    qtikzgui
)

# kate: indent-width 4; replace-tabs on;
