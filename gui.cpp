#define _CRT_SECURE_NO_WARNINGS
//#include <Windows.h>
#include "platform_compat.h"
#include <random>
#include <GLFW/glfw3.h>
#ifdef _WIN32
    #include <gl/GL.h>
    #include <gl/GLU.h>
#else
    #include <GL/gl.h>
    #include <GL/glu.h>
#endif
#include <iostream>
#include "cores.h"
#include "primitivas.h"
#include <omp.h> // paralelismo
#include <cmath>
#include <map>

#include "matplotlibcpp.h"



float num = 0.0;

double pastTime = 0.0;

namespace plt = matplotlibcpp;

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

    std::vector<double> x(n), y(n);
    for (int i = 0; i < n; ++i) {
        x.at(i) = i * 2 * M_PI / n; // Gera valores de 0 a 2*PI
        y.at(i) = std::sin(x.at(i));
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
        else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            glLoadIdentity();
			glTranslatef(100.0, 0.0, 1.0);
            glRotated(1.0, 1.0, 0.0, 0.0);
		}

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    
	std::cout << "Calculando raio x velocidade..." << std::endl;

    const int numDivisoes = 500;
    double velxraio[numDivisoes * 2][2] = {0}; // 0 = velocidade, 1 = numero de corpos
    for (int i = 0; i < n; i++) {
		float raio = sqrt(corpos[i].pos[0] * corpos[i].pos[0] + corpos[i].pos[1] * corpos[i].pos[1]);
		float vel = sqrt(corpos[i].vel[0] * corpos[i].vel[0] + corpos[i].vel[1] * corpos[i].vel[1]);
		int indice = (int)(raio / espacamento * numDivisoes);
        if (indice < numDivisoes * 2) {
            velxraio[indice][0] = velxraio[indice][0] + vel;
		    velxraio[indice][1] = velxraio[indice][1] + 1;
        }
		
    }


	std::vector<double> raioPlot, velPlot;
    for (int i = 0; i < numDivisoes; i++) {
        if (velxraio[i][1] > 0) {
            raioPlot.push_back(i * espacamento / numDivisoes);
            velPlot.push_back(velxraio[i][0] / velxraio[i][1]);
        }

    }
	plt::figure_size(800, 500);
	plt::plot(raioPlot, velPlot);
	plt::xlabel("Raio (AL)");
	plt::ylabel("Velocidade (AL/milenio)");
	plt::grid(true);
	plt::title("Velocidade x Raio");
	plt::save("velxraio.png"); 
	std::cout << "Grafico salvo como velxraio.png" << std::endl;
    glfwTerminate();
    return 0;
}
