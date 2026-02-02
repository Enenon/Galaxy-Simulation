mSol = 1
AL = 1
mil = 1
N = 1

m = 1.1e-16*AL
kpc = 3.27e3*AL
kg = 1/2 * 1e-30 * mSol
s = 1/3.1536 * 1e-10 * mil

# m = 1.1
# kg = 1/2
# s = 1/3.156

G = 6.67e-11*(m**3)/(kg*s**2)

i = -16*3 - (-30) - (-10)*2

# G = G*10**(int(i))
#print(G)
N = kg * m / s**2

G = 6.67e-11 * N * m**2 / kg**2

#print(G)

km = 1.06e-13*AL

v  = s/km
print('1AL/mil =', v, 'km/s')