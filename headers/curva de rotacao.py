import numpy as np
import matplotlib.pyplot as plt
from scipy.integrate import quad

rd = 0.3
M = 1
a = 1

G = 1

def fdisco(r):
    return np.exp(-r/rd)

def fbojo(r):
    return M * a / (2*np.pi*r*(r+a)**3)



dr = 0.01

def integrate(f,r):
    def function(ri):
        function = G*f(r)/(r-ri+0.001)**2
        return function

    return quad(function, 0,r)[0]

r = np.linspace(0,1,300)


disk = [np.sqrt(ri*integrate(fdisco,ri)) for ri in r]



plt.plot(r,disk)

plt.show()
