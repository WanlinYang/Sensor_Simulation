# Simulation of Sensor Manage System

## Motivation

This repository simulates a sensor manage system of a robot. The robot has three sensors to measure distance: Laser, Sonar, and Infrared. Each sensor works independently. We simulate a moving object in 2D space and three sensors, and use our sensor manage system to query the measurement of each sensor.

## Implementation

The software is written in C++. There are several classes implemented in this repo.

#### DistanceEmulator
The emulator to simulate a moving object in 2D space. This class outputs a ground-truth distance between the object and robot. The object moves randomly within range of [-10, 10] meters in both x and y coordinates. The position of the moving object is updated at 10Hz.

#### SensorEmulator
The base class of `LaserEmulator`, `SonarEmulator`, `InfraredEmulator`. The emulator is able to receive simulated distance from `DistanceEmulator`, and adds gaussian noise to ground-truth distance to simulate measurement of each sensor. There are several APIs in the emulator for users to access:

* `init()`: Turn on the sensor if the sensor is available
* `deinit()`: Turn off the sensor 
* `read()`: Read measurement of sensor if the sensor is turned on

#### SensorFrontend
The main class of the sensor manage system. This class runs on the main thread and receives user input and prints output on terminal.

The above classes run on different threads. `SensorFrontend` runs on the main thread, `DistanceEmulator`, `LaserEmulator`, `SonarEmulator`, `InfraredEmulator` run on their separate thread.

## Compile
The software is developed on Ubuntu system, and relies on standard C++ libraries and cmake. Firstly clone the repository.

```
git clone https://github.com/WanlinYang/Sensor_Simulation.git
cd Sensor_Simulation
```

Then compile the software
```
mkdir build
cmake ..
make
```

## Usage
Before running the system, several configurations should be defined. Users can change the code in `main.cpp` to set the configurations of the sensor system. Here are the configs that can be set by user
  * `available_sensors`: sensors that are installed on the robot
  * `init_sensors`: sensors that are active at the beginning
  * `laser_weight`, `sonar_weight`, `infrared_weight`: weights to compute fused measurement
  * `laser_frequency`, `sonar_frequency`, `infrared_frequency`: sampling frequency of each sensor
  * `laser_variance`, `sonar_variance`, `infrared_variance`: parameter of gaussian noise of each sensor

Run the executable file to start the system
```
./SensorSys
``` 

Following are commands of the software. Commands in parentheses are abbreviated commands.
```
  init laser    (il) : Initialize laser sensor
  stop laser    (sl) : Stop laser sensor
  read laser    (rl) : Read laser sensor
  init sonar    (is) : Initialize sonar sensor
  stop sonar    (ss) : Stop sonar sensor
  read sonar    (rs) : Read sonar sensor
  init infrared (ii) : Initialize IR sensor
  stop infrared (si) : Stop IR sensor
  read infrared (ri) : Read IR sensor
  status (list) (ls) : Check status of sensors
  quit           (q) : Quit the system
  dist           (d) : Output measurements of all sensors and fused measurement
  help           (h) : Show available commands
```

## Discussion