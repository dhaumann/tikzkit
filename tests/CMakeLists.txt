find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})
add_definitions(
    ${QT_DEFINITIONS}
)
set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )

include_directories(
  ${CMAKE_SOURCE_DIR}/qtikzcore
  ${CMAKE_SOURCE_DIR}/qtikzcore/style
  ${CMAKE_SOURCE_DIR}/qtikzcore/node
  ${CMAKE_SOURCE_DIR}/qtikzcore/path
  ${CMAKE_SOURCE_DIR}/qtikzgui
  ${CMAKE_CURRENT_BINARY_DIR}
)

# Test: Coord
set(TestCoordSrc TestCoord.cpp)
QT4_AUTOMOC(${TestCoordSrc} TestCoord.h)
QT4_WRAP_CPP(TestCoordMocs)
add_executable(TestCoord ${TestCoordSrc} ${TestCoordMocs})
target_link_libraries(TestCoord ${QT_LIBRARIES} ${QT_QTTEST_LIBRARY} qtikzcore)
add_test(NAME TestCoord COMMAND TestCoord)

# Test: Node
set(TestNodeSrc TestNode.cpp)
QT4_AUTOMOC(${TestNodeSrc} TestNode.h)
QT4_WRAP_CPP(TestNodeMocs)
add_executable(TestNode ${TestNodeSrc} ${TestNodeMocs})
target_link_libraries(TestNode ${QT_LIBRARIES} ${QT_QTTEST_LIBRARY} qtikzcore)
add_test(NAME TestNode COMMAND TestNode)

# Test: MetaPos
set(TestMetaPosSrc TestMetaPos.cpp)
QT4_AUTOMOC(${TestMetaPosSrc} TestMetaPos.h)
QT4_WRAP_CPP(TestMetaPosMocs)
add_executable(TestMetaPos ${TestMetaPosSrc} ${TestMetaPosMocs})
target_link_libraries(TestMetaPos ${QT_LIBRARIES} ${QT_QTTEST_LIBRARY} qtikzcore)
add_test(NAME TestMetaPos COMMAND TestMetaPos)

# Document test
set(DocumentSrc documenttest.cpp)
QT4_AUTOMOC(${DocumentSrc} documenttest.h)
QT4_WRAP_CPP(DocumentMocs)
add_executable(DocumentTest ${DocumentSrc} ${DocumentMocs})
target_link_libraries(DocumentTest ${QT_LIBRARIES} ${QT_QTTEST_LIBRARY} qtikzcore)
add_test(NAME DocumentTest COMMAND DocumentTest)



# Edge test
set(EdgeSrc edgetest.cpp)
QT4_AUTOMOC(${EdgeSrc} edgetest.h)
QT4_WRAP_CPP(EdgeMocs)
add_executable(EdgeTest ${EdgeSrc} ${EdgeMocs})
target_link_libraries(EdgeTest ${QT_LIBRARIES} ${QT_QTTEST_LIBRARY} qtikzcore)
add_test(NAME EdgeTest COMMAND EdgeTest)



# style manager test
set(StyleManagerSrc stylemanagertest.cpp)
QT4_AUTOMOC(${StyleManagerSrc} stylemanagertest.h)
QT4_WRAP_CPP(StyleManagerMocs)
add_executable(StyleManagerTest ${StyleManagerSrc} ${StyleManagerMocs})
target_link_libraries(StyleManagerTest ${QT_LIBRARIES} ${QT_QTTEST_LIBRARY} qtikzcore)
add_test(NAME StyleManagerTest COMMAND StyleManagerTest)



# TikzDocument test
set(TikzDocumentSrc tikzdocumenttest.cpp)
QT4_AUTOMOC(${TikzDocumentSrc} tikzdocumenttest.h)
QT4_WRAP_CPP(${TikzDocumentSrc})
add_executable(TikzDocumentTest ${TikzDocumentSrc} ${TikzDocumentMocs})
target_link_libraries(TikzDocumentTest ${QT_LIBRARIES} ${QT_QTTEST_LIBRARY} qtikzcore qtikzgui)
add_test(NAME TikzDocumentTest COMMAND TikzDocumentTest)

# kate: indent-width 4; replace-tabs on;
