add_library(microphone "")
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


target_include_directories(arduinoWrapper INTERFACE ${CMAKE_CURRENT_LIST_DIR}/LedPack)