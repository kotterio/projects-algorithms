
import sympy as smp

class Diffrential():

    def _dSdt(self, S, t):
        the1, w1, the2, w2, r1, v1, r2, v2 = S
        return [
            self.dthe1dtf(w1),
            self.dw1dtf(self.m_,self.k_,self.g_,the1,the2,r1,r2,w1,w2,v1,v2),
            self.dthe2dtf(w2),
            self.dw2dtf(self.m_,self.k_,self.g_,the1,the2,r1,r2,w1,w2,v1,v2),
            self.dr1dtf(v1),
            self.dv1dtf(self.m_,self.k_,self.g_,the1,the2,r1,r2,w1,w2,v1,v2),
            self.dr2dtf(v2),
            self.dv2dtf(self.m_,self.k_,self.g_,the1,the2,r1,r2,w1,w2,v1,v2),
        ]    
    
    def _differential(self) :
        self.t, self.m, self.g, self.k = smp.symbols('t m g k')
        self.the1, self.the2, self.r1, self.r2 = smp.symbols(r'\theta_1, \theta_2, r_1, r_2', cls=smp.Function)

        # theta1
        self.the1 = self.the1(self.t)
        the1_d = smp.diff(self.the1, self.t)
        the1_dd = smp.diff(the1_d, self.t)

        self.the2 = self.the2(self.t)
        the2_d = smp.diff(self.the2, self.t)
        the2_dd = smp.diff(smp.diff(self.the2, self.t), self.t)

        self.r1 = self.r1(self.t)
        r1_d = smp.diff(self.r1, self.t)
        r1_dd = smp.diff(smp.diff(self.r1, self.t), self.t)

        self.r2 = self.r2(self.t)
        r2_d = smp.diff(self.r2, self.t)
        r2_dd = smp.diff(smp.diff(self.r2, self.t), self.t)
        
        
        self.x1, self.y1, self.x2, self.y2 = smp.symbols('x_1, y_1, x_2, y_2', cls=smp.Function)
        self.x1= self.x1(self.the1, self.r1)
        self.y1= self.y1(self.the1, self.r1)
        self.x2= self.x2(self.the1, self.r1, self.the2, self.r2)
        self.y2= self.y2(self.the1, self.r1, self.the2, self.r2)
        
        self.x1 = (1+self.r1)*smp.cos(self.the1)
        self.y1 = -(1+self.r1)*smp.sin(self.the1)
        self.x2 = (1+self.r1)*smp.cos(self.the1) + (1+self.r2)*smp.cos(self.the2)
        self.y2 = -(1+self.r1)*smp.sin(self.the1)-(1+self.r2)*smp.sin(self.the2)
        
        
        T = 1/2 * self.m * (smp.diff(self.x1, self.t)**2 + smp.diff(self.y1, self.t)**2 + \
               smp.diff(self.x2, self.t)**2 + + smp.diff(self.y2, self.t)**2)
        V = self.m *self.g *self.y1 + self.m *self.g * self.y2 + 1/2 * self.k * self.r1**2 + 1/2 * self.k * self.r2**2
        L = T-V
        
        LE1 = smp.diff(L, self.the1) - smp.diff(smp.diff(L, the1_d), self.t)
        LE1 = LE1.simplify()
        
        LE2 = smp.diff(L, self.the2) - smp.diff(smp.diff(L, the2_d), self.t)
        LE2 = LE2.simplify()
        
        LE3 = smp.diff(L, self.r1) - smp.diff(smp.diff(L, r1_d), self.t)
        LE3 = LE3.simplify()
        
        LE4 = smp.diff(L, self.r2) - smp.diff(smp.diff(L, r2_d), self.t)
        LE4 = LE4.simplify()
        
        sols = smp.solve([LE1, LE2, LE3, LE4], (the1_dd, the2_dd, r1_dd, r2_dd),
                simplify=False, rational=False)
        self.dw1dtf = smp.lambdify((self.m, self.k ,self.g, self.the1, self.the2, self.r1, self.r2, the1_d, the2_d, r1_d, r2_d), sols[the1_dd])
        self.dthe1dtf = smp.lambdify(the1_d, the1_d)

        self.dw2dtf = smp.lambdify((self.m, self.k ,self.g, self.the1, self.the2,  self.r1, self.r2, the1_d, the2_d, r1_d, r2_d), sols[the2_dd])
        self.dthe2dtf = smp.lambdify(the2_d, the2_d)

        self.dv1dtf = smp.lambdify((self.m, self.k ,self.g, self.the1, self.the2, self.r1, self.r2, the1_d, the2_d, r1_d, r2_d), sols[r1_dd])
        self.dr1dtf = smp.lambdify(r1_d, r1_d)

        self.dv2dtf = smp.lambdify((self.m, self.k ,self.g, self.the1, self.the2, self.r1, self.r2, the1_d, the2_d, r1_d, r2_d), sols[r2_dd])
        self.dr2dtf = smp.lambdify(r2_d, r2_d)