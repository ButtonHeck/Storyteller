cmake_minimum_required(VERSION 3.16)

project(StorytellerEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(VENDOR_DIR "${CMAKE_SOURCE_DIR}/vendor")
set(HEADER_FILES
	"${CMAKE_CURRENT_SOURCE_DIR}/include/entry_point.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/storyteller.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/uuid.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/entities.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/game_document.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/game_document_sort_filter_proxy_view.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/game_controller.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/localization_manager.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/console_manager.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/filesystem_utils.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/log.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/application.h"
)
set(SOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/uuid.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/entities.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/game_document.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/game_document_sort_filter_proxy_view.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/game_controller.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/localization_manager.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/console_manager.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/filesystem_utils.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/log.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/application.cpp"
)

add_library(${PROJECT_NAME}
	${HEADER_FILES}
    ${SOURCE_FILES}
)

target_include_directories(${PROJECT_NAME}
	PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE "${VENDOR_DIR}/rapidjson/include"
    PUBLIC "${VENDOR_DIR}/spdlog/include"
)

if(Boost_FOUND)
    target_include_directories(${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} PUBLIC ${Boost_LIBRARIES})
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Storyteller")