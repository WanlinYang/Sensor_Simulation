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
        ^ y
        |     obj
        |
 ---------------> x
        |
        |
maintain states: speed, direction, position
use garbage-in garbage-out strategy to keep positon in range
*/

class DistanceEmulator {
public:
  DistanceEmulator();

  // build thread and update position
  void run_simulation();

  // join thread
  void stop_simulation();

  // output of this class
  std::atomic<double> distance;

private:
  void run();

  // move the object and eagerly compute distance
  void update_position();

  // implement garbage in, garbage out
  double adjust_coordinate(double value, double min_limit, double max_limit);

  std::unique_ptr<std::thread> my_thread_;

  bool active_;
  double frequency_;                    // Hz

  std::pair<double, double> position_;  // (x, y)
  double velocity_;                     // 0 - velocity_limit_
  double direction_;                    // 0 - 2*pi

  std::pair<double, double> position_limit_;
  double velocity_limit_;               // m/s
};