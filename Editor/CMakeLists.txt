cmake_minimum_required(VERSION 3.20)

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
set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY FOLDER Storyteller/Editor)

get_target_property(exeDir ${PROJECT_NAME} RUNTIME_OUTPUT_DIRECTORY)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} "-DDEST_DIR=${exeDir}" "-DSRC_DIRS=${CMAKE_CURRENT_LIST_DIR}/locale;${CMAKE_CURRENT_LIST_DIR}/../Engine/locale" 
	-P "${CMAKE_SOURCE_DIR}/cmake/copy_translations.cmake"
)


include(StorytellerI18N)
if(StorytellerI18N_OK)
    # Update stage
    add_custom_target(StorytellerEditor_pot_update
        COMMENT "Storyteller Editor .pot file update: Done!"
        DEPENDS ${CMAKE_CURRENT_LIST_DIR}/locale/StorytellerEditor.pot
    )
    set_property(TARGET StorytellerEditor_pot_update APPEND PROPERTY FOLDER Storyteller/Editor)

    add_custom_command(TARGET StorytellerEditor_pot_update
        PRE_BUILD
        COMMAND
            ${GETTEXT_XGETTEXT_EXECUTABLE}
            --from-code=utf-8
            --c++
            --force-po
            --output=${CMAKE_CURRENT_LIST_DIR}/locale/StorytellerEditor.pot
            --keyword="Translate:2,2t"
            --keyword="Translate:2,3,4t"
            --keyword="TranslateCtx:2,3c,3t"
            --keyword="TranslateCtx:2,3,5t,5t"
            --keyword="TranslateDefer:2,2t"
            --keyword="TranslateDefer:2,3,4t"
            --keyword="TranslateCtxDefer:2,3c,3t"
            --keyword="TranslateCtxDefer:2,3,5t,5t"
            ${SOURCE_FILES}
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
        COMMENT "Storyteller Editor .pot file generated: ${CMAKE_CURRENT_LIST_DIR}/locale/StorytellerEditor.pot"
    )


    # Merge stage
    add_custom_target(StorytellerEditor_pot_merge
        COMMENT "Storyteller Editor .pot file merge: Done!"
        DEPENDS ${CMAKE_CURRENT_LIST_DIR}/locale/StorytellerEditor.pot
    )
    set_property(TARGET StorytellerEditor_pot_merge APPEND PROPERTY FOLDER Storyteller/Editor)

    file(GLOB_RECURSE PO_FILES ${CMAKE_CURRENT_LIST_DIR}/locale/*/StorytellerEditor.po)
    message(STATUS " PO_FILES: ${PO_FILES}")

    foreach(PO_FILE IN ITEMS ${PO_FILES})
        message(STATUS " Adding msgmerge for: ${PO_FILE}")
        add_custom_command(TARGET StorytellerEditor_pot_merge
            PRE_BUILD
            COMMAND
                ${GETTEXT_MSGMERGE_EXECUTABLE} ${PO_FILE}
                ${CMAKE_CURRENT_LIST_DIR}/locale/StorytellerEditor.pot
            COMMENT "Storyteller Editor .pot file merge: ${PO_FILE}"
        )
    endforeach()


    # Initialization
    add_custom_target(StorytellerEditor_po_init
        COMMENT "Storyteller Editor .po files initialization: Done! LAUNCH CMAKE AGAIN!"
        DEPENDS ${CMAKE_CURRENT_LIST_DIR}/locale/StorytellerEditor.pot
    )
    set_property(TARGET StorytellerEditor_po_init APPEND PROPERTY FOLDER Storyteller/Editor)

    file(GLOB PO_LANGS LIST DIRECTORIES TRUE ${CMAKE_CURRENT_LIST_DIR}/locale/*)
    foreach(PO_LANG_DIR IN ITEMS ${PO_LANGS})
        if(IS_DIRECTORY ${PO_LANG_DIR})
            cmake_path(GET PO_LANG_DIR STEM LANG_NAME)
            list(APPEND LANG_NAMES ${LANG_NAME})
        endif()
    endforeach()
    
    foreach(LANG_NAME IN LISTS LANG_NAMES)
        if(NOT EXISTS ${CMAKE_CURRENT_LIST_DIR}/locale/${LANG_NAME}/LC_MESSAGES/StorytellerEditor.po)
            add_custom_command(TARGET StorytellerEditor_po_init
                PRE_BUILD
                COMMAND
                    ${GETTEXT_MSGINIT_EXECUTABLE}
                    --input=${CMAKE_CURRENT_LIST_DIR}/locale/StorytellerEditor.pot
                    --output-file=${CMAKE_CURRENT_LIST_DIR}/locale/${LANG_NAME}/LC_MESSAGES/StorytellerEditor.po
                    --locale=${LANG_NAME}
                WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
            )
        endif()
    endforeach()


    # Msgfmt stage
    file(GLOB PO_LANGS LIST DIRECTORIES TRUE ${CMAKE_CURRENT_LIST_DIR}/locale/*/*)
    message(STATUS " PO_LANGS: ${PO_LANGS}")

    add_custom_target(StorytellerEditor_po_compile
        COMMENT "Storyteller Editor .po compilation: Done!"
    )
    set_property(TARGET StorytellerEditor_po_compile APPEND PROPERTY FOLDER Storyteller/Editor)

    foreach(PO_LANG IN ITEMS ${PO_LANGS})
        if(IS_DIRECTORY ${PO_LANG})
            message(STATUS " Adding msgfmt for: ${PO_LANG}")
            add_custom_command(TARGET StorytellerEditor_po_compile
                PRE_BUILD
                COMMAND
                    ${GETTEXT_MSGFMT_EXECUTABLE}
                    --output-file=StorytellerEditor.mo
                    StorytellerEditor.po
                WORKING_DIRECTORY ${PO_LANG}
                COMMENT "Storyteller Editor .po compilation: ${PO_LANG}"
            )
            endif()
    endforeach()
endif()