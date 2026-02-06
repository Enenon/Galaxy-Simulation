import matplotlib.pyplot as plt
import numpy as np

# script para plotar curvas de rotação de galáxias a partir dos dados de Lelli et al. 2016

c = open(r'C:\Users\Enenon\Documents\GitHub\Galaxy-Simulation\gui\plots\com_halo.txt').read()
data1 = []
for i in c.split('\n')[1:]:
    if len(i) > 0:
        data1.append(i.split())

c = open(r'C:\Users\Enenon\Documents\GitHub\Galaxy-Simulation\gui\plots\sem_halo.txt').read()
data2 = []
for i in c.split('\n')[1:]:
    if len(i) > 0:
        data2.append(i.split())


g1 = np.array(data1).astype(float)

x1 = g1[:,0]
y1 = g1[:,1]
plt.plot(x1,y1)

g1 = np.array(data2).astype(float)

x1 = g1[:,0]
y1 = g1[:,1]
plt.plot(x1,y1)

plt.show()