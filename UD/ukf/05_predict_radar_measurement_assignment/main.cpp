#include <iostream>
#include "Dense"
#include <vector>
#include "ukf.h"

using namespace std;
using Eigen::MatrixXd;
using Eigen::VectorXd;
using std::vector;

int main() {

	//Create a UKF instance
	UKF ukf;

/*******************************************************************************
* Programming assignment calls
*******************************************************************************/
    
    VectorXd z_out = VectorXd(3);
    MatrixXd S_out = MatrixXd(3, 3);
    ukf.PredictRadarMeasurement(&z_out, &S_out);

	return 0;
}



/*
expected result z_out:
z_pred =

6.12155

0.245993

2.10313

expected result s_out:
S =

0.0946171 -0.000139448 0.00407016

-0.000139448 0.000617548 -0.000770652

0.00407016 -0.000770652 0.0180917


*/