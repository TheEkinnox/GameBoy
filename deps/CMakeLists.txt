include(FetchContent)

##########
# Raylib #
##########
FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG d7a7eda959e6dff41274e06c3b2621f4e40e5963
)
FetchContent_MakeAvailable(raylib)
set(RAYLIB_INCLUDE_DIR ${raylib_SOURCE_DIR}/src)
set(RAYLIB_INCLUDE_DIR ${RAYLIB_INCLUDE_DIR} PARENT_SCOPE)

#########
# IMGUI #
#########
FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui
        GIT_TAG c254db763709e12eeabea54c52631c2b972b7d58
)

set(BUILD_SHARED_LIBS OFF)
FetchContent_MakeAvailable(imgui)

# Add source files
file(GLOB imgui_SOURCE_FILES ${imgui_SOURCE_DIR}/*.cpp)
list(APPEND imgui_SOURCE_FILES ${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp)

# Add header files
file(GLOB imgui_HEADER_FILES ${imgui_SOURCE_DIR}/*.h)
list(APPEND imgui_HEADER_FILES ${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.h)

source_group(TREE ${imgui_SOURCE_DIR} FILES ${imgui_HEADER_FILES} ${imgui_SOURCE_FILES})

# Setup lib
add_library(imgui STATIC ${imgui_HEADER_FILES} ${imgui_SOURCE_FILES})
target_include_directories(imgui PRIVATE ${imgui_SOURCE_DIR})

set(IMGUI_INCLUDE_DIR ${imgui_SOURCE_DIR} PARENT_SCOPE)

###########
# rlimgui #
###########
FetchContent_Declare(
        rlimgui
        GIT_REPOSITORY https://github.com/raylib-extras/rlimgui.git
        GIT_TAG 4d8a61842903978bc42adf3347cd34f4e6524efc
)
FetchContent_MakeAvailable(rlimgui)

# Add source files
file(GLOB rlimgui_SOURCE_FILES ${rlimgui_SOURCE_DIR}/*.cpp)

# Add header files
file(GLOB rlimgui_HEADER_FILES ${rlimgui_SOURCE_DIR}/*.h)

source_group(TREE ${rlimgui_SOURCE_DIR} FILES ${rlimgui_HEADER_FILES} ${rlimgui_SOURCE_FILES})

# Setup lib
add_library(rlimgui STATIC ${rlimgui_HEADER_FILES} ${rlimgui_SOURCE_FILES})
target_include_directories(rlimgui PRIVATE ${rlimgui_SOURCE_DIR} ${RAYLIB_INCLUDE_DIR} ${imgui_SOURCE_DIR})
target_link_libraries(rlimgui PRIVATE raylib imgui)

set(RLIMGUI_INCLUDE_DIR ${rlimgui_SOURCE_DIR} PARENT_SCOPE)
