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

  // @freq: frequency (Hz) of sampling rate
  // @name: name of sensor
  // @dist_emu: pointer of DistanceEmulator
  SensorEmulator(double freq, std::string name, std::shared_ptr<DistanceEmulator> dist_emu);
  virtual ~SensorEmulator();

  // construct thread and start loop
  virtual bool init();

  // return measurement value if sensor is active, return -1 otherwise
  virtual double read();

  // stop updating measurement and join the thread
  virtual bool deinit();

  // getters
  virtual bool isInitialized();
  virtual std::string getSensorName();
  virtual double getFrequency();

protected:

  // add gaussian noise to groud truth
  virtual double addNoise(double value) = 0;

  // the function passed to std::thread
  virtual void run();

  std::string sensor_name_;
  std::atomic<bool> initialized_;               // default: false
  std::atomic<double> frequency_;               // Hz

  std::atomic<double> measurement_;             // measurement of distance, meters

  std::shared_ptr<DistanceEmulator> dist_emu_;
  std::unique_ptr<std::thread> my_thread_;

};