#%%
import numpy as np
from scipy.optimize import bisect
import matplotlib.pyplot as plt

dir = 'C:\\Users\\Enenon\\Documents\\GitHub\\Galaxy-Simulation\\gui\\data\\'

def mass_nfw(r, rs):
    """Massa acumulada NFW (sem as constantes 4*pi*rho0)"""
    x = r / rs
    return np.log(1 + x) - (x / (1 + x))

def nfw(r,rs):
    return 1/((r/rs)*(1 + r/rs)**2)

def achap0(f,rmax,rs):
    M = f(rmax,rs)
    return 1/M

rmax = 1

n = int(5e4)
r = np.linspace(0,rmax,n)

#%% disco galático

def Pdisk(r):
    norm = 1 - np.exp(-rmax/rd)*(1 + rmax/rd)
    p = 1 - np.exp(-r/rd)*(1 + r/rd)
    return p/norm
def mdisk(r):
    return r*np.exp(-r/rd)



rd = 0.25


#r = np.logspace(np.log10(1e-6),np.log10(rmax),n)



p = Pdisk(r)

plt.plot(r,p)
plt.plot(r,np.exp(-r/rd))
plt.show()



#np.savetxt(dir + 'disk_table.txt', np.column_stack((p, r)), header='p ,r')
print('Tabela do disco salva!')


#%%

#print(p)

lhist = np.histogram(p,40)
print(lhist)
fig, ax = plt.subplots()

#ax.hist(p,bins=40)

plt.plot(lhist[1][:-1],lhist[0])
plt.show()



#%% Halo
rmax_halo = 1
rs = 1

p0 = achap0(mass_nfw,rmax_halo,rs)

p = p0*mass_nfw(r*rmax_halo,rs)

np.savetxt(dir + 'halo_table.txt', np.column_stack((p, r*rmax_halo)), header='p ,r')

fig, ax = plt.subplots()

#ax.hist(p,bins=40)
plt.plot(r*rmax_halo,p)
#plt.plot(r*rmax_halo,nfw(r*rmax_halo,rs))
plt.show()
# %%
