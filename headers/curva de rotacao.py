import numpy as np
import matplotlib.pyplot as plt
from scipy.integrate import quad, dblquad

rmax = 2
rd = 1
M = 1
a = 1
p0NFW = 1
Rs = 1

G = 1

def fdisco(r):
    return np.exp(-r/rd)

def fbojo(r):
    return M * a / (2*np.pi*r*(r+a)**3)

def fNFW(r):
    p = p0NFW*Rs/((r+0.000001)*(1 + r/Rs)**2)
    return p

dr = 0.01

def integrate(f,r):
    def function(ri):
        function = G*f(ri)/(r-ri+0.001)**2
        return function

    return quad(function, 0,r)[0]

def integratedisk(f,r):
    function = lambda ri, o: G*f(ri)*np.cos(o)/((r + ri*np.cos(o))**2 + (ri*np.sin(o))**2)

    return dblquad(function, 0, r, 0, 2*np.pi)[0]

r = np.linspace(0,rmax,300)


disk = [np.sqrt(ri*integratedisk(fdisco,ri)) for ri in r]


plt.plot(r,disk)

plt.show()

exit()

plt.plot(r,r*r*fNFW(r))

plt.show()