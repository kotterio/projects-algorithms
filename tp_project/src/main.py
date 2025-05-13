import sys
import double_pen
import animation
import numpy as np
import argparse
import sanimation

#0, 20, 1000, 9.8, 1, 10, np.pi/2,0,(3/2)*np.pi/2,0,0,5,0,5
parser = argparse.ArgumentParser(description="parsing")
parser.add_argument('-g','--g', type=float, required=True, help='Acceleration of free fall')
parser.add_argument('-m', '--m',  type=float, required=True, help='Weight')
parser.add_argument('-k','--k',  type=float, required=True, help='Coefficient of tension of the spring')
parser.add_argument('-theta1', '--theta1', type=float, required=True, help='The angle of deviation from the vertical of the first load in degrees')
parser.add_argument('-r1','--r1', type=float, required=True, help='Initial spring tension for the first load')
parser.add_argument('-theta2', '--theta2', type=float, required=True, help='The angle of deviation from the vertical of the second load in degrees')
parser.add_argument('-r2','--r2', type=float, required=True, help='Initial spring tension for the second load')
parser.add_argument('-w1', '--w1', type=float, required=True, help='Initial angular velocity of the first load')
parser.add_argument('-w2', '--w2', type=float, required=True, help='Initial angular velocity of the second load')
parser.add_argument('-v1', '--v1', type=float, required=True, help='Initial linear velocity of the first load')
parser.add_argument('-v2','--v2', type=float, required=True, help='Initial linear velocity of the second load')
args = parser.parse_args()
v1 = args.v1
v2 = args.v2
w2 = args.w2
w1 = args.w1
r2 = args.r2
theta2 = args.theta2
r1 = args.r1
theta1 = args.theta1
k = args.k
m = args.m
g = args.g

pend = double_pen.DoublePendelum(0, 20, 1000, g, m, k, (np.pi/180) * theta1 ,r1 ,theta2 *(np.pi/180), r2, w1, w2, v1, v2)
pend.decision()
paint = sanimation.SAnimation(pend.ans)
paint.draw()