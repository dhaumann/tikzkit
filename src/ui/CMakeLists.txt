project(qtikzui)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# find Qt5 modules
find_package(Qt5 "5.2.0" NO_MODULE REQUIRED Core Widgets Svg)


include_directories(
    .
    ${CMAKE_SOURCE_DIR}/src/include

    ${CMAKE_SOURCE_DIR}/src/ui/tools
    ${CMAKE_SOURCE_DIR}/src/ui/handle
    ${CMAKE_SOURCE_DIR}/src/ui/paths
    ${CMAKE_SOURCE_DIR}/src/ui/shapes
    ${CMAKE_SOURCE_DIR}/src/ui/arrows
    ${CMAKE_SOURCE_DIR}/src/ui/tex
    ${CMAKE_SOURCE_DIR}/src/ui/widgets
    ${CMAKE_SOURCE_DIR}/src/ui/utils
    ${CMAKE_SOURCE_DIR}/src/ui/colors
    ${CMAKE_SOURCE_DIR}/src/ui/propertybrowser
    ${CMAKE_SOURCE_DIR}/src/ui/include
    ${CMAKE_SOURCE_DIR}/src/ui/app
    ${CMAKE_SOURCE_DIR}/src/ui/document

    ${CMAKE_SOURCE_DIR}/src/qtpropertybrowser/src

#     ${CMAKE_CURRENT_BINARY_DIR}
)

set(qtikzui_SOURCES
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

    utils/AnchorManager.cpp
    utils/Ruler.cpp
    utils/Grid.cpp

    colors/ColorPalette.cpp
    colors/ColorWidget.cpp

    TikzScene.cpp
    TikzView.cpp
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
    widgets/LinePropertyWidget.cpp
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
)

set(qtikzui_FORMS forms/LinePropertyWidget.ui)
qt5_wrap_ui(qtikzui_FORMS_HEADERS ${qtikzui_FORMS})

# add icon resource
set(qtikzui_RESOURCES icons.qrc)
qt5_add_resources(qtikzui_RESOURCES_RCC ${qtikzui_RESOURCES})

add_library(qtikzui SHARED
    ${qtikzui_SOURCES}
    ${qtikzui_RESOURCES_RCC}
    ${qtikzui_FORMS_HEADERS}
)

target_link_libraries(qtikzui
    Qt5::Core
    Qt5::Widgets
    Qt5::Svg
    qtikzcore
    qtpropertybrowser
)

# kate: indent-width 4; replace-tabs on;
