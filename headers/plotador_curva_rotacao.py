import matplotlib.pyplot as plt
import numpy as np


import matplotlib.pyplot as plt
import numpy as np

local_sparc = r'c:\Users\Enenon\Downloads\MassModels_Lelli2016c.mrt.txt'
local_cdr = r'C:\Users\Enenon\Documents\GitHub\Galaxy-Simulation\gui\data\curvas de rotacao'
local_plot = r'C:\Users\Enenon\Documents\GitHub\Galaxy-Simulation\gui\plots'

def ler_SPARC(file,galaxy_name):
    c = open(file).read()
    data = []
    for i in c.split('\n')[25:]:
        if len(i) > 0:
            data.append(i.split())
    
    galaxy = np.array([[float(j) for j in i[1:]] for i in data if i[0] == galaxy_name])

    return np.array(galaxy)

def ler_curva(file,rlimit=None):
    c = open(file).read()
    data = []
    for i in c.split('\n')[1:]:
        if len(i) > 0:
            if rlimit is not None:
                if float(i.split()[0]) > rlimit:
                    continue
            data.append(i.split())
    
    curva = np.array(data).astype(float)


    return curva
    

c1 = ler_SPARC(local_sparc,'UGC11455')
c2 = ler_curva(local_cdr + r'\sem halo.txt',rlimit=43)
c3 = ler_curva(local_cdr + r'\com halo.txt',rlimit=43)
l = ler_curva(local_plot + r'\velxraio_0.txt')






plt.plot(c1[:,1],c1[:,5]*0.7)
plt.scatter(c1[:,1],c1[:,2],s=2)
plt.plot(c2[:,0],c2[:,1])
plt.scatter(c3[:,0],c3[:,1],s=2)
#plt.scatter(l[:,0],l[:,1],s=2)
plt.legend(['SPARC - contribuição do disco (M/L=0.7)','SPARC - velocidade observada','Simulação sem halo','Simulação com halo'])

plt.show()