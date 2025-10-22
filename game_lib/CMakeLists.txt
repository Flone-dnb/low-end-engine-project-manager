cmake_minimum_required(VERSION 3.16)

project(game_lib)

# Define some relative paths.
set(RELATIVE_EXT_PATH "../../ext")
set(RELATIVE_CMAKE_HELPERS_PATH "../.cmake")

# Include essential stuff.
include(${RELATIVE_CMAKE_HELPERS_PATH}/essential.cmake)

# Include helper functions.
include(${RELATIVE_CMAKE_HELPERS_PATH}/utils.cmake)

# -------------------------------------------------------------------------------------------------
#                                          TARGET SOURCES
# -------------------------------------------------------------------------------------------------

# Specify project sources.
set(PROJECT_SOURCES
    src/MyGameInstance.h
    src/MyGameInstance.cpp
    src/GameInputEventIds.hpp
    src/node/MyCharacterNode.cpp
    src/node/MyCharacterNode.h
    # add your .h/.cpp files here
)

# Define target.
add_library(${PROJECT_NAME} STATIC ${PROJECT_SOURCES})

# -------------------------------------------------------------------------------------------------
#                                         CONFIGURE TARGET
# -------------------------------------------------------------------------------------------------

# Set target folder.
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER ${PROJECT_FOLDER})

# Enable more warnings and warnings as errors.
enable_more_warnings_for_target(${PROJECT_NAME})

# Set C++ standard.
set(PROJECT_CXX_STANDARD_VERSION 20)
set(CMAKE_CXX_STANDARD ${PROJECT_CXX_STANDARD_VERSION})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_${PROJECT_CXX_STANDARD_VERSION})
message(STATUS "${PROJECT_NAME}: using the following C++ standard: ${CMAKE_CXX_STANDARD}")

# Add includes.
target_include_directories(${PROJECT_NAME} PUBLIC src)

# Debug helper: create a file named `debug.cmake` next to this file to disable optimizations.
if (NOT IS_RELEASE_BUILD AND EXISTS "${CMAKE_CURRENT_LIST_DIR}/debug.cmake")
    message(STATUS "${PROJECT_NAME}: disabling optimizations because the file \"debug.cmake\" exists.")
    if (MSVC)
        target_compile_options(${PROJECT_NAME} PRIVATE -Od)
    else()
        target_compile_options(${PROJECT_NAME} PRIVATE -O0)
    endif()
endif()

# -------------------------------------------------------------------------------------------------
#                                           TOOLS
# -------------------------------------------------------------------------------------------------

# ASan.
if(NOT IS_RELEASE_BUILD AND NOT WIN32)
    enable_address_sanitizer_for_target(${PROJECT_NAME})
endif()

# node super call checker
add_node_super_call_checker_for_target(${PROJECT_NAME} ${CMAKE_CURRENT_LIST_DIR}/src/node ${CMAKE_CURRENT_LIST_DIR}/src/node)

# -------------------------------------------------------------------------------------------------
#                                       DEPENDENCIES
# -------------------------------------------------------------------------------------------------

# Add engine library.
set(ENGINE_LIB_TARGET engine_lib)
target_link_libraries(${PROJECT_NAME} PUBLIC ${ENGINE_LIB_TARGET})
add_dependencies(${PROJECT_NAME} ${ENGINE_LIB_TARGET})
