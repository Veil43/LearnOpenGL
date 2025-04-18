cmake_minimum_required(VERSION 3.20)

project(learngl)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE_DEBUG ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(demo src/demo/main.cc
    src/ext/glad/src/glad.c)

# Configure GLFW things
add_subdirectory(src/ext/glfw)
target_link_libraries(demo PRIVATE glfw)

# Configure GLAD tings
target_include_directories(demo PRIVATE src/ext/glad/include/)

# Configure OpenGL things
find_package(OpenGL REQUIRED)
target_link_libraries(demo PRIVATE OpenGL::GL)

# Configure lone headers
target_include_directories(demo PRIVATE src/ext)

# Configure GLM
add_subdirectory(src/ext/glm)
target_link_libraries(demo PRIVATE glm)

# Configure Assimp
add_subdirectory(src/ext/assimp)
target_link_libraries(demo PRIVATE assimp)
set(ASSIMP_BUILD_TESTS OFF)
set(ASSIMP_BUILD_ASSIMP_TOOLS ON)
# Suppress warnings about deprecated functions
add_definitions(-D_CRT_SECURE_NO_WARNINGS)

# Disable treating warnings as errors

# Degub config stuff
if(ENABLE_SAN AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Enabling sanitizers (ASan and UBSan)")

    # Enable AddressSanitizer and UndefinedBehaviorSanitizer
    target_compile_options(demo PRIVATE
        -fsanitize=address
        -fsanitize=undefined
        -g  # Enable debugging information
    )

    target_link_libraries(demo PRIVATE
        -fsanitize=address
        -fsanitize=undefined
    )
endif()
