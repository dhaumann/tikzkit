set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )

include_directories(
  ${CMAKE_SOURCE_DIR}/src
  ${CMAKE_SOURCE_DIR}/src/style
  ${CMAKE_SOURCE_DIR}/src/tests
  ${CMAKE_CURRENT_BINARY_DIR}
)

set(StyleManagerSrc stylemanagertest.cpp)
QT4_AUTOMOC(${StyleManagerSrc} stylemanagertest.h)
QT4_WRAP_CPP(StyleManagerMocs)
add_executable(StyleManagerTest ${StyleManagerSrc} ${StyleManagerMocs})
target_link_libraries(StyleManagerTest ${QT_LIBRARIES} ${QT_QTTEST_LIBRARY})
add_test(NAME StyleManagerTest COMMAND StyleManagerTest)
