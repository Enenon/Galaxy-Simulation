import matplotlib.pyplot as plt
import numpy as np
from random import random
from scipy import integrate

n = 100000
ndivisoes = 100

x = np.linspace(0,1,ndivisoes)

r = np.zeros(ndivisoes)

def pbojo(r):
     return 1/(2*np.pi*r*(r+1)**3)

def pbojo_inv():
    p = random()
    return np.sqrt(p)/(1-np.sqrt(p))
rx = []
ry = []

for i in range(n):
    numrnd = pbojo_inv()
    angrnd = 2*random()*np.pi
    rx.append(numrnd*np.cos(angrnd))
    ry.append(numrnd*np.sin(angrnd))

    indice = int(60*numrnd / ndivisoes)
    if indice > ndivisoes-1: continue
    r[indice] += 1

fig, ax = plt.subplots(nrows=1, ncols=1)

ax.set_facecolor('black')


ax.set_xlim([-2000,2000])
ax.set_ylim([-2000,2000])
ax.scatter(rx,ry,s = 0.1,c='w')
plt.show()
plt.plot(x[:int(500/ndivisoes)],r[:len(x[:int(500/ndivisoes)])])
plt.show()
