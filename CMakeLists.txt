cmake_minimum_required(VERSION 3.30)
project(APE VERSION 0.0.1 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(DEPENDENCIES_DIR ${CMAKE_SOURCE_DIR}/dependencies)

find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(Vulkan REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(tinyobjloader CONFIG REQUIRED)

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    fmt::fmt
    spdlog::spdlog
    Vulkan::Vulkan
    EnTT::EnTT
    glm::glm
    glfw
    tinyobjloader::tinyobjloader
)

file(GLOB DEPENDENCY_DLLS CONFIGURE_DEPENDS ${DEPENDENCIES_DIR}/bin/*.dll)
file(COPY ${DEPENDENCY_DLLS} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

file(GLOB_RECURSE ASSET_FILES ${CMAKE_SOURCE_DIR}/assets/*)

add_custom_command(
    OUTPUT ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets.stamp
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets
    COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets.stamp
    DEPENDS ${ASSET_FILES}
COMMENT "Copying assets to build directory"
)

add_custom_target(copy_assets DEPENDS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets.stamp)
add_dependencies(${CMAKE_PROJECT_NAME} copy_assets)

find_program(VULKAN_GLSLC_EXECUTABLE glslc)
if(NOT VULKAN_GLSLC_EXECUTABLE)
    message(FATAL_ERROR "glslc not found! Please install the Vulkan SDK and ensure glslc is in your PATH.")
endif()

set(SHADER_IN_DIR "${CMAKE_SOURCE_DIR}/shaders")
set(SHADER_OUT_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders")

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

add_custom_target(build_shaders DEPENDS ${SHADER_OUT_NAMES})

add_dependencies(${PROJECT_NAME} build_shaders)
