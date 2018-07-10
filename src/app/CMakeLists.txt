project(tikzkit)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# find Qt5 modules
find_package(Qt5 "5.2.0" NO_MODULE REQUIRED Core Widgets Svg)

#find_package(PopplerQt4 REQUIRED)

include_directories(
    .
    ${CMAKE_SOURCE_DIR}/src/include
    #${POPPLER_QT4_INCLUDE_DIR}
#     ${CMAKE_CURRENT_BINARY_DIR}
)

set(tikzkit_SOURCES
    main.cpp
    TikzKit.cpp
    DocumentManager.cpp
    ViewManager.cpp
    MainWindow.cpp
    PdfGenerator.cpp
)

set(tikzkit_FORMS
    # example.ui
)
qt5_wrap_ui(tikzkit_FORMS_HEADERS ${tikzkit_FORMS})

add_executable(tikzkit
    ${tikzkit_SOURCES}
    ${tikzkit_FORMS_HEADERS}
)

target_compile_features(tikzkit PRIVATE cxx_std_14)

target_link_libraries(tikzkit
    Qt5::Core
    Qt5::Widgets
    #${POPPLER_QT4_LIBRARIES}
    qtikzcore
    qtikzui
)

# kate: indent-width 4; replace-tabs on;
