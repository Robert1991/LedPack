add_library(microphone "")
add_library(shiftRegister "")
add_library(ledHeart "")
add_library(gyroscope "")
add_library(movementShow "")
add_library(lowPassSampler "")
add_library(lowPassFilter "")
add_library(lightShow "")
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

target_sources(
    movementShow
    PRIVATE
        LedPack/movementShow.cpp
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/LedPack/movementShow.h
    )

target_include_directories(
    movementShow
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
)

target_sources(
    lowPassSampler
    PRIVATE
        LedPack/lowPassSampler.cpp
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/LedPack/lowPassSampler.h
    )

target_include_directories(
    lowPassSampler
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
)

target_sources(
    lowPassFilter
    PRIVATE
        LedPack/lowPassFilter.cpp
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/LedPack/lowPassFilter.h
    )

target_include_directories(
    lowPassFilter
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
)

target_sources(
    lightShow
    PRIVATE
        LedPack/lightShow.cpp
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/LedPack/lightShow.h
    )

target_include_directories(
    lightShow
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
)

target_include_directories(arduinoWrapper INTERFACE ${CMAKE_CURRENT_LIST_DIR}/LedPack)