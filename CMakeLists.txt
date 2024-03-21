cmake_minimum_required(VERSION 3.16)

project(Storyteller)

include(vendor/Vendor.cmake)
add_subdirectory(Engine)
add_subdirectory(Editor)
add_subdirectory(Runtime)
