add_executable(tikzkit
    main.cpp
    TikzKit.cpp
    DocumentManager.cpp
    ViewManager.cpp
    MainWindow.cpp
    PdfGenerator.cpp
)

target_compile_features(tikzkit PRIVATE cxx_std_14)
target_compile_options(tikzkit PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)

target_include_directories(tikzkit
    PRIVATE
    .
    ${CMAKE_BINARY_DIR}/include
)

target_link_libraries(tikzkit
    Qt5::Core
    Qt5::Widgets
    tikzkitcore
    tikzkitui
)

# kate: indent-width 4; replace-tabs on;
