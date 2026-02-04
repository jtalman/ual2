# Source - https://stackoverflow.com/a/78779276
# Posted by Tarik, modified by community. See post 'Timeline' for change history
# Retrieved 2026-02-04, License - CC BY-SA 4.0

from scipy.integrate import solve_ivp
import numpy as np
import matplotlib.pyplot as plt

def calc_a(t, c): 
   return c*np.exp(t)

def dydt(t, y, c1, c2):
    a1 = calc_a(t, c1)
    a2 = calc_a(t, c2)
    y1, y2 = y[0], y[1]
    dy1dt = a1*y2 + y1
    dy2dt = a2*y1 + y2
    return dy1dt, dy2dt

t0 = 0.0 
tf = 10.0
y1_0 = 0.02
y2_0 = 0.01

c1 = 1E-10
c2 = 2E-10

t_eval = np.linspace(t0, tf, 1000)
result = solve_ivp(dydt, (t0, tf), [y1_0, y2_0], t_eval=t_eval, method='Radau', args=(c1, c2))

plt.figure()
plt.plot(result.t, result.y[0], label='y1')
plt.plot(result.t, result.y[1], label='y2')
plt.legend()
plt.grid()
plt.xlabel('Time')
plt.ylabel('Y Values')
plt.show()

plt.figure()
a1 = calc_a(t_eval, c1)
a2 = calc_a(t_eval, c2)
plt.plot(t_eval, a1, label='a1')
plt.plot(t_eval, a2, label='a2')
plt.legend()
plt.grid()
plt.xlabel('Time')
plt.ylabel('a1 and a2 Values')
plt.show()
