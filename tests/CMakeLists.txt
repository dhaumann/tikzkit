# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# find Qt5 modules
find_package(Qt5 "5.2.0" NO_MODULE REQUIRED Core Widgets Test)

# set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})

include_directories(
  ${CMAKE_SOURCE_DIR}/src/include
  ${CMAKE_CURRENT_BINARY_DIR}
)

# Test: Node
set(TestNodeSrc TestNode.cpp)
add_executable(TestNode ${TestNodeSrc})
target_link_libraries(TestNode Qt5::Core Qt5::Test qtikzcore)
add_test(NAME TestNode COMMAND TestNode)

# Test: MetaPos
set(TestMetaPosSrc TestMetaPos.cpp)
add_executable(TestMetaPos ${TestMetaPosSrc})
target_link_libraries(TestMetaPos Qt5::Core Qt5::Test qtikzcore)
add_test(NAME TestMetaPos COMMAND TestMetaPos)

# Document test
set(DocumentSrc documenttest.cpp)
add_executable(DocumentTest ${DocumentSrc})
target_link_libraries(DocumentTest Qt5::Core Qt5::Test qtikzcore)
add_test(NAME DocumentTest COMMAND DocumentTest)

# Edge test
set(EdgeSrc edgetest.cpp)
add_executable(EdgeTest ${EdgeSrc})
target_link_libraries(EdgeTest Qt5::Core Qt5::Test qtikzcore)
add_test(NAME EdgeTest COMMAND EdgeTest)

# TikzDocument test
set(TikzDocumentSrc tikzdocumenttest.cpp)
add_executable(TikzDocumentTest ${TikzDocumentSrc})
target_link_libraries(TikzDocumentTest Qt5::Core Qt5::Test qtikzcore qtikzui)
add_test(NAME TikzDocumentTest COMMAND TikzDocumentTest)

# kate: indent-width 4; replace-tabs on;
