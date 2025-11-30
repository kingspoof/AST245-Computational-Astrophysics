import numpy as np

## This cell defines the example function as well as their respective comain generators
def step_function(x):
    return np.where((0.4 <= x) & (x <= 0.6), 2, 1)

def gaussian_function(x):
    sigma = 0.1
    return 1 + np.exp(-(x-0.5)**2/(sigma**2))
    
def create_step_domain(N):
    x_values = np.linspace(0, 1, N + 1)
    y_values = np.ones(N + 1)
    y_values[int(N*0.4):int(N*0.6)] = 2
    return x_values, y_values

def create_gaussian_domain(N):
    x_values = np.linspace(0, 1, N + 1)
    y_values = gaussian_function(x_values)
    return x_values, y_values