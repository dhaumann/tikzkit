project(tikzkit)

find_package(Qt5 "5.2.0" NO_MODULE REQUIRED Core Widgets Svg)

add_executable(tikzkit
    main.cpp
    TikzKit.cpp
    DocumentManager.cpp
    ViewManager.cpp
    MainWindow.cpp
    PdfGenerator.cpp
)

target_compile_features(tikzkit PRIVATE cxx_std_14)
target_compile_options(tikzkit PRIVATE -Wall)

target_include_directories(tikzkit
    PRIVATE
    .
    ${CMAKE_SOURCE_DIR}/src/include
)

target_link_libraries(tikzkit
    Qt5::Core
    Qt5::Widgets
    qtikzcore
    qtikzui
)

# kate: indent-width 4; replace-tabs on;
