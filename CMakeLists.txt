cmake_minimum_required(VERSION 3.20)


project(dijisktra)

set(CMAKE_BUILD_TYPE Debug)

set(PROJECT_DIR ${CMAKE_SOURCE_DIR})

include(FetchContent)

set(RAYLIB_VERSION 5.0)

FetchContent_Declare(
    raylib
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    FIND_PACKAGE_ARGS ${RAYLIB_VERSION}
)

set(BUILD_EXAMPLES OFF CACHE INTERNAL "")

FetchContent_MakeAvailable(raylib)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

add_subdirectory(Processing)
add_subdirectory(Renderer)
add_subdirectory(FileParser)

set(PROJECT_SOURCES main.cpp)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_link_libraries(${PROJECT_NAME} Processing Renderer FileParser raylib nlohmann_json::nlohmann_json)

# Copy resources to binary directory
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/Maps)
add_custom_target(copy_resources ALL
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${PROJECT_DIR}/Maps
            ${PROJECT_BINARY_DIR}/Maps
            COMMENT "Copying resources into binary directory"
            )
add_dependencies(${PROJECT_NAME} copy_resources)

# Additional settings for macOS
if (APPLE)
    # Link necessary frameworks for macOS
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()
