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

find_package(Boost REQUIRED COMPONENTS 
	locale 
	program_options 
	REQUIRED
) 

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

add_subdirectory_with_folder("${CMAKE_CURRENT_LIST_DIR}/glfw" "vendor")
add_subdirectory_with_folder("${CMAKE_CURRENT_LIST_DIR}/rapidjson" "vendor")
add_subdirectory_with_folder("${CMAKE_CURRENT_LIST_DIR}/spdlog" "vendor")
add_subdirectory_with_folder("${CMAKE_CURRENT_LIST_DIR}/pfd" "vendor")