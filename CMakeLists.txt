cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

set(USE_FOLDERS ON)
project(nx_gbc LANGUAGES CXX C)

set(BASE_FOLDER ${CMAKE_FOLDER})

# Setup cpp version
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Setup debug defines
if (MSVC)
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /DDEBUG /D_DEBUG")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /DDEBUG /D_DEBUG")
else()
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG -D_DEBUG")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG -D_DEBUG")
endif()

#########
# Tests #
#########
option(BUILD_TESTING "" OFF)
if (${BUILD_TESTING})
    enable_testing()

    # set target
    set(TARGET_NAME Tests)
    set(TESTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests)

    file(GLOB_RECURSE TEST_FILES ${TESTS_DIR}/*)
    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${TEST_FILES})

    add_library(${TARGET_NAME} INTERFACE ${TEST_FILES})
endif()

################
# Dependencies #
################
set(CMAKE_FOLDER deps)
add_subdirectory(deps)

########
# ROMs #
########

# set target
set(TARGET_NAME ROMs)

file(GLOB_RECURSE ROM_FILES ${CMAKE_CURRENT_SOURCE_DIR}/roms/*)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${ROM_FILES})

add_library(${TARGET_NAME} INTERFACE ${ROM_FILES})

##############
# Executable #
##############

set(CMAKE_FOLDER ${BASE_FOLDER})

# Set compile options
if (MSVC AND NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(/W4 /WX /Zc:preprocessor /Zc:u8EscapeEncoding)
else()
    set(IGNORED_WARNINGS
            -Wno-unused-macros
            -Wno-reserved-macro-identifier
            -Wno-c++98-compat -Wno-c++98-compat-pedantic
            -Wno-covered-switch-default
            -Wno-gnu-zero-variadic-macro-arguments
            -Wno-unsafe-buffer-usage
            -Wno-unknown-pragmas
    )

    add_compile_options(-Wall -Wextra -Wpedantic -Werror ${IGNORED_WARNINGS})
endif()

# Add source files
set(TARGET_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
file(GLOB_RECURSE SOURCE_FILES
    ${TARGET_SOURCE_DIR}/*.c
    ${TARGET_SOURCE_DIR}/*.cc # C with classe
    ${TARGET_SOURCE_DIR}/*.cpp
    ${TARGET_SOURCE_DIR}/*.cxx
    ${TARGET_SOURCE_DIR}/*.c++
)

# Add header files
set(TARGET_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
file(GLOB_RECURSE HEADER_FILES
    ${TARGET_INCLUDE_DIR}/*.h
    ${TARGET_INCLUDE_DIR}/*.hpp
    ${TARGET_INCLUDE_DIR}/*.inl
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${HEADER_FILES} ${SOURCE_FILES})

# Setup executable
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE ${TARGET_INCLUDE_DIR} ${RAYLIB_INCLUDE_DIR} ${IMGUI_INCLUDE_DIR} ${RLIMGUI_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE raylib imgui rlimgui)

if (CMAKE_GENERATOR MATCHES "Visual Studio")
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
endif()

if (${BUILD_TESTING})
    target_include_directories(${PROJECT_NAME} PRIVATE GTest::gtest_main)
    target_link_libraries(${PROJECT_NAME} PRIVATE GTest::gtest_main)
    target_compile_definitions(${PROJECT_NAME} PRIVATE BUILD_TESTING)

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E create_symlink ${TESTS_DIR} $<TARGET_FILE_DIR:${PROJECT_NAME}>/tests
    )

    include(GoogleTest)
    gtest_discover_tests(${PROJECT_NAME})
endif()