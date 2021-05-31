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

  std::vector<SensorType> available_sensors;
  std::vector<SensorType> init_sensors;

  double laser_weight;
  double sonar_weight;
  double infrared_weight;

  double laser_frequency;
  double sonar_frequency;
  double infrared_frequency;

  double laser_variance;
  double sonar_variance;
  double infrared_variance;
};

class SensorFrontend {
public:
  typedef FrontendConfig::SensorType SensorType;

  SensorFrontend(FrontendConfig& config);

  void run();

private:
  std::shared_ptr<DistanceEmulator> dist_emu_;

  std::vector<std::unique_ptr<SensorEmulator>> sensors_;              // array of available sensors
  std::unordered_map<SensorType, int, std::hash<int>> index_map_;     // sensortype : index of sensors_ array
  std::unordered_map<SensorType, double, std::hash<int>> weight_map_; // sensortype : weight

  FrontendConfig config_;
};