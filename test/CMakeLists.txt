add_executable(
    unit_tests
    Microphone-test.cpp
)

target_link_libraries(
    unit_tests
    gtest_main
    gmock_main
    microphone
    arduinoWrapper
)

add_test(
  NAME
    unit
  COMMAND
    ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/unit_tests
  )