cmake_minimum_required(VERSION 3.16)
project(GalaxySimulation C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files present in the repo
add_executable(galaxy
    gui.cpp
)

# Find system GLFW and OpenGL
find_package(PkgConfig REQUIRED)
find_package(OpenGL REQUIRED)
pkg_check_modules(GLFW3 REQUIRED glfw3)

target_include_directories(galaxy PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${GLFW3_INCLUDE_DIRS}
)

target_link_libraries(galaxy
    GL
    GLU
    ${GLFW3_LIBRARIES}
    X11 Xrandr Xinerama Xcursor Xi 
)

# Some Ubuntu/GLFW builds prefer this form:
# target_link_libraries(galaxy OpenGL::GL glfw)

# On some distros, you may also need the X11 libs explicitly:
# target_link_libraries(galaxy X11 Xrandr Xinerama Xcursor Xi)

