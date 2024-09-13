cmake_minimum_required(VERSION 3.10)
project(NoteTakingApp)

set(CMAKE_CXX_STANDARD 11)

# Add Dear ImGui files
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/imgui)
file(GLOB IMGUI_SOURCES
    ${IMGUI_DIR}/*.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

# Add your main source file
add_executable(NoteTakingApp main.cpp ${IMGUI_SOURCES})

# Link libraries for ImGui (OpenGL, GLFW)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

# Include the ImGui and backend directories
target_include_directories(NoteTakingApp PRIVATE 
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)

# Link the executable with OpenGL and GLFW libraries
target_link_libraries(NoteTakingApp OpenGL::GL glfw)
