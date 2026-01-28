from sympy import *

p,r = symbols('p,r')
rd, rmax = symbols('rd, R')

print(integrate(exp(-r/rd)*r,r))