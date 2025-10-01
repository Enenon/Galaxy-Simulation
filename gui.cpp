#include<Windows.h>
#include <random>
#include <GLFW/glfw3.h>
#include <gl/GLU.h>
#include <iostream>
#include "cores.h"
#include "primitivas.h"
#include <omp.h> // paralelismo


float num = 0.0;

double pastTime = 0.0;

int main(void)
{
    const int largura = 1920;
    const int altura = 1080;
	

    inicializarCorpos();

    /* Initialize the library */
    glfwInit();

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow(largura, altura, "Salve", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);


    glClearColor(0.02, 0.02, 0.02, 1.0); // cor do fundo

    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // <---- ative essa linha se quiser habilitar transparência

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        int largura, altura;
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glfwGetFramebufferSize(window, &largura, &altura);
        redimensiona(largura,altura);

        //desenha(num);
        desenhag();
		double actualTime = glfwGetTime();
		std::cout << num << " ";
        std::cout << "FPS: " << 1.0 / (actualTime - pastTime) << std::endl;
		pastTime = actualTime;
        num++;

        //std::cout << rand()/double(RAND_MAX) << std::endl;
        
        // aqui eu boto pra o programa fechar no Esc
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}