find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})
add_definitions(
    ${QT_DEFINITIONS}
)
set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )

include_directories(
  ${CMAKE_SOURCE_DIR}/src/qtikzcore
  ${CMAKE_SOURCE_DIR}/src/qtikzcore/style
  ${CMAKE_CURRENT_BINARY_DIR}
)

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

# kate: indent-width 4; replace-tabs on;
