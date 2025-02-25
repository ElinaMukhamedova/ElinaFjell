import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import csv
import os

advection_equation_main_directory_path = os.getcwd()
project_directpry_path = os.path.dirname(advection_equation_main_directory_path)

with open('resources_for_animation/upwind_scheme/1_withoutTime.csv', newline = '') as file:
    reader = csv.reader(file, quoting=csv.QUOTE_NONNUMERIC)
    data = list(reader)

fig, ax = plt.subplots()
t = np.linspace(0, 18, 61)
x = np.linspace(0, 20, 41)

def analytical(t):
    return np.sin(4*np.pi*(x-t)/20)

line_analytical = ax.plot(x, analytical(0), 'b.--', label='analytical solution')[0]
line = ax.plot(x, data[0], 'ro--', label='upwind scheme')[0]

ax.set(xlim=[0, 20], ylim=[-1.1, 1.1], xlabel='x', ylabel='u(x,t)', title='Courant number = 0.6')
ax.legend(loc='upper right')


def update(frame):
    line.set_xdata(x)
    line.set_ydata(data[frame])
    line_analytical.set_xdata(x)
    line_analytical.set_ydata(analytical(t[frame]))
    return (line, line_analytical)

anima = animation.FuncAnimation(fig=fig, func=update, frames=61, interval=300)
anima.save(project_directpry_path+'/results/advection_equation/upwind_1.gif', writer='pillow')
plt.show()