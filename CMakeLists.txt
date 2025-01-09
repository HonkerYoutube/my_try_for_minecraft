cmake_minimum_required(VERSION 3.10)
project(OpenGLProject)

# Define paths to dependencies
set(GLEW_ROOT ${CMAKE_SOURCE_DIR}/dependencies/glew)
set(GLFW_ROOT ${CMAKE_SOURCE_DIR}/dependencies/glfw)

# Add include directories for your dependencies
include_directories(
    ${GLEW_ROOT}/include
    ${GLFW_ROOT}/include
)

# Add the executable
add_executable(${PROJECT_NAME} src/main.cpp)


target_compile_definitions(${PROJECT_NAME} PRIVATE GLEW_STATIC)


# Link libraries to your project
target_link_libraries(${PROJECT_NAME} PRIVATE 
    ${GLEW_ROOT}/lib/glew32s.lib   # Adjust for your platform, .a for Linux
    ${GLFW_ROOT}/lib/glfw3.lib    # Adjust for your platform
    opengl32                      # OpenGL on Windows (system-provided)
)

# (Optional) Add other compile options or definitions
if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE GLEW_STATIC)  # If using static GLEW
endif()
