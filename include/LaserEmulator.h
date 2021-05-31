#pragma once

#include <random>
#include "SensorEmulator.h"

class LaserEmulator: public SensorEmulator {
public:

  LaserEmulator(double freq, std::shared_ptr<DistanceEmulator> dist_emu, double var);
  ~LaserEmulator();

private:
  double variance_;

  std::default_random_engine generator_;
  std::normal_distribution<double> distribution_;
  double addNoise(double value) override;
};