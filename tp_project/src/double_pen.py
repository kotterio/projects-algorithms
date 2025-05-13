import numpy as np
import sympy as smp
from scipy.integrate import odeint
import matplotlib.pyplot as plt
from diffrential import Diffrential



class DoublePendelum(Diffrential) :
    
    def __init__(self, a, b, c, g, m, k, the1, r1, the2, r2, w1, w2, v1, v2):
        self.g_  = g
        self.m_ = m
        self.k_ = k
        self.the1_ = the1
        self.r1_ = r1
        self.the2_ = the2
        self.r2_ = r2
        self.w1_ = w1
        self.w2_ = w2
        self.v1_ = v1
        self.v2_ = v2
        self.t_ = np.linspace(a, b, c)
        
    def decision(self):
        self._differential()
        self.ans = odeint(self._dSdt, y0=[self.the1_, self.w1_, self.the2_, self.w2_, self.r1_, self.v1_, self.r2_, self.v2_], t= self.t_)
    
    
    #  def plot_pendelum_theta1(self):
    #     plt.plot(self.ans.T[0])
    #     plt.savefig('theta1_plot.png')
    #     plt.close()
    
    # def plot_pendelum_theta2(self):
    #     plt.plot(self.ans.T[2])
    #     plt.show()
    
    # def plot_pendelum_r1(self):
    #     plt.plot(self.ans.T[4])
    #     plt.show()
    
    # def plot_pendelum_w1(self):
    #     plt.plot(self.ans.T[1])
    #     plt.show()
    
    # def plot_pendelum_w2(self):
    #     plt.plot(self.ans.T[3])
    #     plt.show()
        
    # def plot_pendelum_r2(self):
    #     plt.plot(self.ans.T[6])
    #     plt.show()
    
    # def plot_pendelum_v1(self):
    #     plt.plot(self.ans.T[5])
    #     plt.show()
        
    # def plot_pendelum_v2(self):
    #     plt.plot(self.ans.T[7])
    #     plt.show()
            
    # def get_x1y1x2y2(self):
    #     return ((1+self.ans.T[4])*np.cos(self.ans.T[0]),
    #             -(1+self.ans.T[4])*np.sin(self.ans.T[0]),
    #             (1+self.ans.T[4])*np.cos(self.ans.T[0]) + (1+self.ans.T[6])*np.cos(self.ans.T[2]),
    #             -(1+self.ans.T[4])*np.sin(self.ans.T[0])-(1+self.ans.T[6])*np.sin(self.ans.T[2])
    #     )
    
    # def  transfer(self):
    #     self.x1, self.y1, self.x2 , self.y2 =self.get_x1y1x2y2()
        
    # def plot_pendelium_x1(self):
    #     self.transfer()
    #     plt.plot(self.x1)
    #     plt.savefig('x1_plot.png')
    #     plt.close()
        
    # def plot_pendelium_x2(self):
    #     self.transfer()
    #     plt.plot(self.x2)
    #     plt.savefig('x2_plot.png')
    #     plt.close()
    
    # def plot_pendelium_y1(self):
    #     self.transfer()
    #     plt.plot(self.y1)
    #     plt.show()
    
    # def plot_pendelium_y2(self):
    #     self.transfer()
    #     plt.plot(self.y2)
    #     plt.show()
    
    
    
     
    
    # def draw(self):
    #     self.transfer()
    #     def animate(i):
    #         ln1.set_data([0, self.x1[i], self.x2[i]], [0, self.y1[i], self.y2[i]])
    #     fig, ax = plt.subplots(1,1, figsize=(8,8))
    #     ax.grid()
    #     ln1, = plt.plot([], [], 'ro--', lw=3, markersize=8)
    #     ax.set_ylim(-10, 10)
    #     ax.set_xlim(-10,10)
    #     ax.set_title("Double pendulum")
    #     ax.set_xlabel("x")
    #     ax.set_ylabel("y")
    #     ani = animation.FuncAnimation(fig, animate, frames=1000, interval=50)
    #     ani.save('Double_pen.gif',writer='pillow',fps=50)
    
       
        
        
        

