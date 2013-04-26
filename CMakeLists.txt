cmake_minimum_required(VERSION 2.8)

PROJECT(tikzkit)
FIND_PACKAGE(Qt4 REQUIRED)

INCLUDE_DIRECTORIES(
    src
    src/style
)

SET(tikzkit_SOURCES
    src/main.cpp
    src/Node.cpp
    src/Coord.cpp
    src/style/Style.cpp
    src/style/StyleManager.cpp
)

SET(tikzkit_HEADERS
#    TkMainWindow.h
)

QT4_WRAP_CPP(tikzkit_HEADERS_MOC ${tikzkit_HEADERS})

INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})

ADD_EXECUTABLE(tikzkit
    ${tikzkit_SOURCES}
    ${tikzkit_HEADERS_MOC}
)

TARGET_LINK_LIBRARIES(tikzkit ${QT_LIBRARIES})

# kate: indent-width 4; replace-tabs on;
