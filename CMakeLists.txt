cmake_minimum_required(VERSION 3.10)
project(OpenGLProject)

set(CMAKE_CXX_STANDARD 17)          # Use C++17
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Enforce C++17

# Define paths to dependencies
set(GLEW_ROOT ${CMAKE_SOURCE_DIR}/dependencies/glew)
set(GLFW_ROOT ${CMAKE_SOURCE_DIR}/dependencies/glfw)
set(STB_IMAGE_ROOT ${CMAKE_SOURCE_DIR}/dependencies/stb_image)
set(GLM_ROOT ${CMAKE_SOURCE_DIR}/dependencies/glm)


# Add include directories for your dependencies
include_directories(
    ${GLEW_ROOT}/include
    ${GLFW_ROOT}/include
    ${STB_IMAGE_ROOT}
    ${GLM_ROOT}
)



#! for updated resource folder u must press ctrl-s to reload this file only then it will be copied

# Add the executable
add_executable(${PROJECT_NAME} src/main.cpp src/voxel_manager.cpp)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
message("Source Dir: ${CMAKE_CURRENT_SOURCE_DIR}")
message("Binary Dir: ${CMAKE_CURRENT_BINARY_DIR}")



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
