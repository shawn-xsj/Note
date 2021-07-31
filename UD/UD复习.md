## UD复习

## 卡尔曼滤波

## c++版本的卡尔曼滤波

1. 两个高斯分布的乘积仍然是一个高斯分布，其均值在两个高斯分布中间，方差要比两个高斯分布都小，均值处的峰值要比两个高斯分布都高;![新的高斯分布](%E5%9B%BE%E7%89%87/%E6%96%B0%E7%9A%84%E9%AB%98%E6%96%AF%E5%88%86%E5%B8%83.png)

   

   

2. 卡尔曼滤波由两部分组成，一个是测量更新(update)，一个是运动更新（predict），测量更新采用乘法实现，类似于贝叶斯方程， 运动更新采用全概率或者一个加法实现；

   ![t2](%E5%9B%BE%E7%89%87/t2.png)

3. ==运动是有误差的，你让自己运动一个状态，自己运动的最终结果不一定完全等于你开始时的目标状态== ， 新的目标状态如何计算？运动更新阶段只是将运动的高斯分布加到原来的高斯分布中;![motion update](%E5%9B%BE%E7%89%87/motion%20update.png)

   ![motion update](motion%20update.png)

4. 卡尔曼滤波的一个很重要的应用就是，即使测量没有直接测到某个状态量，我们也可以利用卡尔曼滤波得到它;

5. 从一维高斯分布变成多维高斯分布时，==方差变成协方差==

6. **==协方差定义了分布的集中度??==**  ==协方差是斜的，意味着两个变量之间有相互影响==

   ![协方差定义了分布的集中度](%E5%9B%BE%E7%89%87/%E5%8D%8F%E6%96%B9%E5%B7%AE%E5%AE%9A%E4%B9%89%E4%BA%86%E5%88%86%E5%B8%83%E7%9A%84%E9%9B%86%E4%B8%AD%E5%BA%A6.png)

7. 位置与速度之间存在这关联，如何由先验和测量得到较小的方差分布的结果？

   ![速度与位置存在关系](%E9%80%9F%E5%BA%A6%E4%B8%8E%E4%BD%8D%E7%BD%AE%E5%AD%98%E5%9C%A8%E5%85%B3%E7%B3%BB.png)

   ![速度与位置存在关系](%E5%9B%BE%E7%89%87/%E9%80%9F%E5%BA%A6%E4%B8%8E%E4%BD%8D%E7%BD%AE%E5%AD%98%E5%9C%A8%E5%85%B3%E7%B3%BB.png)

8. 卡尔曼滤波的状态量分为两类，一种是直接测量就能得到的，一种是隐藏的；但是两种状态量之间存在关联，通过观察到的状态可以推出隐藏的状态量;

   ![状态量分为两类](%E5%9B%BE%E7%89%87/%E7%8A%B6%E6%80%81%E9%87%8F%E5%88%86%E4%B8%BA%E4%B8%A4%E7%B1%BB.png)

9. 基本公式

   ![公式](%E5%85%AC%E5%BC%8F.png)![公式](%E5%9B%BE%E7%89%87/%E5%85%AC%E5%BC%8F.png)

10. 一维kalman filter

    ```python
    '''
    1. 模拟一维空间的卡尔曼滤波；
    2. 状## c++版本的卡尔曼滤波
    
    态量设置为x和vx；
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
        ## c++版本的卡尔曼滤波
    
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
    u = matrix([[0],[0]])  # 假设为匀速运动，加速度为0， 如果是匀加速或者匀减速运动，u就是加速度分量
    # 观测矩阵
    H = matrix([1.0, 0.0])   #将状态投影到观测
    # 测量噪声矩阵
    R = matrix([[1]])  #测量的不确定性
    # 单位矩阵
    I = matrix([[1, 0], [0, 1]])
    
    
    print(kalman_filter(X, P))
    ```

    > one update
    > [[0.999001]                                   # 拿到第一个测量，进行predict，此时预测出来的速度还是0， 下一个的位置是1，此时速度和位置关联性已经很大
    > [0.      ]]
    > [[1000.999001 1000.      ]
    > [1000.       1000.      ]]
    >
    > one update   #拿到第二个观测量，2， 此时预测出下一步的位置是3， 速度是1， 已经很准了
    > [[2.99800299]
    > [0.999002  ]]
    > [[4.99002494 2.99301795]
    > [2.99301795 1.99501297]]
    >
    > one update                    #拿到第三个测量，3， 预测出下一步的位置是4， 速度是1，准；
    > [[3.99966644]
    > [0.99999983]]
    > [[2.33189042 0.99916761]
    > [0.99916761 0.49950058]]

11. numpy 中matrix包含于array

12. 协方差和协方差 系数： https://www.zhihu.com/question/20852004

    > 你变大，同时我也变大，说明两个变量是同向变化的，这时协方差就是正；
    >
    > 你变大，同时我变小，说明两个变量是反向变化的，这时协方差就是负；
    >
    > 从数值上看，协方差的数值越大，两个变量同向变化的程度也就越大，反之亦然；

13. Q矩阵

    **Q矩阵表征的是无法用状态转移矩阵进行表示的噪声，比如车辆运动时突然上坡。Q对整个系统存在影响，但又不能太确定对心痛的影响有多大。工程上一般将Q设置为单位矩阵参与运算**

    ![Q矩阵](Q%E7%9F%A9%E9%98%B5.png)## c++版本的卡尔曼滤![Q矩阵](%E5%9B%BE%E7%89%87/Q%E7%9F%A9%E9%98%B5.png)

    ## c++版本的卡尔曼滤波

    1. 几何概念介绍

       > https://www.mathwarehouse.com/trigonometry/
       >
       > https://classroom.udacity.com/nanodegrees/nd013/parts/40f38239-66b6-46ec-ae68-03afd8a601c8/modules/87f3782a-0a5b-4568-bcf2-edad2f5fdd76/lessons/60367cb6-526f-4255-a92a-c850038c4675/concepts/5640101a-453e-444a-ab2e-d0554396e36c

    2. 激光雷达和毫米波雷达融合的框架图

       ![激光雷达和毫米波雷达融合的框架图1](%E5%9B%BE%E7%89%87/%E6%BF%80%E5%85%89%E9%9B%B7%E8%BE%BE%E5%92%8C%E6%AF%AB%E7%B1%B3%E6%B3%A2%E9%9B%B7%E8%BE%BE%E8%9E%8D%E5%90%88%E7%9A%84%E6%A1%86%E6%9E%B6%E5%9B%BE1.png)

    3. update（更新）: 比较预测的结果和传感器测量的结果，将两者进行结合得到更新后的结果。预测和更新的比重谁占的大，取决于两者的不确定性；

    4. 如果有两个传感器的测量数据同时来了，如何处理？重复两步

       ![两个传感器的数据同时到](%E4%B8%A4%E4%B8%AA%E4%BC%A0%E6%84%9F%E5%99%A8%E7%9A%84%E6%95%B0%E6%8D%AE%E5%90%8C%E6%97%B6%E5%88%B0.png)

    5. 匀速的卡尔曼滤波公式

       ![匀速卡尔曼公式](%E5%8C%80%E9%80%9F%E5%8D%A1%E5%B0%94%E6%9B%BC%E5%85%AC%E5%BC%8F.png)

    6. 预测过程的直观理解: Q 是过程噪声，是对v的不确定度的表征;

       ![预测过程的直观理解](%E9%A2%84%E6%B5%8B%E8%BF%87%E7%A8%8B%E7%9A%84%E7%9B%B4%E8%A7%82%E7%90%86%E8%A7%A3.png)

    7. 状态转移方程中将bu=0的原因： 模型不可表征的部分全部放到了v中，该v服从高斯分布，均值为v，方差为Q

       ![BU为0的原因](BU%E4%B8%BA0%E7%9A%84%E5%8E%9F%E5%9B%A0-1627190877731.png)

    8. v 为0的原因: 状态方程处理的都是均值，不处理方差，噪声的均值设为0， 所以这里不参与运算, 相当与噪声全到协方差运算里面去了；

       > Looking closely at the process noise, we know from the Kalman Filter algorithm that its mean is zero and its covariance matrix is usually noted by Q * N(0,Q)*Q*∗*N*(0,*Q*). The first equation only predicts the mean state. As the mean value of the noise is zero, it does not directly affect the predicted state. However, we can see that the noise covariance Q*Q* is added here to the state covariance prediction so that the state uncertainty always increases through the process noise.

       ![v设置为0的原因](v%E8%AE%BE%E7%BD%AE%E4%B8%BA0%E7%9A%84%E5%8E%9F%E5%9B%A0.png)

    9. 卡尔曼滤波中是先预测还是先更新，两者的顺序并没有太大的影响:

       > Over time, the order of these doesn't have a huge impact, since it is just a cycle from one to the other. 

    10. 预测的时间间隔增强，不确定性会提高： 1）Q矩阵中有与时间相关的量；2）F矩阵中也有与时间相关的量; 3) 想象下将时间进行切片，进行多次预测，但是没有更新，这样不确定度就会逐步提高;

    11. 过程噪声受到两方面的影响，一方面是时间差，一方面是模型之外的加速度等信息；

        > From the examples I’ve just showed you we can clearly see that the process noise depends on both: the elapsed time and the uncertainty of acceleration. So, how can we model the process noise by considering both of these factors? Keep going to find out :)

    12. ==Q矩阵的推导没有懂==，==R矩阵也有类似的问题==，

        > https://mp.weixin.qq.com/s?__biz=MzI1ODYwOTkzNg==&mid=2247508342&idx=2&sn=a701c65ed5b5f92f6342ec33b2ea3c8c&chksm=ea076709dd70ee1f3fe318ee446649173a33f7dd2e28f9804c39e343f20de5bccd82f453fb25&mpshare=1&scene=1&srcid=0220DoPOR0CZDTKVgvobFDRL&sharer_sharetime=1613783463771&sharer_shareid=def5d1aee922d88fa623ca1b2829e1b1&exportkey=AzSVmNVaVNURmrtGbLbGqls%3D&pass_ticket=iyHntoH38n5h0fEKwEtMpyGNmmY8acj76sGVADlrFzXB%2BxvPwOQXHalukuVgreDj&wx_header=0#rd
        >
        > https://classroom.udacity.com/nanodegrees/nd013/parts/40f38239-66b6-46ec-ae68-03afd8a601c8/modules/3a104c73-0af4-4d29-9c26-6af49862a232/lessons/3612b91d-9c33-47ad-8067-a572a6c93837/concepts/1ac6e0ac-1809-4864-b58f-870d6bda9b25

        ![R矩阵的问题](R%E7%9F%A9%E9%98%B5%E7%9A%84%E9%97%AE%E9%A2%98.png)

    13. R矩阵为方正，维数为测量状态的个数;比如激光雷达测量的是位置xy, 那么R矩阵就应该是二维的方阵，R矩阵中的0代表着不相关;

    14. 激光雷达跟踪的效果图

        ![卡尔曼滤波器的效果和R矩阵的影响](%E5%8D%A1%E5%B0%94%E6%9B%BC%E6%BB%A4%E6%B3%A2%E5%99%A8%E7%9A%84%E6%95%88%E6%9E%9C%E5%92%8CR%E7%9F%A9%E9%98%B5%E7%9A%84%E5%BD%B1%E5%93%8D.png)

    15. 毫米波 雷达测量量：特别注意range rate是径向的；

        > - The range, (\rho*ρ*), is the distance to the pedestrian. The range is basically the magnitude of the position vector \rho*ρ* which can be defined as \rho = sqrt(p_x^2 + p_y^2)*ρ*=*s**q**r**t*(*p**x*2+*p**y*2).
        > - \varphi = atan(p_y / p_x)*φ*=*a**t**a**n*(*p**y*/*p**x*). Note that \varphi*φ* is referenced counter-clockwise from the x-axis, so \varphi*φ* from the video clip above in that situation would actually be negative.
        > - The range rate, \dot{\rho}*ρ*˙, is the projection of the velocity, v*v*, onto the line, L*L*.

        ![毫米波雷达测量量](%E6%AF%AB%E7%B1%B3%E6%B3%A2%E9%9B%B7%E8%BE%BE%E6%B5%8B%E9%87%8F%E9%87%8F.png)

    16. 从毫米波雷达的极坐标到笛卡尔坐标系下的状态量的转换

        ![毫米波雷达的H矩阵](%E6%AF%AB%E7%B1%B3%E6%B3%A2%E9%9B%B7%E8%BE%BE%E7%9A%84H%E7%9F%A9%E9%98%B5.png)

    17. 毫米波雷达径向速度的投影计算方法

        ![毫米波雷达径向速度的投影计算方法](%E6%AF%AB%E7%B1%B3%E6%B3%A2%E9%9B%B7%E8%BE%BE%E5%BE%84%E5%90%91%E9%80%9F%E5%BA%A6%E7%9A%84%E6%8A%95%E5%BD%B1%E8%AE%A1%E7%AE%97%E6%96%B9%E6%B3%95.png)

    18. 一个变量如果本身服从高斯分布，经过一个非线性变换之后，就不再服从高斯分布;==经过线性变换还是高斯分布?==

        ![高斯分布进过非线性变换之后就不再服从高斯分布了](%E9%AB%98%E6%96%AF%E5%88%86%E5%B8%83%E8%BF%9B%E8%BF%87%E9%9D%9E%E7%BA%BF%E6%80%A7%E5%8F%98%E6%8D%A2%E4%B9%8B%E5%90%8E%E5%B0%B1%E4%B8%8D%E5%86%8D%E6%9C%8D%E4%BB%8E%E9%AB%98%E6%96%AF%E5%88%86%E5%B8%83%E4%BA%86.png)

        ![高斯分布进过非线性变换之后就不再服从高斯分布了2](%E9%AB%98%E6%96%AF%E5%88%86%E5%B8%83%E8%BF%9B%E8%BF%87%E9%9D%9E%E7%BA%BF%E6%80%A7%E5%8F%98%E6%8D%A2%E4%B9%8B%E5%90%8E%E5%B0%B1%E4%B8%8D%E5%86%8D%E6%9C%8D%E4%BB%8E%E9%AB%98%E6%96%AF%E5%88%86%E5%B8%83%E4%BA%862.png)

    19. 均值位置u的非线性函数是预测分布的最佳估算，然后围绕这个h进行的一阶导数进行运算

        ![对非线性测量和状态量的线性化处理](%E5%AF%B9%E9%9D%9E%E7%BA%BF%E6%80%A7%E6%B5%8B%E9%87%8F%E5%92%8C%E7%8A%B6%E6%80%81%E9%87%8F%E7%9A%84%E7%BA%BF%E6%80%A7%E5%8C%96%E5%A4%84%E7%90%86.png)

    20. ==泰勒公式==

        ![泰勒展开](%E6%B3%B0%E5%8B%92%E5%B1%95%E5%BC%80-1627266068824.png)

    21. 雅克比矩阵的定义

        ![雅克比矩阵的定义](%E9%9B%85%E5%85%8B%E6%AF%94%E7%9F%A9%E9%98%B5%E7%9A%84%E5%AE%9A%E4%B9%89.png)

        ![雅克比矩阵3](%E9%9B%85%E5%85%8B%E6%AF%94%E7%9F%A9%E9%98%B53.png)

        ![雅克比矩阵4](%E9%9B%85%E5%85%8B%E6%AF%94%E7%9F%A9%E9%98%B54.png)

    22. 对扩展卡尔曼滤波，首先线性化非线性预测和测量函数，然后使用与卡尔曼滤波相同的机制进行预测和更新；

    23. 毫米波雷达的y值计算直接使用h公式，计算S,K,P使用雅克比公式

        ![毫米波雷达的y值计算](%E6%AF%AB%E7%B1%B3%E6%B3%A2%E9%9B%B7%E8%BE%BE%E7%9A%84y%E5%80%BC%E8%AE%A1%E7%AE%97.png)

    24. 评价卡尔曼滤波性能最基本的方法：均方根误差（ＲＭＳＥ）

        ![均方根误差](%E5%9D%87%E6%96%B9%E6%A0%B9%E8%AF%AF%E5%B7%AE.png)

    25. ==矩阵和数组的区别==

    26. 汽车或者自行车的转弯，归结于非线性运动: 需要使用无损卡尔曼%E5%9B%BE%E7%89%87/BU%E4%B8%BA0%E7%9A%84%E5%8E%9F%E5%9B%A0.png%E5%9B%BE%E7%89%87/BU%E4%B8%BA0%E7%9A%84%E5%8E%9F%E5%9B%A0.png%E5%9B%BE%E7%89%87/BU%E4%B8%BA0%E7%9A%84%E5%8E%9F%E5%9B%A0.png

    ## 

## 无损卡尔曼滤波(UKF)

ukf是处理非线性过程模型或非线性测量模型的替代方法，但是==不会对非线性函数进行线性化处理==，使用sigma点来近似分布，其效果比线性话的过程要好，可以省去雅克比矩阵的计算过程;

### 运动模型

1. 常见运动模型

   > CV: constant velocity mode
   >
   > CTRV: constant turn rate and velocity magnitude model:恒定的转弯率和速度模型
   >
   > CTRA: constant turn rate and acceleration: 恒定的角速度和加速度模型：
   >
   > CSAV: constant steering angle and velocity: 恒定转角和速度模型;
   >
   > CCA: constant curvature and acceleration: 恒定的曲率和加速度;
   >
   > 恒定速度模型：CV----Constant Velocity
   > 恒定加速度模型：CA----Constant Acceleration
   > 恒定转弯率和速度模型：CTRV----Constant Turn Rate and Velocity
   > 恒定转弯率和加速度模型：CTRA----Constant Turn Rate and Acceleration
   > 恒定转向角和速度模型：CSAV----Constant Steering Angle and Velocity
   > 常曲率和加速度模型：CCA----Constant Curvature and Acceleration
   > ————————————————
   > 版权声明：本文为CSDN博主「泠山」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
   > 原文链接：https://blog.csdn.net/qq_28087491/article/details/109515881

2.  CTRV 模型适合真实的交通场景

   <img src="/home/saic/xsj/note2/Note/UD/CTRV模型.png" alt="CTRV模型" style="zoom: 33%;" />

   

3. CTRV模型的状态量为（位置ｘ, 位置ｙ, 速度, 偏航角, 角速度)(固定的速度和固定的角速度)

4. 状态方程求解

   <img src="/home/saic/xsj/note2/Note/UD/CTRV的状态方程求解１.png" alt="CTRV的状态方程求解１" style="zoom:33%;" />

   <img src="/home/saic/xsj/note2/Note/UD/CTRV的状态方程求解２.png" alt="CTRV的状态方程求解２" style="zoom:33%;" />

   <img src="/home/saic/xsj/note2/Note/UD/CTRV的状态方程求解３.png" alt="CTRV的状态方程求解３" style="zoom:33%;" />

5. CTRＶ模型的噪声部分：因为ctrv模型是固定的速度和角速度，因此噪声部分就是影响这两个固定量的因素，为**加速度和角加速度;**

   <img src="/home/saic/xsj/note2/Note/UD/CTRV模型的噪声部分.png" alt="CTRV模型的噪声部分" style="zoom:33%;" />

   <img src="/home/saic/xsj/note2/Note/UD/CTRV模型的噪声部分２.png" alt="CTRV模型的噪声部分２" style="zoom:33%;" />

   <img src="/home/saic/xsj/note2/Note/UD/完整的过程模型.png" alt="完整的过程模型" style="zoom:25%;" />

6. 线性系统的理解

   <img src="/home/saic/xsj/note2/Note/UD/线性系统的理解.png" alt="线性系统的理解" style="zoom: 25%;" />

7. 使用非线性系统进行预测，得到的后验系统不再符合高斯分布，其分布没法直接预测。ｕkf是假设还是高斯分布，然后去近似，尽可能的去贴近。也就是去寻找平均值和协方差与后验一致的分布;

   <img src="/home/saic/xsj/note2/Note/UD/非线性系统的理解.png" alt="非线性系统的理解" style="zoom: 25%;" />

8. sigma点的由来：偏差的总和一致;

9. sigma点的方法同样可以用于线性模型，但是计算时间会比直接用线性系统要长;

10. 三步:

    > 1) 如何生成（选择) sigma点
    >
    > 2) 如何对这些sigma点进行预测;
    >
    > 3) 根据预测的sigma点计算均值和协方差;

    <img src="/home/saic/xsj/note2/Note/UD/UKF的步骤.png" alt="UKF的步骤" style="zoom: 25%;" />

11. sigma点的个数取决于模型状态量的个数,状态变量个数为ｎ, 则sigma点的个数为２×ｎ＋１;

12. sigma点产生方法

    <img src="/home/saic/xsj/note2/Note/UD/sigma点产生方法１.png" alt="sigma点产生方法１" style="zoom:25%;" />

13. cholesky分解，计算矩阵的平方根;

14. 几个概念：过程噪声, 过程噪声协方差矩阵;

    <img src="/home/saic/xsj/note2/Note/UD/过程噪声和过程协方差矩阵.png" alt="过程噪声和过程协方差矩阵" style="zoom:25%;" />

    

15. ==采用曾广状态来表征噪声==：之所以需要扩充是因为过程噪音对状态有非线性影响

    <img src="/home/saic/xsj/note2/Note/UD/增广状态.png" alt="增广状态" style="zoom:25%;" />

16. sigma点的预测: 不包含增广的两个状态;

    <img src="/home/saic/xsj/note2/Note/UD/sigma点的预测不包括增广的状态.png" alt="sigma点的预测不包括增广的状态" style="zoom: 25%;" />

17. sigma点的预测

    <img src="/home/saic/xsj/note2/Note/UD/sigma点的预测.png" alt="sigma点的预测" style="zoom: 33%;" />

18. 计算预测的的均值和协方差

    <img src="/home/saic/xsj/note2/Note/UD/预测的均值和协方差计算公式.png" alt="预测的均值和协方差计算公式" style="zoom:25%;" />

19. 更新过程中，将predict产生的sigma点使用测量模型进行转换

    <img src="/home/saic/xsj/note2/Note/UD/更新过程的sigma点计算.png" alt="更新过程的sigma点计算" style="zoom:25%;" />

    <img src="/home/saic/xsj/note2/Note/UD/更新过程的sigma点计算２.png" alt="更新过程的sigma点计算２" style="zoom:25%;" />

20. 毫米波雷达的测量更新过程

    <img src="/home/saic/xsj/note2/Note/UD/毫米波雷达的测量更新１.png" alt="毫米波雷达的测量更新１" style="zoom:25%;" />

    <img src="/home/saic/xsj/note2/Note/UD/毫米波雷达的测量更新２.png" alt="毫米波雷达的测量更新２" style="zoom:33%;" />

21. 状态更新方程

    <img src="/home/saic/xsj/note2/Note/UD/状态更新方程.png" alt="状态更新方程" style="zoom:25%;" />

22. 如何选择噪声参数以及如何评估选择

    <img src="/home/saic/xsj/note2/Note/UD/噪声参数选择１.png" alt="噪声参数选择１" style="zoom: 50%;" />

23. 测量噪声：查看传感器手册，测量标准差的平方（方差）

24. 过程噪声: 

    > 1. 假设过程噪声是白噪声（服从高斯分布）
    >
    > 2. 估算你在环境中可能碰到的最大加速度，比如环境中的汽车，最大加速度不会超过６m/s2，选择预期最大加速度的一半作为过程噪声的标准差（如果要求系统的反应速度更快，那应该选择更高点的
    >
    >    过程噪声，如果需要平滑估算，那么选择更低点的过程噪声）
    >
    > 3. <img src="/home/saic/xsj/note2/Note/UD/过程噪声的选择.png" alt="过程噪声的选择" style="zoom:33%;" />

25. 评价参数是否设置的正确：滤波器一致性检查(Normalized innovation squared: NIS)

26. 如果每次测量都落在预测的外面：低估了系统的不确定性，估算的精度比想的低；（滤波器不一致）

    <img src="/home/saic/xsj/note2/Note/UD/评价参数１.png" alt="评价参数１" style="zoom:25%;" />

27. 如果测量每次都落在预测的里面: 高估了系统的不确定性，估算精度比想的要高;（滤波器不一致）

    <img src="/home/saic/xsj/note2/Note/UD/高估了系统的不确定性.png" alt="高估了系统的不确定性" style="zoom:25%;" />

28. 滤波器一致性：能够提供真实的估算不确定性;

29. 在设计滤波器时始终进行一致性检查:NIS

    <img src="/home/saic/xsj/note2/Note/UD/nis计算公式.png" alt="nis计算公式" style="zoom:33%;" />

30. 归一化检查：ｎｉｓ的值服从卡方分布

    <img src="/home/saic/xsj/note2/Note/UD/卡方分布理解１.png" alt="卡方分布理解１" style="zoom:33%;" />

31. 正常的nis和不正常的nis

    <img src="/home/saic/xsj/note2/Note/UD/正常ｎｉｓ.png" alt="正常ｎｉｓ" style="zoom:50%;" />

    <img src="/home/saic/xsj/note2/Note/UD/低估的nis.png" alt="低估的nis" style="zoom:50%;" />

    <img src="/home/saic/xsj/note2/Note/UD/高估的nis.png" alt="高估的nis" style="zoom:33%;" />

32. 评价参数

    > ### Process Noise and the UKF Project
    >
    > For the CTRV model, two parameters define the process noise:
    >
    > - \large \sigma^2_a*σ**a*2 representing longitudinal acceleration noise (you might see this referred to as linear acceleration)
    > - \large \sigma^2_{\ddot\psi}*σ**ψ*¨2 representing yaw acceleration noise (this is also called angular acceleration)
    >
    > In the project, both of these values will need to be tuned. You will have to test different values in order to get a working solution. In the video, Dominik mentions using \large \sigma^2_a = 9 \frac{m^2}{s^4}*σ**a*2=9*s*4*m*2 as a starting point when tracking a vehicle. In the UKF project, you will be tracking a bicycle rather than a vehicle. So 9 might not be an appropriate acceleration noise parameter. Tuning will involve:
    >
    > - guessing appropriate parameter values
    > - running the UKF filter
    > - deciding if the results are good enough
    > - tweaking the parameters and repeating the process
    >
    > 
    >
    > ### Linear Acceleration Noise Parameter Intuition
    >
    > Let's get some intuition for these noise parameters. The units for the acceleration noise parameter \large \sigma^2_a*σ**a*2are \Large\frac{m^2}{s^4}*s*4*m*2. Taking the square root, we get \large \sigma_a*σ**a* with units \large \frac{m}{s^2}*s*2*m*. So the square root of the acceleration noise parameter has the same units as acceleration: \large \frac{m}{s^2}*s*2*m*
    >
    > The parameter \large \sigma_a*σ**a* is the standard deviation of linear acceleration! Remember from the "CTRV Process Noise Vector" lecture that the linear acceleration is being modeled as a Gaussian distribution with mean zero and standard deviation \large \sigma_a*σ**a*. In a Gaussian distribution, about 95% of your values are within 2\large \sigma_a*σ**a*.
    >
    > So if you choose \large \sigma^2_a = 9 \frac{m^2}{s^4}*σ**a*2=9*s*4*m*2, then you expect the acceleration to be between \large -6 \frac{m}{s^2}−6*s*2*m* and \large +6 \frac{m}{s^2}+6*s*2*m* about 95% of the time.
    >
    > Tuning parameters involves some trial and error. Using your intuition can help you find reasonable initial values.
    >
    > 
    >
    > ### Yaw Acceleration Noise Parameter Intuition
    >
    > If yaw acceleration has units of radians/s^2*r**a**d**i**a**n**s*/*s*2, what are the units of the yaw acceleration noise parameter \large \sigma^2_{\ddot\psi}*σ**ψ*¨2?
    >
    > (A) rad/s^2*r**a**d*/*s*2
    >
    > (B) rad^2/s^2*r**a**d*2/*s*2
    >
    > (C) rad/s^4*r**a**d*/*s*4
    >
    > (D) rad^2/s^4*r**a**d*2/*s*4
    >
    > 
    >
    > ### 练习题
    >
    > What are the units for the yaw acceleration noise parameter?
    >
    > - A
    > - B
    > - C
    > - D
    >
    > 提交
    >
    > 
    >
    > Let's think about what values might be reasonable for the yaw acceleration noise parameter.
    >
    > Imagine the bicycle is traveling in a circle with a constant yaw rate (angular velocity) of \Large \frac{\pi}{8} \frac{rad}{s}8*π**s**r**a**d*. That means the bicycle would complete a full circle in 16 seconds: \Large\frac{\pi}{8} \frac{rad}{s} \cdot8*π**s**r**a**d*⋅ 16 s = 2\pi16*s*=2*π*.
    >
    > That seems reasonable for an average bike rider traveling in a circle with a radius of maybe 16 meters.
    >
    > The bike rider would have also have a tangential velocity of 6.28 meters per second because \Large\frac{\pi}{8} \frac{rad}{s} \cdot8*π**s**r**a**d*⋅ 16 \text{ meters}16 meters = 6.28 \text{ meters per second}=6.28 meters per second.
    >
    > What if the angular acceleration were now \large -2\pi \frac{rad}{s^2}−2*π**s*2*r**a**d* instead of zero? In just one second, the angular velocity would go from \Large \frac{\pi}{8} \frac{rad}{s}8*π**s**r**a**d* to \Large -\frac{15\pi}{8} \frac{rad}{s}−815*π**s**r**a**d*. This comes from \Large \frac{\pi}{8} \frac{rad}{s}8*π**s**r**a**d*- 2\pi−2*π* \Large \frac{rad}{s^2} \cdot*s*2*r**a**d*⋅1 s = -1*s*=−\Large \frac{15\pi}{8} \frac{rad}{s}815*π**s**r**a**d*.
    >
    > The bicycle has been completing a complete circle in 16 seconds. But with such a high angular acceleration, then all of a sudden the bicycle is going around the circle in the opposite direction and only takes about 1.1 second to complete the circle.
    >
    > From a bicycle, a setting in the range of \large \sigma_{\ddot\psi} = 2\pi \frac{rad}{s^2}*σ**ψ*¨=2*π**s*2*r**a**d* seems too high. In the project, you'll have to experiment with different values to see what works well.
    >
    > 
    >
    > ### Measurement Noise Parameters
    >
    > Measurement noise parameters represent uncertainty in sensor measurements. In general, the manufacturer will provide these values in the sensor manual. In the UKF project, you will not need to tune these parameters.

    ![评价参数２](/home/saic/xsj/note2/Note/UD/评价参数２.png)

    ![评价参数３](/home/saic/xsj/note2/Note/UD/评价参数３.png)

33. Note that although radar does include velocity information, the radar velocity and the CTRV velocity are not the same. Radar velocity is measured from the autonomous vehicle's perspective. If you drew a straight line from the vehicle to the bicycle, radar measures the velocity along that line.

34. 初值设定

    <img src="/home/saic/xsj/note2/Note/UD/初值设定.png" alt="初值设定" style="zoom:33%;" />

35. ## Optional Resources

    To complete the project, you only need the files in the github repo; however, we are also providing some extra resources that you can use to develop your solution:

    - A [Sensor Fusion utilities repo](https://github.com/udacity/CarND-Mercedes-SF-Utilities) containing Matlab scripts that will generate more sample data (generating your own sample data is completely optional)
    - A visualization package that you can also find within [the Sensor Fusion utilities repo](https://github.com/udacity/CarND-Mercedes-SF-Utilities)
    - a 2-dimensional Unity simulator for visualizing your results. See the "2-D Unity Simulator" lecture slides for more information.

36. 

    

    













