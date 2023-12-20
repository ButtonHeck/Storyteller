cmake_minimum_required(VERSION 3.16)

project(Storyteller)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------------------------------------------------------------------------------------
# Vendor
set(STORYTELLER_VENDOR_DIR "${PROJECT_SOURCE_DIR}/vendor")
set(IMGUI_IMPL_DIR "${STORYTELLER_VENDOR_DIR}/imgui/backends")
aux_source_directory("${STORYTELLER_VENDOR_DIR}/imgui" IMGUI_SOURCE_FILES)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(RAPIDJSON_BUILD_CXX11 OFF CACHE BOOL "" FORCE)
set(RAPIDJSON_BUILD_CXX17 ON CACHE BOOL "" FORCE)
set(RAPIDJSON_BUILD_DOC OFF CACHE BOOL "" FORCE)
set(RAPIDJSON_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(RAPIDJSON_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(Boost_NO_SYSTEM_PATHS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_STATIC_RUNTIME OFF)
set(BOOST_ALL_DYN_LINK OFF)

find_package (Boost REQUIRED COMPONENTS locale REQUIRED) 

function(get_all_targets result dir)
    get_property(subdirs DIRECTORY "${dir}" PROPERTY SUBDIRECTORIES)
    foreach(subdir IN LISTS subdirs)
        get_all_targets(${result} "${subdir}")
    endforeach()
    get_property(sub_targets DIRECTORY "${dir}" PROPERTY BUILDSYSTEM_TARGETS)
    set(${result} ${${result}} ${sub_targets} PARENT_SCOPE)
endfunction()

function(add_subdirectory_with_folder subdirectory folder_name)
    add_subdirectory(${subdirectory} ${ARGN})
    get_all_targets(targets "${subdirectory}")
    foreach(target IN LISTS targets)
        set_target_properties(${target} PROPERTIES FOLDER "${folder_name}")
    endforeach()
endfunction()

add_subdirectory_with_folder("${STORYTELLER_VENDOR_DIR}/glfw" "vendor")
add_subdirectory_with_folder("${STORYTELLER_VENDOR_DIR}/rapidjson" "vendor")
add_subdirectory_with_folder("${STORYTELLER_VENDOR_DIR}/spdlog" "vendor")


# -----------------------------------------------------------------------------------
# Engine
set(STORYTELLER_ENGINE "StorytellerEngine")
set(STORYTELLER_ENGINE_SOURCE_DIR "Engine/src")
set(STORYTELLER_ENGINE_SOURCE_FILES
    "${STORYTELLER_ENGINE_SOURCE_DIR}/storyteller.h"
    "${STORYTELLER_ENGINE_SOURCE_DIR}/uuid.h"
    "${STORYTELLER_ENGINE_SOURCE_DIR}/uuid.cpp"
    "${STORYTELLER_ENGINE_SOURCE_DIR}/entities.h"
    "${STORYTELLER_ENGINE_SOURCE_DIR}/entities.cpp"
    "${STORYTELLER_ENGINE_SOURCE_DIR}/game_document.h"
    "${STORYTELLER_ENGINE_SOURCE_DIR}/game_document.cpp"
    "${STORYTELLER_ENGINE_SOURCE_DIR}/game_document_sort_filter_proxy_view.h"
    "${STORYTELLER_ENGINE_SOURCE_DIR}/game_document_sort_filter_proxy_view.cpp"
    "${STORYTELLER_ENGINE_SOURCE_DIR}/game_controller.h"
    "${STORYTELLER_ENGINE_SOURCE_DIR}/game_controller.cpp"
    "${STORYTELLER_ENGINE_SOURCE_DIR}/localization_manager.h"
    "${STORYTELLER_ENGINE_SOURCE_DIR}/localization_manager.cpp"
    "${STORYTELLER_ENGINE_SOURCE_DIR}/console_manager.h"
    "${STORYTELLER_ENGINE_SOURCE_DIR}/console_manager.cpp"
)

add_library(${STORYTELLER_ENGINE}
    ${STORYTELLER_ENGINE_SOURCE_FILES}
)

target_include_directories(${STORYTELLER_ENGINE}
    PRIVATE ${STORYTELLER_ENGINE_SOURCE_DIR}
    PRIVATE "${STORYTELLER_VENDOR_DIR}/rapidjson/include"
)


# -----------------------------------------------------------------------------------
# Editor
set(STORYTELLER_EDITOR "StorytellerEditor")
set(STORYTELLER_EDITOR_SOURCE_DIR "Editor/src")
set(STORYTELLER_EDITOR_SOURCE_FILES
	"${STORYTELLER_EDITOR_SOURCE_DIR}/main.cpp"
    "${STORYTELLER_EDITOR_SOURCE_DIR}/glfw_helpers.h"
    "${STORYTELLER_EDITOR_SOURCE_DIR}/glfw_helpers.cpp"
    "${STORYTELLER_EDITOR_SOURCE_DIR}/imgui_helpers.h"
    "${STORYTELLER_EDITOR_SOURCE_DIR}/imgui_helpers.cpp"
    "${STORYTELLER_EDITOR_SOURCE_DIR}/file_dialogs.h"
    "${STORYTELLER_EDITOR_SOURCE_DIR}/file_dialogs.cpp"
)

add_executable(${STORYTELLER_EDITOR}
	${STORYTELLER_EDITOR_SOURCE_FILES}
	${IMGUI_SOURCE_FILES}
	${IMGUI_IMPL_DIR}/imgui_impl_opengl3.cpp
    ${IMGUI_IMPL_DIR}/imgui_impl_glfw.cpp
    ${STORYTELLER_VENDOR_DIR}/imgui/misc/cpp/imgui_stdlib.cpp
)

source_group("Imgui Sources" FILES 
    ${IMGUI_SOURCE_FILES}
    ${IMGUI_IMPL_DIR}/imgui_impl_opengl3.cpp
    ${IMGUI_IMPL_DIR}/imgui_impl_glfw.cpp
    ${STORYTELLER_VENDOR_DIR}/imgui/misc/cpp/imgui_stdlib.cpp
)

add_compile_definitions(STORYTELLER_EDITOR_FONT_DIR="${PROJECT_SOURCE_DIR}/Editor/fonts")

target_include_directories(${STORYTELLER_EDITOR}
    PRIVATE "${STORYTELLER_ENGINE_SOURCE_DIR}"
	PRIVATE "${STORYTELLER_VENDOR_DIR}/glfw/include"
	PRIVATE "${STORYTELLER_VENDOR_DIR}/imgui"
	PRIVATE "${STORYTELLER_VENDOR_DIR}/spdlog/include"
)

target_link_libraries(${STORYTELLER_EDITOR}
	PRIVATE glfw
    PRIVATE ${STORYTELLER_ENGINE}
)

if (Boost_FOUND)
    include_directories(SYSTEM ${Boost_INCLUDE_DIR})
    target_link_libraries(${STORYTELLER_EDITOR} PRIVATE ${Boost_LIBRARIES})
endif ()


# -----------------------------------------------------------------------------------
set_target_properties(${STORYTELLER_ENGINE} PROPERTIES FOLDER "Storyteller")
set_target_properties(${STORYTELLER_EDITOR} PROPERTIES FOLDER "Storyteller")
