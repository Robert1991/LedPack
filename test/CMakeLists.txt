add_executable(LightShow-test
  LightShow-test.cpp
)

target_link_libraries(LightShow-test
  gtest_main
  gmock_main
  ledHeart
  shiftRegister
  arduinoWrapper
  lightShow
)

add_test(
  NAME
    LightShow-test
  COMMAND
    ${CMAKE_BINARY_DIR}/test/LightShow-test
  )

add_executable(LightShowEffects-test
  LightShowEffects-test.cpp
)

target_link_libraries(LightShowEffects-test
  gtest_main
  gmock_main
  ledHeart
  shiftRegister
  arduinoWrapper
  lightShow
)

add_test(
  NAME
  LightShowEffects-test
  COMMAND
    ${CMAKE_BINARY_DIR}/test/LightShowEffects-test
  ) 

add_executable(Gyroscope-test
  Gyroscope-test.cpp
)

target_link_libraries(Gyroscope-test
  gtest_main
  gmock_main
  gyroscope
  arduinoWrapper
)

add_test(
  NAME
    Gyroscope-test
  COMMAND
    ${CMAKE_BINARY_DIR}/test/Gyroscope-test
  )

add_executable(LedHeart-test
  LedHeart-test.cpp
)

target_link_libraries(LedHeart-test
  gtest_main
  gmock_main
  ledHeart
  shiftRegister
  arduinoWrapper
)

add_test(
  NAME
    LedHeart-test
  COMMAND
    ${CMAKE_BINARY_DIR}/test/LedHeart-test
  )

add_executable(LedHeart-ColumnActivator-test
  LedHeart-ColumnActivator-test.cpp
)

target_link_libraries(LedHeart-ColumnActivator-test
  gtest_main
  gmock_main
  ledHeart
  shiftRegister
  arduinoWrapper
)

add_test(
  NAME
    LedHeart-ColumnActivator-test
  COMMAND
    ${CMAKE_BINARY_DIR}/test/LedHeart-ColumnActivator-test
  )


add_executable(LedHeart-LevelActivator-test
  LedHeart-LevelActivator-test.cpp
)

target_link_libraries(LedHeart-LevelActivator-test
  gtest_main
  gmock_main
  ledHeart
  shiftRegister
  arduinoWrapper
)

add_test(
  NAME
    LedHeart-LevelActivator-test
  COMMAND
    ${CMAKE_BINARY_DIR}/test/LedHeart-LevelActivator-test
  )

add_executable(Microphone-test
  Microphone-test.cpp
)

target_link_libraries(Microphone-test
  gtest_main
  gmock_main
  microphone
  arduinoWrapper
)

add_test(
  NAME
    Microphone-test
  COMMAND
    ${CMAKE_BINARY_DIR}/test/Microphone-test
  )

add_executable(MovementShow-test
  MovementShow-test.cpp
)

target_link_libraries(MovementShow-test
  gtest_main
  gmock_main
  gyroscope
  movementShow
  ledHeart
  shiftRegister
  arduinoWrapper
)

add_test(
  NAME
    MovementShow-test
  COMMAND
    ${CMAKE_BINARY_DIR}/test/MovementShow-test
  )

add_executable(ShiftRegister-test
  ShiftRegister-test.cpp
)

target_link_libraries(ShiftRegister-test
  gtest_main
  gmock_main
  shiftRegister
  arduinoWrapper
)

add_test(
  NAME
    ShiftRegister-test
  COMMAND
    ${CMAKE_BINARY_DIR}/test/ShiftRegister-test
  )

add_executable(LowPassSampler-test
  LowPassSampler-test.cpp
)

target_link_libraries(LowPassSampler-test
  gtest_main
  gmock_main
  lowPassFilter
  lowPassSampler
  microphone
  arduinoWrapper
)

add_test(
  NAME
    LowPassSampler-test
  COMMAND
    ${CMAKE_BINARY_DIR}/test/LowPassSampler-test
  )