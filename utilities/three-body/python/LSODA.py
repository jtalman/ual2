import numpy as np
from scipy.integrate import solve_ivp

# 1. Define derivative function
def der_state(t, y):
    # Example: Simple exponential decay
    return -0.5 * y

# 2. Set initial conditions and time points
state0 = [1.0]
tf = 10
T = np.linspace(0, tf, 100) # Times to evaluate

# 3. Solve
sol = solve_ivp(der_state, (0, tf), state0, t_eval=T, method="LSODA")

# 4. Access results
print(sol.y)
