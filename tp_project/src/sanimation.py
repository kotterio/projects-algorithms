import matplotlib.pyplot as plt
from matplotlib import animation
import numpy as np
from matplotlib.animation import PillowWriter
from IAnimation import IAnimation

class SAnimation(IAnimation) :
    def __init__(self, ans):
        self.ans = ans
        
    
    def _get_x1y1x2y2(self):
        return ((1+self.ans.T[4])*np.cos(self.ans.T[0]),
                -(1+self.ans.T[4])*np.sin(self.ans.T[0]),
                (1+self.ans.T[4])*np.cos(self.ans.T[0]) + (1+self.ans.T[6])*np.cos(self.ans.T[2]),
                -(1+self.ans.T[4])*np.sin(self.ans.T[0])-(1+self.ans.T[6])*np.sin(self.ans.T[2])
        )
    
    def  _transfer(self):
        self.x1, self.y1, self.x2 , self.y2 =self._get_x1y1x2y2()
    
    def draw(self):
        self._transfer()
        def animate(i):
            ln1.set_data([0, self.x1[i], self.x2[i]], [0, self.y1[i], self.y2[i]])
        fig, ax = plt.subplots(1,1, figsize=(8,8))
        ax.grid()
        ln1, = plt.plot([], [], 'ro--', lw=3, markersize=8)
        ax.set_ylim(-10, 10)
        ax.set_xlim(-10,10)
        ax.set_title("Double pendulum")
        ax.set_xlabel("x")
        ax.set_ylabel("y")
        ani = animation.FuncAnimation(fig, animate, frames=1000, interval=50)
        ani.save('../result/Double_pen.gif',writer='pillow',fps=50)
        result = open('../result/animation.html', 'w')
        result.write("""<!DOCTYPE html>
                        <html>
                        <head>
                            <title>Анимация двойного маятника</title>
                        </head>
                        <body>
                            <h1>Анимация двойного маятника</h1>
                            <img src="../result/Double_pen.gif" alt="Анимация">
                        </body>
                        </html>""")
        result.close()
        
    