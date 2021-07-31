'''
1. 模拟一维空间的卡尔曼滤波；
2. 状态量设置为x和vx；
'''
import numpy as np
from numpy import *

# 卡尔曼滤波函数，输入状态和协方差，最终返回一个更新后的状态和协方差
def kalman_filter(X, P):
    for n in range(len(measurements)):

        # update
        z = matrix([[measurements[n]]]) #从measurement中取其中一个状态作为当时的测量;
        error = z - H*X
        s = H*P*np.transpose(H) + R
        k = P*np.transpose(H)*np.linalg.inv(s)
        X = X +k*error
        P = (I - k*H) *P

        #predict
        X = F * X +u
        P = F *P *np.transpose(F)
        print("one update")
        print(X)
        print(P)
        print("        ")
    return X, P


# 测量值;
measurements = [1, 2, 3] #分别代表t1, t2, t3时刻观察到的位置
# 初始的位置和速度;
X = matrix([[0],[0]])
# 初始的协方差矩阵
P = matrix([[1000, 0], [0, 1000]])  # 初始情况下，位置和速度的协方差都很大，而且不知道位置和速度之间是否有关联，所以都设置为0, 不确定性很高
# 状态转移矩阵
F = matrix([[1, 1], [0 , 1]])
# 外部扰动
u = matrix([[0],[0]])
# 观测矩阵
H = matrix([1.0, 0.0])   #将状态投影到观测
# 测量噪声矩阵
R = matrix([[10000]])  #测量的不确定性
# 单位矩阵
I = matrix([[1, 0], [0, 1]])


print(kalman_filter(X, P))
