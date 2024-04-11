cmake_minimum_required(VERSION 3.16)

project(StorytellerRuntime)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(VENDOR_DIR "${CMAKE_SOURCE_DIR}/vendor")
set(SOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/console_manager.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/console_manager.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/game_controller.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/game_controller.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/runtime_application.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/runtime_application.cpp"
)

add_executable(${PROJECT_NAME}
    ${SOURCE_FILES}
)

target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:GNU>:-std=c++20 -fno-char8_t>
    $<$<CXX_COMPILER_ID:Clang>:-std=c++20 -fno-char8_t>
    $<$<CXX_COMPILER_ID:MSVC>:-std:c++20 /Zc:char8_t- /Zc:preprocessor>
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE STRTLR_TR_DOMAIN_RUNTIME=\"StorytellerRuntime\"
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE StorytellerEngine
)

if(MSVC)
    target_link_options(${PROJECT_NAME} PRIVATE $<$<CONFIG:RELWITHDEBINFO>:/PROFILE>)
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER Storyteller)
