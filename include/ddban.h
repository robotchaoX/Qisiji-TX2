//
// Created by doudouban on 18-10-4.
//

#ifndef BOX_RUNTIME_VERTION_DDBAN_H
#define BOX_RUNTIME_VERTION_DDBAN_H

#include "can_jetson.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <mainwindow.h>
#include <math.h>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

class control {
public:
  double car_up;
  double angle_1;
  void vertical_control(vector<Point2f> &fallPoints_2D);
  void traversal_control(vector<Point3f> &fallpoint_world,
                         const int height_of_basket, const int symbol_step,
                         MainWindow &w);
  void receive_control();
  void can_state();

protected:
  const float g1 = 9.8;
  const float Pi = 3.1415926;
  const int arm_r = 1;
  const int v = 1;
  const double angle_a = 30; ///
  const double angle_b = 60;
  long long CAN_id;
  const int CAN_dlc = 8;
  //  array<int, 8> CAN_data = {0, 0, 0, 0, 0, 0, 0, 0};  //  error: array must
  //  be initialized with a brace-enclosed initializer array<int, 8>
  //  receive_data = {0, 0, 0, 0, 0, 0, 0, 0};  // error: too many initializers
  //  for ‘std::array<int, 8ul>’
  array<int, 8> CAN_data = {{0, 0, 0, 0, 0, 0, 0, 0}};
  array<int, 8> receive_data = {{0, 0, 0, 0, 0, 0, 0, 0}};
  //  array<int, 8> CAN_data = {};
  //  array<int, 8> receive_data ={};
  int receive_id;
};

#endif // BOX_RUNTIME_VERTION_DDBAN_H
