#pragma once

#include <iostream>
#include <utility>
#include <time.h>
#include <math.h>
#include <stdlib.h>

#include <thread>
#include <atomic>

#include "color.h"

/*
This class simulates a moving object around origin point in 2D space
        ^ y
        |     obj
        |
 ---------------> x
        |
        |
velocity(t) = velocity(t-1) + delta_velocity
direction(t) = direction(t-1) + delta_angle
distance = delta_t * velocity
position_x(t) = position_x(t-1) + distance * cos(direction(t))
position_y(t) = position_y(t-1) + distance * sin(direction(t))

maintain states: speed, direction, position
use garbage-in garbage-out strategy to keep positon in range [-x_lim, x_lim], [-y_lim, y_lim]
*/

class DistanceEmulator {
public:
  // config of the emulator will be hard-coded
  DistanceEmulator();

  // build thread and update position
  void run_simulation();

  // deactivate and join thread
  void stop_simulation();

  // output of this class
  std::atomic<double> distance;

private:
  // the function passed to std::thread
  void run();

  // move the object and eagerly compute distance
  void update_position();

  // implement garbage in, garbage out transition
  double adjust_coordinate(double value, double min_limit, double max_limit);

  std::unique_ptr<std::thread> my_thread_;

  bool active_;
  double frequency_;                            // Hz

  std::pair<double, double> position_;          // (x, y)
  double velocity_;                             // 0 to velocity_limit_
  double direction_;                            // 0 to 2*pi

  std::pair<double, double> position_limit_;    // (x_lim, y_lim)
  double velocity_limit_;                       // m/s
};