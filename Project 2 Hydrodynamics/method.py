import numpy as np


def gaussian_domain(n):
    x = np.linspace(0, 1, n)
    f_values = np.zeros_like(x)
    
    for i in range(n):
        f_values[i] = 1 + np.exp(-(x[i]-0.5)**2 / 0.1**2)
    
    return x, f_values  # Keep coordinates separate!

def step_domain(n):
    x = np.linspace(0, 1, n)
    domain = np.ones(n)
    domain[int(n*0.4):int(n*0.6)] = 2
    return x, domain