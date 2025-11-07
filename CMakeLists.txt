# Set the minimum CMake version and project name
cmake_minimum_required(VERSION 3.15)
project(GalaxySimulation CXX)

# Set the C++ standard to C++20 (as mentioned in your README)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Find All Dependencies ---

# 1. Find OpenMP (for parallelism)
find_package(OpenMP REQUIRED)

# 2. Find OpenGL and GLU (for graphics)
find_package(OpenGL REQUIRED)
find_package(GLU REQUIRED)

# 3. Find GLFW (for the window)
find_package(glfw3 3.3 REQUIRED)

# 4. Find Python (for matplotlibcpp)
# This finds the interpreter, compiler flags, and libraries
find_package(Python 3 REQUIRED COMPONENTS Development.Embed)

# 5. Find NumPy
# We use Python itself to find the NumPy header path
execute_process(
  COMMAND "${Python_EXECUTABLE}" -c "import numpy; print(numpy.get_include())"
  OUTPUT_VARIABLE NumPy_INCLUDE_DIRS
  OUTPUT_STRIP_TRAILING_WHITESPACE
  RESULT_VARIABLE _numpy_result
)

# Fail if NumPy isn't found
if(NOT _numpy_result EQUAL 0)
  message(FATAL_ERROR "Failed to find NumPy include directory. Is numpy installed? (pip install numpy)")
endif()

# --- Configure the Executable ---

# Add the main C++ file as the executable
add_executable(galaxy_sim gui.cpp)

# Add all the include directories
target_include_directories(galaxy_sim PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}"  # Lets it find matplotlibcpp.h, primitivas.h
  "${OpenGL_INCLUDE_DIR}"
  "${GLU_INCLUDE_DIR}"
  "${glfw3_INCLUDE_DIRS}"
  "${Python_INCLUDE_DIRS}"
  "${NumPy_INCLUDE_DIRS}"
)

# Link all the libraries
target_link_libraries(galaxy_sim PRIVATE
  OpenMP::OpenMP_CXX  # Link OpenMP
  OpenGL::GL          # Link OpenGL
  GLU::GLU            # Link GLU
  glfw                # Link GLFW
  Python::Python      # Link Python
)