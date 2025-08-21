cmake_minimum_required(VERSION 3.10)
project(OpenGLProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define paths to dependencies
set(GLEW_ROOT ${CMAKE_SOURCE_DIR}/dependencies/glew)
set(GLFW_ROOT ${CMAKE_SOURCE_DIR}/dependencies/glfw)
set(STB_IMAGE_ROOT ${CMAKE_SOURCE_DIR}/dependencies/stb_image)
set(GLM_ROOT ${CMAKE_SOURCE_DIR}/dependencies/glm)
set(IMGUI_ROOT ${CMAKE_SOURCE_DIR}/dependencies/imgui-docking)

# Add include directories for your dependencies
include_directories(
    ${GLEW_ROOT}/include
    ${GLFW_ROOT}/include
    ${STB_IMAGE_ROOT}
    ${GLM_ROOT}
    ${IMGUI_ROOT}
    ${IMGUI_ROOT}/backends
)

# IMGUI Sources
set(IMGUI_SOURCES
    ${IMGUI_ROOT}/imgui.cpp
    ${IMGUI_ROOT}/imgui_draw.cpp
    ${IMGUI_ROOT}/imgui_tables.cpp
    ${IMGUI_ROOT}/imgui_widgets.cpp
    ${IMGUI_ROOT}/backends/imgui_impl_glfw.cpp
    ${IMGUI_ROOT}/backends/imgui_impl_opengl3.cpp
    ${IMGUI_ROOT}/imgui_demo.cpp # Consider removing this from release builds
)

# Define the executable target *FIRST*
add_executable(${PROJECT_NAME}
    src/main.cpp
    src/voxel_manager.cpp
    src/constants.cpp
    ${IMGUI_SOURCES} # Add IMGUI sources here
)

# Now you can modify the target
target_include_directories(${PROJECT_NAME} PRIVATE ${IMGUI_ROOT})

target_compile_definitions(${PROJECT_NAME} PRIVATE GLEW_STATIC)

# Link libraries to your project
target_link_libraries(${PROJECT_NAME} PRIVATE
    ${GLEW_ROOT}/lib/glew32s.lib
    ${GLFW_ROOT}/lib/glfw3.lib
    opengl32
)

# Copy resources
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
message("Source Dir: ${CMAKE_CURRENT_SOURCE_DIR}")
message("Binary Dir: ${CMAKE_CURRENT_BINARY_DIR}")

# (Optional) Add other compile options or definitions
if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE GLEW_STATIC)  # If using static GLEW
endif()