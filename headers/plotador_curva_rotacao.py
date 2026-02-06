import matplotlib.pyplot as plt
import numpy as np


c = open(r'c:\Users\Enenon\Downloads\MassModels_Lelli2016c.mrt.txt').read()
data = []
for i in c.split('\n')[25:]:
    if len(i) > 0:
        data.append(i.split())