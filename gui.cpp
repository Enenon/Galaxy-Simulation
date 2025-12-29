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
#include "headers/cores.h"
#include "primitivas.h"
#include <omp.h> // paralelismo
#include <cmath>
#include <map>

#include "headers/old.cpp"
#include "headers/matplotlibcpp.h"
#include <algorithm> // Para std::lower_bound
#include <fstream>


const float intervaloPlot = 0.5; // <--- minutos
int numplots = 0;
float num = 0.0;

double pastTime = 0.0;

namespace plt = matplotlibcpp;

static void plotar() {

    std::cout << "Calculando raio x velocidade..." << std::endl;

	const float raioMaximoPlot = 1.1; // até quantos raios eu quero plotar
    const int numDivisoes = 100;
	const int numTotalDivisoes = numDivisoes * raioMaximoPlot;
    double velxraio[numTotalDivisoes][2] = { 0 }; // 0 = velocidade, 1 = numero de corpos
    for (int i = 0; i < n; i++) {
        float raio = sqrt(corpos[i].pos[0] * corpos[i].pos[0] + corpos[i].pos[1] * corpos[i].pos[1]);
        float vel = sqrt(corpos[i].vel[0] * corpos[i].vel[0] + corpos[i].vel[1] * corpos[i].vel[1]);
        int indice = (int)(raio / espacamento * numDivisoes);
        if (indice < numTotalDivisoes) {
            velxraio[indice][0] = velxraio[indice][0] + vel;
            velxraio[indice][1] = velxraio[indice][1] + 1;
        }
    }
    std::vector<double> raioPlot, velPlot, massaPlot, massaSamplePlot;
    for (int i = 0; i < numTotalDivisoes; i++) {
        if (velxraio[i][1] > 0) {
            raioPlot.push_back(i * espacamento / numDivisoes);
            velPlot.push_back(velxraio[i][0] / velxraio[i][1]);
            //massaPlot.push_back(velxraio[i][1]);
            //massaSamplePlot.push_back(exp(-i / numDivisoes));
        }
    }

	std::vector<double> velSuavizada = suavizar(velPlot, 3);
    std::vector<double> raioSuavizado = suavizar(raioPlot, 3);


    plt::figure_size(1000, 500);
    plt::plot(raioPlot, velPlot, {
    { "label", "valores" },
    { "color", "black" },
    { "linestyle", "" },
    { "marker", "." },
    { "markersize", "3" } });

    plt::plot(raioSuavizado, velSuavizada, {{"label","curva suavizada"}});

    //plt::plot(raioPlot, massaPlot); plt::plot(raioPlot, massaSamplePlot);
	plt::legend();
    plt::grid(true);
    plt::xlabel("Raio(AL)");
    plt::ylabel("Velocidade(AL/milenio)");
}

static void plotar_corpos() {
    std::cout << "Plotando corpos..." << std::endl;
    std::vector<double> x, y, z;
    for (int i = 0; i < n; i++) {
        x.push_back(corpos[i].pos[0]);
        y.push_back(corpos[i].pos[1]);
        z.push_back(corpos[i].pos[2]);
    }
    plt::figure_size(800, 800);
    //plt::scatter3(x, y, z, 1.0);
	//plt::subplot(1, 1, 1);
    plt::scatter(x, y, 1.0,{{"color","black"}});
    plt::xlabel("X (AL)");
    plt::ylabel("Y (AL)");
    //plt::set_zlabel("Z (AL)");
	plt::xlim(-espacamento * 1.5, espacamento * 1.5);
	plt::ylim(-espacamento * 1.5, espacamento * 1.5);
    std::cout << "Gráfico salvo como distribuicao_corpos.png" << std::endl;
}

struct LUT {
    std::vector<double> m_vals;
    std::vector<double> r_vals;
};

// Função para ler o documento e preencher os arrays
LUT loadTable(const std::string& filename) {
    LUT table;
    std::ifstream file(filename);
    double m, r;

    if (!file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo: " << filename << std::endl;
        return table;
    }

    while (file >> m >> r) {
        table.m_vals.push_back(m);
        table.r_vals.push_back(r);
    }

    file.close();
    return table;
}
// Função de Busca Binária e Interpolação Linear
double sampleRadius(double u, const LUT& table) {
    // Busca binária: encontra o primeiro elemento que não é menor que 'u'
    auto it = std::lower_bound(table.m_vals.begin(), table.m_vals.end(), u);

    // Obtém o índice do elemento
    size_t idx = std::distance(table.m_vals.begin(), it);

    // Evita extrapolação fora dos limites da tabela
    if (idx <= 0) return table.r_vals.front();
    if (idx >= table.m_vals.size()) return table.r_vals.back();

    // Interpolação Linear (Lerp)
    double m0 = table.m_vals[idx - 1];
    double m1 = table.m_vals[idx];
    double r0 = table.r_vals[idx - 1];
    double r1 = table.r_vals[idx];

    // r = r0 + (u - m0) * (r1 - r0) / (m1 - m0)
    return r0 + (u - m0) * (r1 - r0) / (m1 - m0);
}

int main(void)
{
    const int largura = 800;
    const int altura = 600;
	
	densidadeMassa();
    inicializarCorpos();
    //return 0;
    //return 0;
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
    //glClearColor(1, 1, 1, 1.0);

    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // <---- ative essa linha se quiser habilitar transparência
    glLoadIdentity();
    gluLookAt(0, 0, 200, 0, 0, 0, 0, 1, 0); // <---- câmera fixa

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        if (glfwGetTime() / 60 > numplots * intervaloPlot) {

            plotar();
            plt::title("Velocidade x Raio apos " + std::to_string(num * dt) + " milenios");
            plt::save("plots/" + std::to_string(numplots));
            plotar_corpos();
            plt::title("Distribuição dos corpos apos " + std::to_string(num * dt) + " milenios");
            plt::save("plots/dist_" + std::to_string(numplots) + ".png");
            numplots++;
        }
        /* Render here */
        int largura, altura;
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glfwGetFramebufferSize(window, &largura, &altura);
        redimensiona(largura,altura);

        //desenha(num);
        desenhag();
		double actualTime = glfwGetTime();
        std::cout << num*dt << " milenios | ";

        std::cout << "FPS: " << 1.0 / (actualTime - pastTime) << std::endl;
		pastTime = actualTime;
        num++;


        //std::cout << rand()/double(RAND_MAX) << std::endl;
        
        // aqui eu boto pra o programa fechar no Esc
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
        else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            //glLoadIdentity();
			// Para girar 90 graus em torno do eixo X use:
            glRotated(1, 1.0, 0.0, 0.0);
			// Alternativa (versão float):
			// glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
		}
        else if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) {
            glLoadIdentity(); gluLookAt(0, 0, 200, 0, 0, 0, 0, 1, 0);
        }
        else if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
            glLoadIdentity(); gluLookAt(0, 0, 200, 0, 0, 0, 0, 1, 0);
            glRotated(90, 1.0, 0.0, 0.0);
        }



        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    plotar();
	plt::title("Velocidade x Raio");
	plt::save("velxraio.png"); 
	std::cout << "Grafico salvo como velxraio.png" << std::endl;
    glfwTerminate();
    return 0;
}
