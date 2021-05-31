#pragma once

#include <iostream>
#include <iomanip>
#include <vector>
#include <memory>
#include <unordered_map>

#include "DistanceEmulator.h"
#include "SensorEmulator.h"
#include "LaserEmulator.h"
#include "SonarEmulator.h"
#include "InfraredEmulator.h"

struct FrontendConfig {
  enum SensorType {laser, sonar, infrared};

  // sensors that are installed on the robot
  std::vector<SensorType> available_sensors;
  // active sensors at the beginning
  std::vector<SensorType> init_sensors;

  // weights to compute fused distance
  double laser_weight;
  double sonar_weight;
  double infrared_weight;

  // sampling frequency
  double laser_frequency;
  double sonar_frequency;
  double infrared_frequency;

  // add gaussian noise generate simulated measurement
  double laser_variance;
  double sonar_variance;
  double infrared_variance;
};

class SensorFrontend {
public:
  typedef FrontendConfig::SensorType SensorType;

  // constructor
  SensorFrontend(FrontendConfig& config);

  // the main pipeline
  void run();

private:
  std::shared_ptr<DistanceEmulator> dist_emu_;                        // pointer to DistanceEmulator

  std::vector<std::unique_ptr<SensorEmulator>> sensors_;              // array of available sensors
  std::unordered_map<SensorType, int, std::hash<int>> index_map_;     // sensortype : index of sensors_ array
  std::unordered_map<SensorType, double, std::hash<int>> weight_map_; // sensortype : weight

  FrontendConfig config_;
};