# Versão mínima do CMake
cmake_minimum_required(VERSION 3.12)

# Nome do projeto
project(MeuProjetoGrafico)

# --- Encontrar Bibliotecas ---

# 1. Encontrar Python (incluindo headers de desenvolvimento)
# Isso substitui o -I/usr/include/python3.12 e -lpython3.12
find_package(Python 3.12 REQUIRED COMPONENTS Interpreter Development)
find_package(Threads REQUIRED)

# 2. Encontrar o Numpy (o jeito "correto" via CMake)
# Isso executa um mini-script Python para perguntar ao Numpy onde ele está
execute_process(
  COMMAND ${Python_EXECUTABLE} -c "import numpy; print(numpy.get_include())"
  OUTPUT_VARIABLE NUMPY_INCLUDE_DIR
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
# Informa ao CMake sobre o diretório de include do Numpy
include_directories(${NUMPY_INCLUDE_DIR})


# 3. Encontrar OpenGL, GLU e GLFW
# Isso substitui o -lGL -lGLU -lglfw
find_package(OpenGL REQUIRED)
find_package(GLU REQUIRED)
find_package(glfw3 3.3 REQUIRED) # 3.3 é uma versão comum

# --- Configurar o Executável ---

# Diz ao CMake para criar um executável chamado "result_plot" a partir de "gui.cpp"
add_executable(result_plot gui.cpp)

# Linka todas as bibliotecas que encontramos com o seu executável
target_link_libraries(result_plot PRIVATE
    ${Python_LIBRARIES}  
    OpenGL::GL           
    OpenGL::GLU          
    glfw                 
    Threads::Threads  
)

# Adiciona os diretórios de include necessários
target_include_directories(result_plot PRIVATE
    ${Python_INCLUDE_DIRS}
    ${NUMPY_INCLUDE_DIR}
)
