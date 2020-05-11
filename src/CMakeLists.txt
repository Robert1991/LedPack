add_library(microphone "")
add_library(shiftRegister "")
add_library(ledHeart "")
add_library(gyroscope "")
add_library(arduinoWrapper INTERFACE)

target_sources(
    microphone
    PRIVATE
        LedPack/microphone.cpp
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/LedPack/microphone.h
    )

target_include_directories(
    microphone
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
)

target_sources(
    gyroscope
    PRIVATE
        LedPack/gyroscope.cpp
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/LedPack/gyroscope.h
    )

target_include_directories(
    gyroscope
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
)

target_sources(
    shiftRegister
    PRIVATE
        LedPack/shiftRegister.cpp
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/LedPack/shiftRegister.h
    )

target_include_directories(
    shiftRegister
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
)

target_sources(
    ledHeart
    PRIVATE
        LedPack/ledHeart.cpp
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/LedPack/ledHeart.h
    )

target_include_directories(
    ledHeart
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
)

target_include_directories(arduinoWrapper INTERFACE ${CMAKE_CURRENT_LIST_DIR}/LedPack)