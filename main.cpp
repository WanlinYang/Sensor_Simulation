#include <iostream>
#include <string>

#include "LaserEmulator.h"
#include "DistanceEmulator.h"
#include "SensorFrontend.h"

FrontendConfig buildConfig() {
  typedef FrontendConfig::SensorType SensorType;
  FrontendConfig config;

  config.available_sensors = {SensorType::laser, SensorType::sonar, SensorType::infrared};
  config.init_sensors = {SensorType::laser};

  config.laser_weight = 0.9;
  config.sonar_weight = 0.75;
  config.infrared_weight = 0.66;

  config.laser_frequency = 1.0;
  config.sonar_frequency = 1.0;
  config.infrared_frequency = 1.0;

  config.laser_variance = 0.05;
  config.sonar_variance = 0.1;
  config.infrared_variance = 0.2;

  return config;
}

int main(int, char**) {
  FrontendConfig config = buildConfig();
  SensorFrontend front(config);
  front.run();
  return 0;
}
