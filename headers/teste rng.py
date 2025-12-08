import matplotlib.pyplot as plt
import numpy as np
from random import random
from scipy import integrate

rd = 110
n = 100000
ndivisoes = 50
p0 = 1
p0 = 1/(1-1/np.exp(1))


x = np.linspace(0,rd,ndivisoes)
def calc_p0(rd):
    return 1/(rd*(1-np.exp(-1)))

def p(r):
	return np.exp(-r/rd)

def pr(r,rd):
     return np.exp(-r/rd)

def pbojo(r):
     return 1/(2*np.pi*r*(r+1)**3)

def pbojo_inv():
    p = random()
    return np.sqrt(p)/(1-np.sqrt(p))

p0 = calc_p0(rd)

print(calc_p0(rd),calc_p0(1))

print(p0*integrate.quad(p,0,rd)[0]) # checando se p0 realmente é a normalização dessa função


def draw_random_number_from_pdf(pdf, interval, pdfmax = 1, integers = False, max_iterations = 10000):
        for i in range(max_iterations):
            if integers == True:
                rand_x = np.random.randint(interval[0], interval[1])
            else:
                rand_x = (interval[1] - interval[0]) * np.random.random(1) + interval[0] #(b - a) * random_sample() + a

            rand_y = pdfmax * np.random.random(1) 
            calc_y = pdf(rand_x)

            if(rand_y <= calc_y ):
                return rand_x
        raise ValueError("deu erro")


r = np.zeros(ndivisoes)
r2 = np.zeros(ndivisoes)

# --- abordagem 1: utilizando a função do stackoverflow

p0 = calc_p0(1)

for i in range(n):
    numrnd = draw_random_number_from_pdf(p,[0,rd])[0]
    indice = int(numrnd / rd * ndivisoes)
    r[indice] += 1

# --- abordagem 2: usando inversa da PDF

p0 = calc_p0(1) # nessa abordagem, vai ser utilizado o raio de 0 a 1, e o raio real vai ser multiplicado ao rng (0,1] gerado

for i in range(n):
    numrnd = - rd* ( np.log(1 - random()/p0) )
    indice = int(numrnd / rd * ndivisoes)
    if indice > ndivisoes-1: continue
    r2[indice] += 1

# --- validação

p0 = calc_p0(rd)
print(n*p0*rd/ndivisoes) # essa é a quantidade de partículas dispersas no começo; rd/ndivisoes = Δr

print(sum(r),sum(r2))

plt.plot(x,r)
plt.plot(x,r2)
plt.plot(x,p0*n*rd*np.exp(-x/rd)/ndivisoes)
plt.legend(['Função desconhecida','Método da inversa acumulada','Valor real'])
plt.show()
