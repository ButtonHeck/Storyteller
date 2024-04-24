cmake_minimum_required(VERSION 3.20)

project(StorytellerRuntime LANGUAGES CXX)

set(VENDOR_DIR "${CMAKE_SOURCE_DIR}/vendor")
set(SOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/console_manager.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/console_manager.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/game_controller.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/game_controller.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/runtime_application.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/runtime_application.cpp"
)

if(WIN32)
	set(STRTLR_WIN_ICO_PATH "${CMAKE_SOURCE_DIR}/common/assets/icons/strtlr64.ico")
	configure_file("${CMAKE_CURRENT_SOURCE_DIR}/src/win/StorytellerRuntime.rc_in" "${CMAKE_CURRENT_BINARY_DIR}/StorytellerRuntime.rc")
	configure_file("${CMAKE_CURRENT_SOURCE_DIR}/src/win/resource.h" "${CMAKE_CURRENT_BINARY_DIR}/resource.h" COPYONLY)
	set(WIN_RESOURCE_FILES
		"${CMAKE_CURRENT_BINARY_DIR}/resource.h"
		"${CMAKE_CURRENT_BINARY_DIR}/StorytellerRuntime.rc"
	)
	source_group("Resources" FILES ${WIN_RESOURCE_FILES})
endif()

add_executable(${PROJECT_NAME}
    ${SOURCE_FILES}
	${WIN_RESOURCE_FILES}
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
    set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/exe/$<CONFIG>)
endif()

set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/exe/$<CONFIG>)
set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY FOLDER Storyteller/Runtime)

get_target_property(exeDir ${PROJECT_NAME} RUNTIME_OUTPUT_DIRECTORY)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND}
    "-D DEST_DIR=${exeDir}" "-D SRC_DIRS=${CMAKE_CURRENT_LIST_DIR}/locale;${CMAKE_CURRENT_LIST_DIR}/../Engine/locale" 
	-P "${CMAKE_SOURCE_DIR}/cmake/copy_translations.cmake"
)

CreateTranslationHelperTargets("StorytellerRuntime" "StorytellerRuntime" ${CMAKE_CURRENT_SOURCE_DIR} Storyteller/Runtime ${SOURCE_FILES})