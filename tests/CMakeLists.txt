# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# find Qt5 modules
find_package(Qt5 "5.2.0" NO_MODULE REQUIRED Core Widgets Test Svg)

# set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})

include_directories(
  ${CMAKE_BINARY_DIR}/include
  ${CMAKE_CURRENT_BINARY_DIR}
)

# Test: Node
set(TestNodeSrc TestNode.cpp)
add_executable(TestNode ${TestNodeSrc})
target_link_libraries(TestNode Qt5::Core Qt5::Test tikzkitcore)
add_test(NAME TestNode COMMAND TestNode)

# Test: MetaPos
set(TestMetaPosSrc TestMetaPos.cpp)
add_executable(TestMetaPos ${TestMetaPosSrc})
target_link_libraries(TestMetaPos Qt5::Core Qt5::Test tikzkitcore)
add_test(NAME TestMetaPos COMMAND TestMetaPos)

# Test: Value
set(TestValueSrc TestValue.cpp)
add_executable(TestValue ${TestValueSrc})
target_link_libraries(TestValue Qt5::Core Qt5::Test tikzkitcore)
add_test(NAME TestValue COMMAND TestValue)

# Test: Pos
set(TestPosSrc TestPos.cpp)
add_executable(TestPos ${TestPosSrc})
target_link_libraries(TestPos Qt5::Core Qt5::Test tikzkitcore)
add_test(NAME TestPos COMMAND TestPos)

# Document test
set(DocumentSrc documenttest.cpp)
add_executable(DocumentTest ${DocumentSrc})
target_link_libraries(DocumentTest Qt5::Core Qt5::Test tikzkitcore)
add_test(NAME DocumentTest COMMAND DocumentTest)

# Edge test
set(EdgeSrc edgetest.cpp)
add_executable(EdgeTest ${EdgeSrc})
target_link_libraries(EdgeTest Qt5::Core Qt5::Test tikzkitcore)
add_test(NAME EdgeTest COMMAND EdgeTest)

# TikzDocument test
set(TikzDocumentSrc tikzdocumenttest.cpp)
add_executable(TikzDocumentTest ${TikzDocumentSrc})
target_link_libraries(TikzDocumentTest Qt5::Core Qt5::Test tikzkitcore tikzkitui)
add_test(NAME TikzDocumentTest COMMAND TikzDocumentTest)

# kate: indent-width 4; replace-tabs on;
