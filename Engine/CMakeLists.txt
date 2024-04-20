cmake_minimum_required(VERSION 3.20)

project(StorytellerEngine LANGUAGES CXX)

set(VENDOR_DIR "${CMAKE_SOURCE_DIR}/vendor")
set(HEADER_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Storyteller/entry_point.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Storyteller/storyteller.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Storyteller/uuid.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Storyteller/entities.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Storyteller/game_document.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Storyteller/game_document_manager.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Storyteller/game_document_serializer.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Storyteller/game_document_sort_filter_proxy_view.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Storyteller/json_reader.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Storyteller/json_writer.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Storyteller/i18n_base.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Storyteller/i18n_manager.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Storyteller/i18n_translator.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Storyteller/i18n_library.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Storyteller/i18n_lookup_dictionary.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Storyteller/filesystem.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Storyteller/log.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Storyteller/application.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Storyteller/window_application.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Storyteller/settings.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Storyteller/pointers.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Storyteller/window.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Storyteller/window_glfw.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Storyteller/dialogs.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Storyteller/event.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Storyteller/window_event.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Storyteller/mouse_event.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Storyteller/mouse_codes.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Storyteller/key_codes.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Storyteller/key_event.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Storyteller/string_utils.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Storyteller/image.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Storyteller/platform.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Storyteller/console_utils.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Storyteller/function_utils.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Storyteller/config.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Storyteller/macro.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Storyteller/strtlr_assert.h"
)
set(SOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/uuid.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/entities.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/game_document.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/game_document_manager.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/game_document_serializer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/game_document_sort_filter_proxy_view.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/json_reader.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/json_writer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/i18n_manager.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/i18n_translator.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/i18n_library.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/i18n_lookup_dictionary.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/filesystem.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/log.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/application.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/window_application.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/settings.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/window.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/window_glfw.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/dialogs.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/image.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/string_utils.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/console_utils.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/config.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/strtlr_assert.cpp"
)

aux_source_directory(${VENDOR_DIR}/stb_image STB_IMAGE_SRC)

add_library(${PROJECT_NAME}
    ${HEADER_FILES}
    ${SOURCE_FILES}
    ${STB_IMAGE_SRC}
)


include(CheckCXXCompilerFlag)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    check_cxx_compiler_flag(-std=c++20 CXX_20_OK)
    check_cxx_compiler_flag(-fno-char8_t CXX_NO_CHAR8_OK)
    if(NOT CXX_20_OK OR NOT CXX_NO_CHAR8_OK)
        message(SEND_ERROR "GNU compiler failed to check necessary flags support!")
    else()
        message(STATUS "GNU compiler check necessary flags OK")
    endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    check_cxx_compiler_flag(-std=c++20 CXX_20_OK)
    check_cxx_compiler_flag(-fno-char8_t CXX_NO_CHAR8_OK)
    if(NOT CXX_20_OK OR NOT CXX_NO_CHAR8_OK)
        message(SEND_ERROR "Clang compiler failed to check necessary compiler flags support!")
    else()
        message(STATUS "Clang compiler check necessary flags OK")
    endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    check_cxx_compiler_flag(-std:c++20 CXX_20_OK)
    check_cxx_compiler_flag(/Zc:char8_t- CXX_NO_CHAR8_OK)
    check_cxx_compiler_flag(/Zc:preprocessor CXX_PREPROCESSOR_OK)
    if(NOT CXX_20_OK OR NOT CXX_NO_CHAR8_OK OR NOT CXX_PREPROCESSOR_OK)
        message(SEND_ERROR "MSVC compiler failed to check necessary compiler flags support!")
    else()
        message(STATUS "MSVC compiler check necessary flags OK")
    endif()
else()
    message(WARNING "Cannot check compiler flags for: ${CMAKE_CXX_COMPILER_ID}")
endif()

target_compile_options(${PROJECT_NAME} PUBLIC
    $<$<CXX_COMPILER_ID:GNU>:-std=c++20 -fno-char8_t>
    $<$<CXX_COMPILER_ID:Clang>:-std=c++20 -fno-char8_t>
    $<$<CXX_COMPILER_ID:MSVC>:-std:c++20 /Zc:char8_t- /Zc:preprocessor>
)


target_include_directories(${PROJECT_NAME}
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PUBLIC "${VENDOR_DIR}/glfw/include"
    PUBLIC "${VENDOR_DIR}/spdlog/include"
    PUBLIC "${VENDOR_DIR}/rapidjson/include"

    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/Storyteller"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/Storyteller/internal"
    PRIVATE "${VENDOR_DIR}/stb_image"
    PRIVATE "${VENDOR_DIR}/pfd"
)

option(STORYTELLER_USE_WINDOW_BACKEND_GLFW "Use GLFW for window management" ON)
if(${STORYTELLER_USE_WINDOW_BACKEND_GLFW})
    target_compile_definitions(${PROJECT_NAME} PUBLIC STRTLR_WINDOW_BACKEND_GLFW)
else()
    message(SEND_ERROR "No window backend is chosen")
endif()

option(STORYTELLER_USE_GRAPHICS_BACKEND_OPENGL "Use OpenGL for graphics" ON)
if (${STORYTELLER_USE_GRAPHICS_BACKEND_OPENGL})
    target_compile_definitions(${PROJECT_NAME} PUBLIC STRTLR_GRAPHICS_BACKEND_OPENGL)
else()
    message(SEND_ERROR "No graphics backend is chosen")
endif()

if(Boost_FOUND)
    target_include_directories(${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} PUBLIC ${Boost_LIBRARIES})
endif()

target_compile_definitions(${PROJECT_NAME} 
    PUBLIC STRTLR_ASSETS_PATH=\"${PROJECT_SOURCE_DIR}/assets\"
    PUBLIC STRTLR_TR_DOMAIN_ENGINE=\"Storyteller\"
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE glfw
)

set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY FOLDER Storyteller)
