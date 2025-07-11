cmake_minimum_required(VERSION 3.16)

if (NOT GAME_PROJECT_NAME)
    message(FATAL_ERROR "Expected game project name to be defined.")
endif()
project(${GAME_PROJECT_NAME}_tests)

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
enable_more_warnings()

# Set C++ standard.
set(PROJECT_CXX_STANDARD_VERSION 20)
set(CMAKE_CXX_STANDARD ${PROJECT_CXX_STANDARD_VERSION})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_${PROJECT_CXX_STANDARD_VERSION})
message(STATUS "${PROJECT_NAME}: using the following C++ standard: ${CMAKE_CXX_STANDARD}")

# Add includes.
target_include_directories(${PROJECT_NAME} PUBLIC src)

# Disable optimizations for tests.
if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE -Od)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -O0)
endif()

create_symlink_to_res()

# -------------------------------------------------------------------------------------------------
#                                           TOOLS
# -------------------------------------------------------------------------------------------------

# Enable Address Sanitizer in `Debug` builds on non-Windows OS.
if(NOT IS_RELEASE_BUILD AND NOT WIN32)
    enable_address_sanitizer()
endif()

# -------------------------------------------------------------------------------------------------
#                                       DEPENDENCIES
# -------------------------------------------------------------------------------------------------

# Add game library.
set(GAME_LIB_TARGET ${GAME_PROJECT_NAME}_lib)
if (NOT TARGET ${GAME_LIB_TARGET}) # define target only if not defined yet
    message(STATUS "${PROJECT_NAME}: started adding ${GAME_LIB_TARGET}...\n----------------------------------------------\n")
    add_subdirectory(../${GAME_LIB_TARGET} ${DEPENDENCY_BUILD_DIR_NAME}/${GAME_LIB_TARGET})
    message(STATUS "\n\n----------------------------------------------\n${PROJECT_NAME}: finished adding ${GAME_LIB_TARGET}")
else()
    message(STATUS "${PROJECT_NAME}: ${GAME_LIB_TARGET} already defined, just using it without redefining")
endif()
target_link_libraries(${PROJECT_NAME} PUBLIC ${GAME_LIB_TARGET})
add_dependencies(${PROJECT_NAME} ${GAME_LIB_TARGET})

# Add `Catch2`.
message(STATUS "${PROJECT_NAME}: adding external dependency \"Catch2\"...")
if (NOT TARGET Catch2) # define target only if not defined yet
    add_subdirectory(${RELATIVE_EXT_PATH}/Catch2 ${DEPENDENCY_BUILD_DIR_NAME}/Catch2 SYSTEM)
endif()
set_target_properties(Catch2 PROPERTIES FOLDER ${EXTERNAL_FOLDER})
set_target_properties(Catch2WithMain PROPERTIES FOLDER ${EXTERNAL_FOLDER})
target_link_libraries(${PROJECT_NAME} PUBLIC Catch2::Catch2)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/ext/Catch2/contrib")
include(CTest)
include(Catch)
