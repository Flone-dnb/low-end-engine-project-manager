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

set(GAME_LIB_TARGET ${PROJECT_NAME})
set(GAME_LIB_TARGET_FOR_EDITOR ${PROJECT_NAME}_for_editor)

add_library(${GAME_LIB_TARGET} STATIC ${PROJECT_SOURCES})
add_library(${GAME_LIB_TARGET_FOR_EDITOR} STATIC ${PROJECT_SOURCES})

# Define editor macros for editor's version of the game lib.
target_compile_definitions(${GAME_LIB_TARGET_FOR_EDITOR} PUBLIC ENGINE_EDITOR)
if (IS_RELEASE_BUILD)
    # The editor needs debug drawing and debug console even in release builds.
    target_compile_definitions(${GAME_LIB_TARGET_FOR_EDITOR} PUBLIC ENGINE_DEBUG_TOOLS)
endif()

# -------------------------------------------------------------------------------------------------
#                                         CONFIGURE TARGET
# -------------------------------------------------------------------------------------------------

# Set target folder.
set_target_properties(${GAME_LIB_TARGET} PROPERTIES FOLDER ${PROJECT_FOLDER})
set_target_properties(${GAME_LIB_TARGET_FOR_EDITOR} PROPERTIES FOLDER ${PROJECT_FOLDER})

# Enable more warnings and warnings as errors.
enable_more_warnings_for_target(${GAME_LIB_TARGET})
enable_more_warnings_for_target(${GAME_LIB_TARGET_FOR_EDITOR})

# Set C++ standard.
set(PROJECT_CXX_STANDARD_VERSION 20)
set(CMAKE_CXX_STANDARD ${PROJECT_CXX_STANDARD_VERSION})
target_compile_features(${GAME_LIB_TARGET} PUBLIC cxx_std_${PROJECT_CXX_STANDARD_VERSION})
target_compile_features(${GAME_LIB_TARGET_FOR_EDITOR} PUBLIC cxx_std_${PROJECT_CXX_STANDARD_VERSION})
message(STATUS "${PROJECT_NAME}: using the following C++ standard: ${CMAKE_CXX_STANDARD}")

# Add includes.
target_include_directories(${GAME_LIB_TARGET} PUBLIC src)
target_include_directories(${GAME_LIB_TARGET_FOR_EDITOR} PUBLIC src)

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

# Group all dependencies into a single interface library, later we will link it to 2 version of the engine_lib
# one for the editor (with some editor macros) and one for the game.
set(GAME_LIB_DEPS_TARGET game_lib_deps)
add_library(${GAME_LIB_DEPS_TARGET} INTERFACE)

# Add engine library.
set(ENGINE_LIB_TARGET engine_lib)
set(ENGINE_LIB_TARGET_FOR_EDITOR engine_lib_for_editor)
target_link_libraries(${GAME_LIB_TARGET} PUBLIC ${ENGINE_LIB_TARGET})
add_dependencies(${GAME_LIB_TARGET} ${ENGINE_LIB_TARGET})
target_link_libraries(${GAME_LIB_TARGET_FOR_EDITOR} PUBLIC ${ENGINE_LIB_TARGET_FOR_EDITOR})
add_dependencies(${GAME_LIB_TARGET_FOR_EDITOR} ${ENGINE_LIB_TARGET_FOR_EDITOR})

# Example: add GLM (already included in engine lib, just showing as an example).
#message(STATUS "${PROJECT_NAME}: adding external dependency \"GLM\"...")
#add_subdirectory(${RELATIVE_EXT_PATH}/glm ${DEPENDENCY_BUILD_DIR_NAME}/glm SYSTEM)
#target_link_libraries(${GAME_LIB_DEPS_TARGET} INTERFACE glm)
#set_target_properties(glm PROPERTIES FOLDER ${EXTERNAL_FOLDER})

# Link game library to dependencies.
target_link_libraries(${GAME_LIB_TARGET} PUBLIC ${GAME_LIB_DEPS_TARGET})
target_link_libraries(${GAME_LIB_TARGET_FOR_EDITOR} PUBLIC ${GAME_LIB_DEPS_TARGET})