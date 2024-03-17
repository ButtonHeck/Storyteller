cmake_minimum_required(VERSION 3.16)

project(StorytellerEngine)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std:c++20 /Zc:char8_t-")
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Storyteller/game_controller.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Storyteller/json_reader.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Storyteller/json_writer.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Storyteller/localization_manager.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Storyteller/localization_translator.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Storyteller/localization_dictionary.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Storyteller/localization_library.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Storyteller/console_manager.h"
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
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Storyteller/utils.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Storyteller/image.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Storyteller/platform.h"
)
set(SOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/uuid.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/entities.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/game_document.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/game_document_manager.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/game_document_serializer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/game_document_sort_filter_proxy_view.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/game_controller.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/json_reader.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/json_writer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/localization_manager.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/localization_translator.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/localization_dictionary.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/localization_library.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/console_manager.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/filesystem.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/log.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/application.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/window_application.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/settings.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/window.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/window_glfw.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/dialogs.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/image.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/utils.cpp"
)

aux_source_directory(${VENDOR_DIR}/stb_image STB_IMAGE_SRC)

add_library(${PROJECT_NAME}
	${HEADER_FILES}
    ${SOURCE_FILES}
    ${STB_IMAGE_SRC}
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
    message(ERROR "No window backend is chosen")
endif()

option(STORYTELLER_USE_GRAPHICS_BACKEND_OPENGL "Use OpenGL for graphics" ON)
if (${STORYTELLER_USE_GRAPHICS_BACKEND_OPENGL})
    target_compile_definitions(${PROJECT_NAME} PUBLIC STRTLR_GRAPHICS_BACKEND_OPENGL)
else()
    message(ERROR "No graphics backend is chosen")
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

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER Storyteller)