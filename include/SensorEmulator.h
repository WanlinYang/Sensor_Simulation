#pragma once
#include <iostream>
#include <string>

#include <thread>
#include <chrono>
#include <atomic>

#include "DistanceEmulator.h"
#include "color.h"


class SensorEmulator {
public:

  SensorEmulator(double freq, std::string name, std::shared_ptr<DistanceEmulator> dist_emu);
  virtual ~SensorEmulator();

  // construct thread and start loop
  virtual bool init();

  virtual double read();

  virtual bool deinit();

  virtual bool isInitialized();

  virtual std::string getSensorName();

  virtual double getFrequency();

protected:

  virtual double addNoise(double value) = 0;

  virtual void run();

  std::string sensor_name_;

  std::atomic<bool> initialized_;
  std::atomic<double> frequency_;
  std::atomic<double> measurement_;

  std::shared_ptr<DistanceEmulator> dist_emu_;
  std::unique_ptr<std::thread> my_thread_;

};