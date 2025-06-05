cmake_minimum_required(VERSION 3.30)
project(AnanthamEngineProject VERSION 0.0.1 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(OUT_DIR ${CMAKE_SOURCE_DIR}/out)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUT_DIR}/bin)

set(ASSETS_OUT_DIR ${OUT_DIR}/assets)
set(ENGINE_ASSETS_OUT_DIR ${ASSETS_OUT_DIR}/engine)
set(EDITOR_ASSETS_OUT_DIR ${ASSETS_OUT_DIR}/editor)

set(ENGINE_SOURCE_DIR ${CMAKE_SOURCE_DIR}/engine)
set(EDITOR_SOURCE_DIR ${CMAKE_SOURCE_DIR}/editor)
set(DEPENDENCIES_SOURCE_DIR ${CMAKE_SOURCE_DIR}/dependencies)


find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(Vulkan REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(tinyobjloader CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)


# Engine

file(GLOB_RECURSE ENGINE_SRC_FILES CONFIGURE_DEPENDS
    ${ENGINE_SOURCE_DIR}/src/*.cpp
)
add_library(AnanthamEngine SHARED ${ENGINE_SRC_FILES})
target_include_directories(AnanthamEngine
    PUBLIC
        ${ENGINE_SOURCE_DIR}/include
    PRIVATE
        ${ENGINE_SOURCE_DIR}/src
)
target_link_libraries(AnanthamEngine PRIVATE
    fmt::fmt
    spdlog::spdlog
    Vulkan::Vulkan
    EnTT::EnTT
    glm::glm
    glfw
    tinyobjloader::tinyobjloader
    imgui::imgui
)

file(GLOB_RECURSE ENGINE_ASSET_FILES ${ENGINE_SOURCE_DIR}/assets/*)
set(ENGINE_ASSETS_STAMP_FILE ${CMAKE_CURRENT_BINARY_DIR}/engineAssets.stamp)

add_custom_command(
    OUTPUT ${ENGINE_ASSETS_STAMP_FILE}
    COMMAND ${CMAKE_COMMAND}
        -E copy_directory_if_different
        ${ENGINE_SOURCE_DIR}/assets ${ENGINE_ASSETS_OUT_DIR}
    COMMAND ${CMAKE_COMMAND} -E touch ${ENGINE_ASSETS_STAMP_FILE}
    DEPENDS ${ENGINE_ASSET_FILES}
    COMMENT "Copying assets to build directory"
    VERBATIM
)

add_custom_target(sync_engine_assets ALL DEPENDS ${ENGINE_ASSETS_STAMP_FILE})
add_dependencies(AnanthamEngine sync_engine_assets)


find_program(VULKAN_GLSLC_EXECUTABLE glslc)
if(NOT VULKAN_GLSLC_EXECUTABLE)
    message(FATAL_ERROR
    "VulkanSDK glslc not found! Please ensure glslc is in your PATH.")
endif()

set(SHADER_IN_DIR "${ENGINE_SOURCE_DIR}/shaders")
set(SHADER_OUT_DIR "${ENGINE_ASSETS_OUT_DIR}/shaders")

file(GLOB_RECURSE SHADER_FILES CONFIGURE_DEPENDS
    ${SHADER_IN_DIR}/*.vert
    ${SHADER_IN_DIR}/*.frag
)

foreach(SHADER_FILE ${SHADER_FILES})
    get_filename_component(SHADER_NAME ${SHADER_FILE} NAME)
    set(SHADER_OUT_NAME "${SHADER_OUT_DIR}/${SHADER_NAME}.spv")
    list(APPEND SHADER_OUT_NAMES ${SHADER_OUT_NAME})
    add_custom_command(
        MAIN_DEPENDENCY ${SHADER_FILE}
        OUTPUT ${SHADER_OUT_NAME}
        COMMAND ${VULKAN_GLSLC_EXECUTABLE} ${SHADER_FILE} -o ${SHADER_OUT_NAME}
        VERBATIM
    )
endforeach()

add_custom_target(build_engine_shaders DEPENDS ${SHADER_OUT_NAMES})

add_dependencies(AnanthamEngine build_engine_shaders)


# Editor

file(GLOB_RECURSE EDITOR_SRC_FILES CONFIGURE_DEPENDS
    ${EDITOR_SOURCE_DIR}/src/*.cpp
)
message(STATUS "Editor source files: ${EDITOR_SRC_FILES}")

add_executable(AnanthamEditor ${EDITOR_SRC_FILES})


target_include_directories(AnanthamEditor
    PRIVATE
        ${EDITOR_SOURCE_DIR}/include
        ${EDITOR_SOURCE_DIR}/src
)

target_link_libraries(AnanthamEditor PRIVATE
    AnanthamEngine
    spdlog::spdlog
    imgui::imgui
)

file(GLOB_RECURSE EDITOR_ASSET_FILES ${EDITOR_SOURCE_DIR}/assets/*)
set(EDITOR_ASSETS_STAMP_FILE ${CMAKE_CURRENT_BINARY_DIR}/editorAssets.stamp)

add_custom_command(
    OUTPUT ${EDITOR_ASSETS_STAMP_FILE}
    COMMAND ${CMAKE_COMMAND}
        -E copy_directory_if_different
        ${EDITOR_SOURCE_DIR}/assets ${EDITOR_ASSETS_OUT_DIR}
    COMMAND ${CMAKE_COMMAND} -E touch ${EDITOR_ASSETS_STAMP_FILE}
    DEPENDS ${EDITOR_ASSET_FILES}
    COMMENT "Copying assets to build directory"
    VERBATIM
)

add_custom_target(sync_editor_assets ALL DEPENDS ${EDITOR_ASSETS_STAMP_FILE})
add_dependencies(AnanthamEditor sync_editor_assets)


# Manual dependencies
file(GLOB DEPENDENCY_BINARY_FILES ${DEPENDENCIES_SOURCE_DIR}/bin/*.dll)
add_custom_target(copy_dependencies ALL
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${DEPENDENCY_BINARY_FILES}
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    COMMENT "Copying dependencies to output directory"
)
add_dependencies(AnanthamEngine copy_dependencies)
add_dependencies(AnanthamEditor copy_dependencies)
