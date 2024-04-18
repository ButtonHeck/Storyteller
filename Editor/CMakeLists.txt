cmake_minimum_required(VERSION 3.16)

project(StorytellerEditor LANGUAGES CXX)

set(VENDOR_DIR "${CMAKE_SOURCE_DIR}/vendor")
set(SOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/editor_application.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/editor_application.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/icons_font.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/editor_ui.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/editor_ui.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/editor_ui_impl.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/editor_ui_impl.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/editor_ui_impl_opengl_glfw.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/editor_ui_impl_opengl_glfw.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/editor_ui_compositor.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/editor_ui_compositor.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/ui_utils.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/ui_utils.cpp"
)

set(IMGUI_IMPL_DIR "${VENDOR_DIR}/imgui/backends")
aux_source_directory("${VENDOR_DIR}/imgui" IMGUI_SOURCE_FILES)

add_executable(${PROJECT_NAME} WIN32
    ${SOURCE_FILES}
    ${IMGUI_SOURCE_FILES}
    ${IMGUI_IMPL_DIR}/imgui_impl_opengl3.cpp
    ${IMGUI_IMPL_DIR}/imgui_impl_glfw.cpp
    ${VENDOR_DIR}/imgui/misc/cpp/imgui_stdlib.cpp
)

source_group("Imgui Sources" FILES 
    ${IMGUI_SOURCE_FILES}
    ${IMGUI_IMPL_DIR}/imgui_impl_opengl3.cpp
    ${IMGUI_IMPL_DIR}/imgui_impl_glfw.cpp
    ${VENDOR_DIR}/imgui/misc/cpp/imgui_stdlib.cpp
)


include(CheckCXXCompilerFlag)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    check_cxx_compiler_flag(-std=c++20 CXX_20_OK)
    check_cxx_compiler_flag(-fno-char8_t CXX_NO_CHAR8_OK)
    if(NOT CXX_20_OK OR NOT CXX_NO_CHAR8_OK)
        message(ERROR "GNU compiler failed to check necessary flags support!")
    else()
        message(STATUS "GNU compiler check necessary flags OK")
    endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    check_cxx_compiler_flag(-std=c++20 CXX_20_OK)
    check_cxx_compiler_flag(-fno-char8_t CXX_NO_CHAR8_OK)
    if(NOT CXX_20_OK OR NOT CXX_NO_CHAR8_OK)
        message(ERROR "Clang compiler failed to check necessary compiler flags support!")
    else()
        message(STATUS "Clang compiler check necessary flags OK")
    endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    check_cxx_compiler_flag(-std:c++20 CXX_20_OK)
    check_cxx_compiler_flag(/Zc:char8_t- CXX_NO_CHAR8_OK)
    check_cxx_compiler_flag(/Zc:preprocessor CXX_PREPROCESSOR_OK)
    if(NOT CXX_20_OK OR NOT CXX_NO_CHAR8_OK OR NOT CXX_PREPROCESSOR_OK)
        message(ERROR "MSVC compiler failed to check necessary compiler flags support!")
    else()
        message(STATUS "MSVC compiler check necessary flags OK")
    endif()
else()
    message(WARNING "Cannot check compiler flags for: ${CMAKE_CXX_COMPILER_ID}")
endif()

target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:GNU>:-std=c++20 -fno-char8_t>
    $<$<CXX_COMPILER_ID:Clang>:-std=c++20 -fno-char8_t>
    $<$<CXX_COMPILER_ID:MSVC>:-std:c++20 /Zc:char8_t- /Zc:preprocessor>
)


target_compile_definitions(${PROJECT_NAME} 
    PRIVATE STRTLR_EDITOR_FONT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/fonts"
    PRIVATE STRTLR_TR_DOMAIN_EDITOR=\"StorytellerEditor\"
)

if (WIN32)
    target_compile_definitions(${PROJECT_NAME}
        PRIVATE STRTLR_WINMAIN
    )
endif()

target_include_directories(${PROJECT_NAME}
    PRIVATE "${VENDOR_DIR}/imgui"
    PRIVATE "${VENDOR_DIR}/spdlog/include"
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE StorytellerEngine
)

if(MSVC)
    target_link_options(${PROJECT_NAME} PRIVATE $<$<CONFIG:RELWITHDEBINFO>:/PROFILE>)
    set_property(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
    set_property(DIRECTORY ${CMAKE_BINARY_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
	set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/exe/$<CONFIG>)
endif()

set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/exe/$<CONFIG>)
set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY FOLDER Storyteller)

get_target_property(exeDir ${PROJECT_NAME} RUNTIME_OUTPUT_DIRECTORY)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} "-DDEST_DIR=${exeDir}" "-DSRC_DIRS=${CMAKE_CURRENT_LIST_DIR}/locale;${CMAKE_CURRENT_LIST_DIR}/../Engine/locale" 
	-P "${CMAKE_SOURCE_DIR}/cmake/copy_translations.cmake"
)
