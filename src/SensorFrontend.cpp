#include "SensorFrontend.h"

SensorFrontend::SensorFrontend(FrontendConfig& config) :
  config_(config)
{
  // distance emulator
  dist_emu_ = std::make_shared<DistanceEmulator>();
  // build sensor pointers
  for (SensorType sensor_type : config_.available_sensors)  {
    switch (sensor_type) {
    case SensorType::laser:
      sensors_.emplace_back(std::make_unique<LaserEmulator>(
        config_.laser_frequency, dist_emu_, config_.laser_variance
      ));
      index_map_[SensorType::laser] = sensors_.size() - 1;
      weight_map_[SensorType::laser] = config_.laser_weight;
      break;

    case SensorType::sonar:
      sensors_.emplace_back(std::make_unique<SonarEmulator>(
        config_.sonar_frequency, dist_emu_, config_.sonar_variance
      ));
      index_map_[SensorType::sonar] = sensors_.size() - 1;
      weight_map_[SensorType::sonar] = config_.sonar_weight;
      break;

    case SensorType::infrared:
      sensors_.emplace_back(std::make_unique<InfraredEmulator>(
        config_.infrared_frequency, dist_emu_, config_.infrared_variance
      ));
      index_map_[SensorType::infrared] = sensors_.size() - 1;
      weight_map_[SensorType::infrared] = config_.infrared_weight;
      break;

    default:
      std::cout << RED << "Unknown Sensor Type" << RESET << std::endl;
      break;
    }
  }
}

void SensorFrontend::run() {
  dist_emu_->run_simulation();
  for (SensorType sensor_type : config_.init_sensors)
    sensors_[index_map_[sensor_type]]->init();

  std::string in;
  while (true) {
    std::getline(std::cin, in);
    if (in == "quit" || in == "q") {
      break;
    } 
    /** Laser Sensor **/
    else if (in == "init laser" || in == "il" || in == "stop laser" || in == "sl" || in == "read laser" || in == "rl") {
      if (index_map_.find(SensorType::laser) == index_map_.end()) {
        std::cout << RED << "Error: Laser is not available" << RESET << std::endl;
        continue;
      }
      if (in == "init laser" || in == "il") {
        sensors_[index_map_[SensorType::laser]]->init();
      } else if (in == "stop laser" || in == "sl") {
        sensors_[index_map_[SensorType::laser]]->deinit();
      } else if (in == "read laser" || in == "rl") {
        double val = sensors_[index_map_[SensorType::laser]]->read();
        std::cout.precision(3);
        if (abs(val + 1.0) > 1e-9)
          std::cout << "get laser value: " << val << std::endl;
      } 
    }
    /** Sonar Sensor **/
    else if (in == "init sonar" || in == "is" || in == "stop sonar" || in == "ss" || in == "read sonar" || in == "rs") {
      if (index_map_.find(SensorType::sonar) == index_map_.end()) {
        std::cout << RED << "Error: Sonar is not available" << RESET << std::endl;
        continue;
      }
      if (in == "init sonar" || in == "is") {
        sensors_[index_map_[SensorType::sonar]]->init();
      } else if (in == "stop sonar" || in == "ss") {
        sensors_[index_map_[SensorType::sonar]]->deinit();
      } else if (in == "read sonar" || in == "rs") {
        double val = sensors_[index_map_[SensorType::sonar]]->read();
        std::cout.precision(3);
        if (abs(val + 1.0) > 1e-9)
          std::cout << "get sonar value: " << val << std::endl;
      }
    } 
    /** Infrared Sensor **/
    else if (in == "init infrared" || in == "ii" || in == "stop infrared" || in == "si" || in == "read infrared" || in == "ri") {
      if (index_map_.find(SensorType::infrared) == index_map_.end()) {
        std::cout << RED << "Error: Infrared is not available" << RESET << std::endl;
        continue;
      }
      if (in == "init infrared" || in == "ii") {
        sensors_[index_map_[SensorType::infrared]]->init();
      } else if (in == "stop infrared" || in == "si") {
        sensors_[index_map_[SensorType::infrared]]->deinit();
      } else if (in == "read infrared" || in == "ri") {
        double val = sensors_[index_map_[SensorType::infrared]]->read();
        std::cout.precision(3);
        if (abs(val + 1.0) > 1e-9)
          std::cout << "get infrared value: " << val << std::endl;
      }
    }
    /** Status **/
    else if (in == "status" || in == "list" || in == "ls") {
      std::cout << CYAN << "Status:" << std::endl;
      std::cout << " Available sensors:  ";
      for (int i = 0; i < sensors_.size(); i++)
        std::cout << sensors_[i]->getSensorName() << " ";
      std::cout << std::endl;

      std::cout << " Active sensors:     ";
      std::cout.precision(2); std::cout << std::fixed;
      for (int i = 0; i < sensors_.size(); i++)
        if (sensors_[i]->isInitialized())
          std::cout << sensors_[i]->getSensorName() << " (" << sensors_[i]->getFrequency() << "Hz) ";
      std::cout << RESET << std::endl;
    }
    /** Get all output and fused distance **/
    else if (in == "dist" || in == "d") {
      std::cout << CYAN << "Measurements:" << std::endl;
      std::cout.precision(3); std::cout << std::fixed;
      double dist_sum = 0.0, weight_sum = 0.0;
      for (auto it = index_map_.begin(); it != index_map_.end(); it++) {
        SensorType sensor_type = it->first;
        int idx = it->second;
        if (sensors_[idx]->isInitialized()) {
          double val = sensors_[idx]->read();
          std::cout << "  " << sensors_[idx]->getSensorName() << ": " << val << std::endl;
          weight_sum += weight_map_[sensor_type];
          dist_sum += val * weight_map_[sensor_type];
        }
      }
      if (weight_sum > 1e-3)
        std::cout << "Fused distance: " << dist_sum/weight_sum << std::endl;
      std::cout << RESET;
    /** Help **/
    } else if (in == "help" || in == "h") {
      std::cout << CYAN <<
      "Commands:\n" << 
      "  init laser    (il) : Initialize laser sensor\n" <<
      "  stop laser    (sl) : Stop laser sensor\n" <<
      "  read laser    (rl) : Read laser sensor\n" <<
      "  init sonar    (is) : Initialize sonar sensor\n" <<
      "  stop sonar    (ss) : Stop sonar sensor\n" <<
      "  read sonar    (rs) : Read sonar sensor\n" <<
      "  init infrared (ii) : Initialize IR sensor\n" <<
      "  stop infrared (si) : Stop IR sensor\n" <<
      "  read infrared (ri) : Read IR sensor\n" <<
      "  status (list) (ls) : Check status of sensors\n" <<
      "  quit           (q) : Quit the system\n" << 
      "  dist           (d) : Output measurements of all sensors and fused measurement\n" <<
      "  help           (h) : Show available commands\n";
      std::cout << RESET << std::endl;
    }
    else {
      std::cout << MAGENTA << "Unknown command" << RESET << std::endl;
    }
  }

  for (int i = 0; i < sensors_.size(); i++)
    sensors_[i]->deinit();
  dist_emu_->stop_simulation();
  std::cout << CYAN << "Finish" << RESET << std::endl;
}