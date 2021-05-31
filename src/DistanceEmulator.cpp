#include "DistanceEmulator.h"

DistanceEmulator::DistanceEmulator() :
  frequency_(10.0),
  velocity_limit_(1.0),
  distance(0.0),
  direction_(0.0),
  active_(false)
{
  // keep object in range (-10, 10) of both x and y coord
  position_limit_ = std::make_pair(10.0, 10.0);
  // initial position of object
  position_ = std::make_pair(5.0, 5.0);
}

void DistanceEmulator::run_simulation() {
  std::cout << CYAN << "DistanceEumlator : start simulation" << RESET << std::endl;
  active_ = true;
  my_thread_ = std::make_unique<std::thread>(&DistanceEmulator::run, this);
}

void DistanceEmulator::stop_simulation() {
  std::cout << CYAN << "DistanceEumlator : stop simulation" << RESET << std::endl;
  active_ = false;
  if (my_thread_->joinable())
    my_thread_->join();
}

void DistanceEmulator::run() {
  while (active_) {
    std::this_thread::sleep_for(std::chrono::milliseconds((int) (1000.0/frequency_)));
    update_position();
  }
}

void DistanceEmulator::update_position() {
  // delta_velocity in range (-0.1, 0.1)
  double delta_velocity = (((double) rand() / RAND_MAX) - 0.5) * 0.2;
  velocity_ = abs(velocity_ + delta_velocity) > velocity_limit_ ?
              velocity_ + delta_velocity : velocity_ - delta_velocity;
  // delta_angle in range (-10deg, 10deg)
  double delta_angle = (((double) rand() / RAND_MAX) - 0.5) * 20.0 / 180.0 * M_PI;
  direction_ = direction_ + delta_angle;
  if (direction_ < 0)
    direction_ += 2 * M_PI;
  if (direction_ > 2 * M_PI)
    direction_ -= 2 * M_PI;
  // update position
  double delta_x = cos(direction_) * velocity_ / frequency_;
  double delta_y = sin(direction_) * velocity_ / frequency_;
  position_.first = position_.first + delta_x;
  position_.second = position_.second + delta_y;
  // garbage in, garbage out
  position_.first = adjust_coordinate(position_.first, -position_limit_.first, position_limit_.first);
  position_.second = adjust_coordinate(position_.second, -position_limit_.second, position_limit_.second);
  // compute distance to origin
  distance = sqrt(position_.first*position_.first + position_.second*position_.second);
}

double DistanceEmulator::adjust_coordinate(double value, double min_limit, double max_limit) {
  double output = value;
  if (value < min_limit) {
    output = max_limit + value - min_limit;
  } else if (value > max_limit) {
    output = min_limit + value - max_limit;
  }
  return output;
}