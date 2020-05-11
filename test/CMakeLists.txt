add_executable(
    unit_tests
    Gyroscope-test.cpp
    LedHeart-test.cpp
    LedHeart-LevelActivator-test.cpp
    LedHeart-ColumnActivator-test.cpp
    Microphone-test.cpp
    ShiftRegister-test.cpp
)

target_link_libraries(
    unit_tests
    gtest_main
    gmock_main
    microphone
    shiftRegister
    ledHeart
    arduinoWrapper
    gyroscope
)

add_test(
  NAME
    unit
  COMMAND
    ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/unit_tests
  )