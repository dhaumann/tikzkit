project(qtikzui)

find_package(Qt4 COMPONENTS QtCore QtGui QtSvg QtOpenGL REQUIRED)

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
)

set(qtikzui_SOURCES
    TikzDocument.cpp
    TikzItem.cpp
    TikzNode.cpp
#     TikzEdge.cpp
#     TikzEdge_p.cpp

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

    TikzScene.cpp
    TikzView.cpp
    PaintHelper.cpp
    CurveHandle.cpp
    BezierCurve.cpp
    NodeText.cpp
    NodeText_p.cpp
    TikzToolBox.cpp

    shapes/AbstractShape.cpp
    shapes/RectShape.cpp
    shapes/CircleShape.cpp
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
)

set(qtikzui_HEADERS
    TikzDocument.h
    TikzDocument_p.h
    TikzItem.h
    TikzNode.h
#     TikzEdge.h
#     TikzEdge_p.h

    tools/AbstractTool.h
    tools/ProxyTool.h
    tools/NodeTool.h
    tools/EllipseTool.h
    tools/LineTool.h
    tools/SelectTool.h

    paths/PathItem.h
    paths/EllipsePathItem.h
    paths/EdgePathItem.h

    handle/Handle.h
    handle/ResizeHandle.h
    handle/RotateHandle.h
    handle/MoveHandle.h
    handle/AnchorHandle.h

    utils/AnchorManager.h
    utils/Ruler.h

    TikzScene.h
    TikzView.h
    CurveHandle.h
    NodeText_p.h
    TikzToolBox.h

    tex/TexGenerator.h
#    tex/PdfRenderer.h

    widgets/ArrowComboBox.h
    widgets/IconComboBox.h
)

QT4_WRAP_CPP(qtikzui_HEADERS_MOC ${qtikzui_HEADERS})

# add icon resource
set(qtikzui_RESOURCES icons.qrc)
qt4_add_resources(qtikzui_RESOURCES_RCC ${qtikzui_RESOURCES})

include(${QT_USE_FILE})
add_definitions(
    -DMAKE_TIKZUI
    ${QT_DEFINITIONS}
)

add_library(qtikzui SHARED
    ${qtikzui_SOURCES}
    ${qtikzui_HEADERS_MOC}
    ${qtikzui_RESOURCES_RCC}
)

target_link_libraries(qtikzui
  ${QT_LIBRARIES}
  qtikzcore
)

# kate: indent-width 4; replace-tabs on;
