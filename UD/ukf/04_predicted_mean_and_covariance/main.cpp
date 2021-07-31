/*
Solution
The first part of my solution is setting a vector of weights as we discussed in the last video.
The predicted state mean is just the implementation of the corresponding equation.
When calculating the predicted state covariance matrix,
I did something you might not have done.
In the equation we always need the difference between the mean predicted state and a sigma points. 
The problem here is that the state contains an angle. 
As you have learned before,
  subtracting angles is a problem for Kalman filters,
  because the result might be 2\pi2π plus a small angle, 
  instead of just a small angle. That’s why I normalize the angle here.
Make sure you always normalize when you calculate the difference between angles.


*/
#include "Dense"
#include "ukf.h"

using Eigen::MatrixXd;
using Eigen::VectorXd;

int main() {

  // Create a UKF instance
  UKF ukf;

  /**
   * Programming assignment calls
   */
  VectorXd x_pred = VectorXd(5);
  MatrixXd P_pred = MatrixXd(5, 5);
  ukf.PredictMeanAndCovariance(&x_pred, &P_pred);

  return 0;
}


/*
expected result x:
x =
5.93637
1.49035
2.20528
0.536853
0.353577

expected result p:
P =

0.00543425 -0.0024053 0.00341576 -0.00348196 -0.00299378

-0.0024053 0.010845 0.0014923 0.00980182 0.00791091

0.00341576 0.0014923 0.00580129 0.000778632 0.000792973

-0.00348196 0.00980182 0.000778632 0.0119238 0.0112491

-0.00299378 0.00791091 0.000792973 0.0112491 0.0126972

*/
