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
#include <iomanip>




const float intervaloPlot = 2; // <--- minutos
int numplots = 0;
float num = 0.0;

double pastTime = 0.0;

namespace plt = matplotlibcpp;

static void salvar_tabela(string nome_arquivo, std::vector<double> r, std::vector<double> v) {
    std::ofstream arquivo(nome_arquivo);
    int tamanho = r.size();
    arquivo << std::setw(10) << std::left << "#r";
    arquivo << std::setw(10) << std::left << "v";
    arquivo << "\n";

    if (arquivo.is_open()) {
        for (int i = 0; i < tamanho; i++) {
            arquivo << std::setw(10) << std::left << r[i];
            arquivo << std::setw(10) << std::left << v[i];
            arquivo << "\n"; // Quebra de linha ao fim de cada linha da matriz
        }
        arquivo.close();
        std::cout << "Dados salvos com sucesso!" << std::endl;
    }
    else {
        std::cout << "Erro ao abrir o arquivo." << std::endl;
    }
}



static void plotar() {

    std::cout << "Calculando raio x velocidade..." << std::endl;

    const int numTotalDivisoes = tem_corpo_teste ? numDivisoes : numDivisoes*raioMaximoPlot;
    double velxraio[numTotalDivisoes][2] = { 0 }; // 0 = velocidade, 1 = numero de corpos
    std::vector<double> raioPlot, velPlot, massaPlot, massaSamplePlot;

    if (!tem_corpo_teste){
    for (int i = 0; i < n; i++) {
        //float raio = sqrt(corpos[i].pos[0] * corpos[i].pos[0] + corpos[i].pos[1] * corpos[i].pos[1]);
        float raio = sqrt(corpos[i].pos[0] * corpos[i].pos[0] + corpos[i].pos[1] * corpos[i].pos[1] + corpos[i].pos[2] * corpos[i].pos[2]);
        float vel = sqrt(corpos[i].vel[0] * corpos[i].vel[0] + corpos[i].vel[1] * corpos[i].vel[1]);
        int indice = (int)(raio / espacamento * numDivisoes);
        if (raio > 0) {
            if (indice < numTotalDivisoes) {
                velxraio[indice][0] = velxraio[indice][0] + vel;
                velxraio[indice][1] = velxraio[indice][1] + 1;
            }
        }
    }
    for (int i = 0; i < numTotalDivisoes; i++) {
        if (velxraio[i][1] > 0) {
            raioPlot.push_back(i * espacamento / numDivisoes * 3.07e-4); // conversão de AL para kpc
            velPlot.push_back(velxraio[i][0] / velxraio[i][1] * 299.14); // conversão de AL/milenio para km/s
            //massaPlot.push_back(velxraio[i][1]);
            //massaSamplePlot.push_back(exp(-i / numDivisoes));
        }
    }
    }
    else {
        // corpo de teste
        for (int i = n; i < n + numCorposTeste; i++) {
            float raio = corpos[i].raioInicial;
            float vel = corpos[i].vel[1];
			//cout << "Raio: " << raio << " Velocidade: " << vel << endl;
			raioPlot.push_back(raio * 3.07e-4); // conversão de AL para kpc
			velPlot.push_back(vel * 299.14); // conversão de AL/milenio para km/s
			//cout << "Indice: " << i - n << " VelxRaio: " << velxraio[i - n][0] << " NumCorpos: " << velxraio[i - n][1] << endl;
            }
        
	}

    std::vector<double> velSuavizada = exponentialSmoothing(velPlot, 0.08);
    std::vector<double> raioSuavizado = exponentialSmoothing(raioPlot, 0.08);
    for (int i = 0; i < velSuavizada.size(); i++) {
        //cout << "Raio suavizado: " << raioSuavizado[i] << " Velocidade suavizada: " << velSuavizada[i] << endl;
	}
    if (numplots == 0) {
        salvar_tabela("plots/velxraio_" + std::to_string(numplots) + ".txt", raioPlot, velPlot);
	}
	


    plt::figure_size(800, 600);
    plt::plot(raioPlot, velPlot, {
    { "label", "valores" },
    { "color", "gray" },
    { "linestyle", "" },
    { "marker", "." },
    { "markersize", "3" } });
    if (!tem_corpo_teste) {
        plt::plot(raioSuavizado, velSuavizada, { {"label","curva suavizada"} });
    }
    
    //plt::plot(raioPlot, massaPlot); plt::plot(raioPlot, massaSamplePlot);
	plt::legend();
    //plt::grid(true);
    plt::xlabel("Raio(kpc)");
    plt::ylabel("Velocidade(km/s)");
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


    //glClearColor(0.02, 0.02, 0.02, 1.0); // cor do fundo
    glClearColor(1, 1, 1, 1.0);

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
			cout << "Plot salvo como plots/" << numplots - 1 << ".png" << endl;
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
