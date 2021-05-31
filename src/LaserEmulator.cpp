#include "LaserEmulator.h"

LaserEmulator::LaserEmulator(double freq,
    std::shared_ptr<DistanceEmulator> dist_emu, double var) :
  SensorEmulator(freq, "Laser", dist_emu),
  variance_(var)
{
  std::cout << "Construct " << sensor_name_ << std::endl;
  distribution_ = std::normal_distribution<double>(0, variance_);
}

LaserEmulator::~LaserEmulator() {}

double LaserEmulator::addNoise(double value) {
  double diff = distribution_(generator_);
  return value + diff;
}
