project(qtikzgui)

find_package(Qt4 COMPONENTS QtCore QtGui QtSvg QtOpenGL REQUIRED)
#find_package(PopplerQt4 REQUIRED)

include_directories(
    .
    ${CMAKE_SOURCE_DIR}/src/qtikzcore
    ${CMAKE_SOURCE_DIR}/src/qtikzgui/shapes
    ${CMAKE_SOURCE_DIR}/src/qtikzgui/arrows
    ${CMAKE_SOURCE_DIR}/src/qtikzgui/tex
#    ${POPPLER_QT4_INCLUDE_DIR}
)

set(qtikzgui_SOURCES
    TikzItem.cpp
    TikzNode.cpp
    NodeHandle.cpp
    TikzEdge.cpp
    TikzEdge_p.cpp
    TikzScene.cpp
    PaintHelper.cpp
    AnchorHandle.cpp
    CurveHandle.cpp
    BezierCurve.cpp
    NodeText.cpp
    NodeText_p.cpp

    shapes/AbstractShape.cpp
    shapes/RectShape.cpp
    shapes/CircleShape.cpp
    shapes/EllipseShape.cpp

    arrows/AbstractArrow.cpp
    arrows/ToArrow.cpp
    arrows/StealthArrow.cpp

    tex/TexGenerator.cpp
#    tex/PdfRenderer.cpp
)

set(qtikzgui_HEADERS
    TikzItem.h
    TikzNode.h
    NodeHandle.h
    TikzEdge.h
    TikzScene.h
    AnchorHandle.h
    CurveHandle.h
    NodeText_p.h

    tex/TexGenerator.h
#    tex/PdfRenderer.h
)

QT4_WRAP_CPP(qtikzgui_HEADERS_MOC ${qtikzgui_HEADERS})

include(${QT_USE_FILE})
add_definitions(
    -DMAKE_TIKZGUI
    ${QT_DEFINITIONS}
#    ${POPPLER_QT4_DEFINITIONS}
)

add_library(qtikzgui SHARED
    ${qtikzgui_SOURCES}
    ${qtikzgui_HEADERS_MOC}
)

target_link_libraries(qtikzgui
  ${QT_LIBRARIES}
#  ${POPPLER_QT4_LIBRARIES}
  qtikzcore
)

# kate: indent-width 4; replace-tabs on;
