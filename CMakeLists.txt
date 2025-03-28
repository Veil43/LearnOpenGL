cmake_minimum_required(VERSION 3.20)

project(learngl)

set(CMAKE_CXX_STANDARD 17)

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