#pragma once

#include <random>
#include "SensorEmulator.h"

class InfraredEmulator: public SensorEmulator {
public:

  InfraredEmulator(double freq, std::shared_ptr<DistanceEmulator> dist_emu, double var);
  ~InfraredEmulator();

private:
  double variance_;

  std::default_random_engine generator_;
  std::normal_distribution<double> distribution_;
  double addNoise(double value) override;
};