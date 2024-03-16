cmake_minimum_required(VERSION 3.16)

project(StorytellerEditor)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std:c++20 /Zc:char8_t-")
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

add_executable(${PROJECT_NAME}
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

target_compile_definitions(${PROJECT_NAME} 
	PRIVATE STRTLR_EDITOR_FONT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/fonts"
    PRIVATE STRTLR_TR_DOMAIN_EDITOR=\"StorytellerEditor\"
)

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
endif()

add_custom_target(copy_translations_to_editor
	COMMAND ${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/cmake/copy_translations_to_editor.cmake
)
add_dependencies(${PROJECT_NAME} copy_translations_to_editor)

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER Storyteller)
set_target_properties(copy_translations_to_editor PROPERTIES FOLDER Storyteller)
