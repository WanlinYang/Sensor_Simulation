#include "SensorEmulator.h"

SensorEmulator::SensorEmulator(double freq, 
  std::string name, std::shared_ptr<DistanceEmulator> dist_emu) :
    frequency_(freq),
    sensor_name_(name),
    initialized_(false)
{
  dist_emu_ = dist_emu;
}

SensorEmulator::~SensorEmulator() {}

bool SensorEmulator::init() {
  if (initialized_) {
    std::cout << CYAN << sensor_name_ << " already initialized" << RESET << std::endl;
    return false;
  }
  // start a thread for this sensor
  initialized_ = true;
  my_thread_ = std::make_unique<std::thread>(&SensorEmulator::run, this);
  std::cout << CYAN << sensor_name_ << " initialized" << RESET << std::endl;
  return true;
}

double SensorEmulator::read() {
  // output error message rather than throw an exception
  if (!initialized_) {
    std::cout << RED << "Error: " << sensor_name_ << " not initialized" << RESET << std::endl;
    return -1;
  }
  return measurement_;
}

bool SensorEmulator::deinit() {
  if (!initialized_ || !my_thread_->joinable()) {
    initialized_ = false;
    std::cout << CYAN << sensor_name_ << " already deinitialized" << RESET << std::endl;
    return false;
  }
  initialized_ = false;
  if (my_thread_->joinable())
    my_thread_->join();
  std::cout << CYAN << sensor_name_ << " deinitialized" << RESET << std::endl;
  return true;
}

bool SensorEmulator::isInitialized() {
  return initialized_;
}

std::string SensorEmulator::getSensorName() {
  return sensor_name_;
}

double SensorEmulator::getFrequency() {
  return frequency_;
}

void SensorEmulator::run() {
  while (initialized_) {
    std::this_thread::sleep_for(std::chrono::milliseconds((int) (1000.0/frequency_)));
    measurement_ = addNoise(dist_emu_->distance);
  }
}