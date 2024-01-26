cmake_minimum_required(VERSION 3.16)

project(StorytellerEditor)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(VENDOR_DIR "${CMAKE_SOURCE_DIR}/vendor")
set(SOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/dialogs.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/dialogs.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/editor_application.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/editor_application.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/window.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/window.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/editor_ui.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/editor_ui.cpp"
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
	PRIVATE STORYTELLER_EDITOR_FONT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/fonts"
)

target_include_directories(${PROJECT_NAME}
	PRIVATE "${VENDOR_DIR}/glfw/include"
	PRIVATE "${VENDOR_DIR}/imgui"
	PRIVATE "${VENDOR_DIR}/spdlog/include"
)

target_link_libraries(${PROJECT_NAME}
	PRIVATE glfw
    PRIVATE StorytellerEngine
)

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Storyteller")