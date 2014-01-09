project(qtikzgui)

find_package(Qt4 COMPONENTS QtCore QtGui QtSvg QtOpenGL REQUIRED)

include_directories(
    .
    ${CMAKE_SOURCE_DIR}/qtikzcore
    ${CMAKE_SOURCE_DIR}/qtikzcore/node
    ${CMAKE_SOURCE_DIR}/qtikzcore/path
    ${CMAKE_SOURCE_DIR}/qtikzcore/path/ellipse
    ${CMAKE_SOURCE_DIR}/qtikzcore/style

    ${CMAKE_SOURCE_DIR}/qtikzgui/tools
    ${CMAKE_SOURCE_DIR}/qtikzgui/handle
    ${CMAKE_SOURCE_DIR}/qtikzgui/paths
    ${CMAKE_SOURCE_DIR}/qtikzgui/paths/ellipse
    ${CMAKE_SOURCE_DIR}/qtikzgui/shapes
    ${CMAKE_SOURCE_DIR}/qtikzgui/arrows
    ${CMAKE_SOURCE_DIR}/qtikzgui/tex
    ${CMAKE_SOURCE_DIR}/qtikzgui/widgets
)

set(qtikzgui_SOURCES
    TikzDocument.cpp
    TikzItem.cpp
    TikzNode.cpp
#     TikzEdge.cpp
#     TikzEdge_p.cpp
    TikzPath.cpp
    TikzPath_p.cpp

    tools/AbstractTool.cpp
    tools/ProxyTool.cpp
    tools/NodeTool.cpp
    tools/EllipseTool.cpp
    tools/SelectTool.cpp

    paths/AbstractTikzPath.cpp
    paths/ellipse/TikzEllipsePath.cpp

    handle/Handle.cpp
    handle/ResizeHandle.cpp
    handle/RotateHandle.cpp
    handle/MoveHandle.cpp

    TikzScene.cpp
    TikzView.cpp
    TikzRuler.cpp
    PaintHelper.cpp
    AnchorHandle.cpp
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
)

set(qtikzgui_HEADERS
    TikzDocument.h
    TikzDocument_p.h
    TikzItem.h
    TikzNode.h
#     TikzEdge.h
#     TikzEdge_p.h
    TikzPath.h

    tools/AbstractTool.h
    tools/ProxyTool.h
    tools/NodeTool.h
    tools/EllipseTool.h
    tools/SelectTool.h

    paths/AbstractTikzPath.h
    paths/ellipse/TikzEllipsePath.h

    handle/Handle.h
    handle/ResizeHandle.h
    handle/RotateHandle.h
    handle/MoveHandle.h

    TikzScene.h
    TikzView.h
    TikzRuler.h
    AnchorHandle.h
    CurveHandle.h
    NodeText_p.h
    TikzToolBox.h

    tex/TexGenerator.h
#    tex/PdfRenderer.h

    widgets/ArrowComboBox.h
)

QT4_WRAP_CPP(qtikzgui_HEADERS_MOC ${qtikzgui_HEADERS})

# add icon resource
set(qtikzgui_RESOURCES icons.qrc)
qt4_add_resources(qtikzgui_RESOURCES_RCC ${qtikzgui_RESOURCES})

include(${QT_USE_FILE})
add_definitions(
    -DMAKE_TIKZGUI
    ${QT_DEFINITIONS}
)

add_library(qtikzgui SHARED
    ${qtikzgui_SOURCES}
    ${qtikzgui_HEADERS_MOC}
    ${qtikzgui_RESOURCES_RCC}
)

target_link_libraries(qtikzgui
  ${QT_LIBRARIES}
  qtikzcore
)

# kate: indent-width 4; replace-tabs on;
