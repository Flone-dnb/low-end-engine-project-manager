cmake_minimum_required(VERSION 3.16)

if (NOT DEFINED GAME_PROJECT_NAME)
    message(FATAL_ERROR "Expected game project name to be defined.")
endif()
project(${GAME_PROJECT_NAME}) # only `game` target has the actual game name to allow renaming the game very easy

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

# Sources.
set(PROJECT_SOURCES
    src/main.cpp
    # add your .h/.cpp files here
)

# Define target.
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

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

create_symlink_to_res()

# -------------------------------------------------------------------------------------------------
#                                           TOOLS
# -------------------------------------------------------------------------------------------------

# ASan.
if(NOT IS_RELEASE_BUILD AND NOT WIN32)
    enable_address_sanitizer_for_target(${PROJECT_NAME})
endif()

# -------------------------------------------------------------------------------------------------
#                                       DEPENDENCIES
# -------------------------------------------------------------------------------------------------

# Add game library.
set(GAME_LIB_TARGET game_lib)
target_link_libraries(${PROJECT_NAME} PUBLIC ${GAME_LIB_TARGET})
add_dependencies(${PROJECT_NAME} ${GAME_LIB_TARGET})
