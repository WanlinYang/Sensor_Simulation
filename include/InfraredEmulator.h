#pragma once

#include <random>
#include "SensorEmulator.h"

class InfraredEmulator: public SensorEmulator {
public:

  InfraredEmulator(double freq, std::shared_ptr<DistanceEmulator> dist_emu, double var);
  ~InfraredEmulator();

private:

  // members to add gaussian noise to groud truth distance, to simulate measurement
  double variance_;
  std::default_random_engine generator_;
  std::normal_distribution<double> distribution_;
  double addNoise(double value) override;
};