set(tikzkitui_SOURCES
    TikzItem.cpp
    NodeItem.cpp
#     TikzEdge.cpp
#     TikzEdge_p.cpp

    include/Editor.cpp
    include/Application.cpp
    include/MainWindow.cpp
    include/Document.cpp
    include/View.cpp

    app/EditorPrivate.cpp

    document/DocumentPrivate.cpp

    tools/AbstractTool.cpp
    tools/ProxyTool.cpp
    tools/NodeTool.cpp
    tools/EllipseTool.cpp
    tools/LineTool.cpp
    tools/SelectTool.cpp

    paths/PathItem.cpp
    paths/EllipsePathItem.cpp
    paths/EdgePathItem.cpp

    handle/Handle.cpp
    handle/ResizeHandle.cpp
    handle/RotateHandle.cpp
    handle/MoveHandle.cpp
    handle/AnchorHandle.cpp

    view/ViewPrivate.cpp
    view/Renderer.cpp
    view/AnchorManager.cpp
    view/Ruler.cpp
    view/Grid.cpp
    view/ZoomController.cpp

    colors/ColorPalette.cpp
    colors/ColorWidget.cpp

    TikzScene.cpp
    Painter.cpp
    CurveHandle.cpp
    BezierCurve.cpp
    NodeText.cpp
    NodeText_p.cpp
    TikzToolBox.cpp

    shapes/AbstractShape.cpp
    shapes/RectShape.cpp
    shapes/CircleShape.cpp
    shapes/DiamondShape.cpp
    shapes/EllipseShape.cpp

    arrows/AbstractArrow.cpp
    arrows/ToArrow.cpp
    arrows/StealthArrow.cpp
    arrows/LatexArrow.cpp
    arrows/PipeArrow.cpp
    arrows/StealthTickArrow.cpp

    tex/TexGenerator.cpp
#    tex/PdfRenderer.cpp

    widgets/ArrowComboBox.cpp
    widgets/IconComboBox.cpp
    widgets/SliderSpinBox.cpp
    widgets/SliderDoubleSpinBox.cpp
    widgets/ToolLayout.cpp

    propertybrowser/PropertyBrowser.cpp
    propertybrowser/ValuePropertyManager.cpp
    propertybrowser/ValueSpinBoxFactory.cpp
    propertybrowser/ValueSpinBox.cpp
    propertybrowser/OpacityPropertyManager.cpp
    propertybrowser/OpacityEditorFactory.cpp
    propertybrowser/OpacityEditor.cpp
    propertybrowser/UidPropertyManager.cpp
)

set(tikzkitui_FORMS )
qt5_wrap_ui(tikzkitui_FORMS_HEADERS ${tikzkitui_FORMS})

# add icon resource
set(tikzkitui_RESOURCES icons.qrc)
qt5_add_resources(tikzkitui_RESOURCES_RCC ${tikzkitui_RESOURCES})

add_library(tikzkitui SHARED
    ${tikzkitui_SOURCES}
    ${tikzkitui_RESOURCES_RCC}
    ${tikzkitui_FORMS_HEADERS}
)

target_compile_features(tikzkitui PRIVATE cxx_std_14)
target_compile_options(tikzkitui PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)

target_link_libraries(tikzkitui
    Qt5::Core
    Qt5::Widgets
    Qt5::Svg
    tikzkitcore
    qtpropertybrowser
)

target_include_directories(tikzkitui
    PRIVATE
    .
    ${CMAKE_BINARY_DIR}/include

    ${CMAKE_SOURCE_DIR}/src/ui/tools
    ${CMAKE_SOURCE_DIR}/src/ui/handle
    ${CMAKE_SOURCE_DIR}/src/ui/paths
    ${CMAKE_SOURCE_DIR}/src/ui/shapes
    ${CMAKE_SOURCE_DIR}/src/ui/arrows
    ${CMAKE_SOURCE_DIR}/src/ui/tex
    ${CMAKE_SOURCE_DIR}/src/ui/widgets
    ${CMAKE_SOURCE_DIR}/src/ui/view
    ${CMAKE_SOURCE_DIR}/src/ui/colors
    ${CMAKE_SOURCE_DIR}/src/ui/propertybrowser
    ${CMAKE_SOURCE_DIR}/src/ui/include
    ${CMAKE_SOURCE_DIR}/src/ui/app
    ${CMAKE_SOURCE_DIR}/src/ui/document

    ${CMAKE_SOURCE_DIR}/src/qtpropertybrowser/src
)

# kate: indent-width 4; replace-tabs on;
