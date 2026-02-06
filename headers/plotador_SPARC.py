import matplotlib.pyplot as plt
import numpy as np

# script para plotar curvas de rotação de galáxias a partir dos dados de Lelli et al. 2016


c = open(r'c:\Users\Enenon\Downloads\MassModels_Lelli2016c.mrt.txt').read()
data = []
for i in c.split('\n')[25:]:
    if len(i) > 0:
        data.append(i.split())


galaxy = np.array([[float(j) for j in i[1:]] for i in data if i[0] == 'UGC11455'])

x = galaxy[:,1]
y = galaxy[:,2]

y1 = galaxy[:,4]
y2 = galaxy[:,5]
y3 = galaxy[:,6]

plt.plot(x,y)
plt.plot(x,y1)
plt.plot(x,y2*0.8)
plt.plot(x,y3)
plt.legend(['Velocidade observada','Contribuição do gás','Contribuição do disco','Contribuição do bojo'])
plt.title('Curva de rotação - UGC11455 (SPARC)')
plt.xlabel('Raio (kpc)')
plt.ylabel('Velocidade (km/s)')

plt.show()