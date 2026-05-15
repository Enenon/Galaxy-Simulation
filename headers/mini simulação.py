import matplotlib.pyplot as plt
import numpy as np
from random import random
from random import seed
from scipy.signal import savgol_filter

def rng(): return random()-0.5

obj = 1 # 0 = plotar trajetorias, 1 plotar leapfrog vs euler, 2 = energia para diferentes passos de tempo, 3 = plotar trajetorias leapfrog vs euler
n = 5
random_seed = 42
seed(random_seed)


corpos = np.zeros((n,10))
#massa, x, y, z, vx, vy, vz, ax, ay, az
corpos[0] = [2000, 0, 0, 0, 0, 0, 0 ,0,0,0]
corpos[1] = [1, 0, 0, 0, 0, 0, 0 ,0,0,0]
corpos[2] = [0.001, 0, 0, 0, 0, 0, 0 ,0,0,0]

'''
rdis = 1

for i in range(n):
    corpos[i] = [rng()*5,rng()*rdis,rng()*rdis,rng()*rdis, rng()*0.1,rng()*0.1,rng()*0.1, 0,0,0]'''
raioorbita = 1500
r_min2 = 10
t = 1000000
h = 100
G = 0.4e-4
def calcula_orbita(corpoi, corpoj, raioorbita=raioorbita):
    theta = random()*2*np.pi
    znorm = 2*rng()
    r_xy = raioorbita * np.sqrt(1.0 - znorm * znorm)
    if (r_xy != r_xy): # verifica se é NaN
        r_xy = 0

    corpoi[1] = np.cos(theta) * r_xy + corpoj[1]
    corpoi[2] = np.sin(theta) * r_xy + corpoj[2]
    corpoi[3] = znorm * raioorbita + corpoj[3]


    r2 = (corpoi[1])**2+(corpoi[2])**2+(corpoi[3])**2
    r = np.sqrt(r2)
    a = -G*corpoj[0]/(r2)
    velocidade = np.sqrt(-a*r)
    corpoi[4] = -velocidade * np.sin(theta) + corpoj[4]
    corpoi[5] = velocidade * np.cos(theta) + corpoj[5]
    corpoi[6] = 0

    return corpoi

corpos[1] = calcula_orbita(corpos[1], corpos[0])
corpos[2] = calcula_orbita(corpos[2], corpos[1],raioorbita=10)

def corpos_aleatorios(corpos,massa):
    for i in range(n):
        corpos[i] = [massa, rng()*raioorbita, rng()*raioorbita, rng()*raioorbita, rng()*0.1, rng()*0.1, rng()*0.1, 0, 0, 0]
    return corpos

corpos = corpos_aleatorios(corpos,1000)



class Sistema:
    def __init__(self, corpos):
        self.corpos = corpos.copy()
        self.Ecin = None
        self.Epot = None
        self.posis = None


    def evolui_temporalmente(self,t, h,leapfrog=False):
        self.posis = np.zeros((n,int(t/h)+1,3))

        self.Ecin = np.zeros(int(t/h)+1)
        self.Epot = np.zeros(int(t/h)+1)


        for tl in np.arange(0,t,h):
            for i, ci in enumerate(self.corpos):
                self.corpos[i][7], self.corpos[i][8], self.corpos[i][9] = 0, 0, 0
                for j, cj in enumerate(self.corpos):
                    if i != j:
                        r2 = (ci[1]-cj[1])**2+(ci[2]-cj[2])**2+(ci[3]-cj[3])**2
                        r = np.sqrt(r2)
                        a = -G*cj[0]/(r2+r_min2)
                        for xi in range(1,4):
                            self.corpos[i][xi+6] += a*(ci[xi] - cj[xi])/r
                        self.Epot[int(round(tl/h,0))] += ci[0]*a*r/2
                self.Ecin[int(round(tl/h,0))] += ci[0]*(ci[4]*ci[4]+ci[5]*ci[5]+ci[6]*ci[6])*0.5

            for i, ci in enumerate(self.corpos):
                if leapfrog:
                    for xi in range(1,4):
                        if tl == 0:
                            self.corpos[i][xi+3] += h*self.corpos[i][xi+6]*0.5
                        else:
                            self.corpos[i][xi+3] += h*self.corpos[i][xi+6]
                            self.corpos[i][xi] += h*self.corpos[i][xi+3]

                        self.posis[i,int(round(tl/h,0)),xi-1] = self.corpos[i][xi]
                else:
                    for xi in range(1,4):
                        self.corpos[i][xi+3] += h*self.corpos[i][xi+6]
                        self.corpos[i][xi] += h*self.corpos[i][xi+3]

                        self.posis[i,int(round(tl/h,0)),xi-1] = self.corpos[i][xi]

sistema = Sistema(corpos)
import scienceplots
if obj == 0: # se eu quiser plotar as trajetórias
    ax = plt.figure().add_subplot(projection='3d')
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Z')
    ax.scatter(corpos[1][1],corpos[1][2],corpos[1][3], s=10, marker='x', c='black',linewidth=1,label='Posição inicial da partícula 1')
    sistema.evolui_temporalmente(t,h,leapfrog=True)
    for i in range(n):
        ax.plot(sistema.posis[i,:-1,0],sistema.posis[i,:-1,1],sistema.posis[i,:-1,2], label=f'Trajetória Partícula {i}')
        # Marca a posição final com um ponto maior
        ax.scatter(sistema.posis[i,-2,0], sistema.posis[i,-2,1], sistema.posis[i,-2,2], s=10, marker='o', linewidth=1, label = f'Posição final da partícula {i}')
        # Opcional: adiciona texto com o número da partícula próximo ao ponto final
        #ax.text(sistema.posis[i,-1,0], sistema.posis[i,-1,1], sistema.posis[i,-1,2], f'  P{i}', fontsize=10)

    ax.legend()
    plt.show()

    plt.ylabel('Energias')
    plt.xlabel('Tempo')
    plt.plot(np.arange(0,t,h), sistema.Ecin[:-1], label='Energia cinética')
    plt.plot(np.arange(0,t,h), sistema.Epot[:-1], label='Energia potencial')
    plt.legend()
    plt.show()
    plt.plot(np.arange(0,t,h), sistema.Ecin[:-1]+sistema.Epot[:-1])
    plt.ylabel('Energia total')
    plt.xlabel('Tempo')
    plt.title('Energia mecânica x tempo')
    plt.show()



if obj == 1:
    plt.style.use(["science","no-latex"])
    sistema.evolui_temporalmente(t,h,leapfrog=True)
    plt.plot(np.arange(0,t,h), sistema.Epot[:-1]+sistema.Ecin[:-1], label='Leapfrog')
    sistema = Sistema(corpos)
    sistema.evolui_temporalmente(t,h,leapfrog=False)
    plt.plot(np.arange(0,t,h), sistema.Epot[:-1]+sistema.Ecin[:-1], label='Euler')
    plt.ylabel('Energia total')
    plt.xlabel('Tempo')
    plt.legend()
    plt.title('Energia mecânica x tempo')
    plt.show()
# energia total
# mpl.rcParams.update(mpl.rcParamsDefault)
if obj == 2:
    

    plt.style.use(["science","no-latex"])
    for h in [50,100,150,1000,1500]:
        print(f'Calculando para passo h={h}...')
        sistema = Sistema(corpos)
        sistema.evolui_temporalmente(t,h)
        plt.plot(np.arange(0,t,h), (sistema.Ecin+sistema.Epot)[:len(np.arange(0,t,h))], label=f'Passo h={h}')
    plt.legend()
    plt.ylabel('Energia total')
    plt.xlabel('Tempo')
    plt.ylim(-4e-5,4e-5)
    plt.show()
    for h in [50,100,150]:
        print(f'Calculando para passo h={h}...')
        sistema = Sistema(corpos)
        sistema.evolui_temporalmente(t,h)
        plt.plot(np.arange(0,t,h), (sistema.Ecin)[:len(np.arange(0,t,h))], label=f'Energia cinética - Passo h={h}')
        plt.plot(np.arange(0,t,h), -(sistema.Epot)[:len(np.arange(0,t,h))], label=f'Energia potencial - Passo h={h}')
    plt.ylabel('Energia')
    plt.xlabel('Tempo')
    plt.legend()
    #plt.ylim(-4e-5,4e-5)
    plt.show()
    print(corpos)

if obj == 3:
    ax = plt.figure().add_subplot(projection='3d')
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Z')
    sistema.evolui_temporalmente(t,h,leapfrog=True)
    for i in range(n):
        ax.plot(sistema.posis[i,:-1,0],sistema.posis[i,:-1,1],sistema.posis[i,:-1,2], label=f'Trajetória Partícula {i}, leapfrog=True')
        # Marca a posição final com um ponto maior
        ax.scatter(sistema.posis[i,-2,0], sistema.posis[i,-2,1], sistema.posis[i,-2,2], s=10, marker='o', linewidth=1, label = f'Posição final da partícula {i}, leapfrog=True')
        # Opcional: adiciona texto com o número da partícula próximo ao ponto final
        #ax.text(sistema.posis[i,-1,0], sistema.posis[i,-1,1], sistema.posis[i,-1,2], f'  P{i}', fontsize=10)
    sistema = Sistema(corpos)
    sistema.evolui_temporalmente(t,h,leapfrog=False)
    for i in range(n):
        ax.plot(sistema.posis[i,:-1,0],sistema.posis[i,:-1,1],sistema.posis[i,:-1,2], label=f'Trajetória Partícula {i}, leapfrog=False')
        # Marca a posição final com um ponto maior
        ax.scatter(sistema.posis[i,-2,0], sistema.posis[i,-2,1], sistema.posis[i,-2,2], s=10, marker='o', linewidth=1, label = f'Posição final da partícula {i}, leapfrog=False')
        # Opcional: adiciona texto com o número da partícula próximo ao ponto final
        #ax.text(sistema.posis[i,-1,0], sistema.posis[i,-1,1], sistema.posis[i,-1,2], f'  P{i}', fontsize=10)

    ax.legend()
    plt.show()
