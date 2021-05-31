cmake_minimum_required(VERSION 3.0.2)
project(RobotDist VERSION 0.1.0)

include(CTest)
enable_testing()

include_directories(
  include
)

set(CMAKE_CXX_FLAGS -pthread)

add_executable(SensorSys
  src/DistanceEmulator.cpp
  src/SensorEmulator.cpp
  src/LaserEmulator.cpp
  src/SonarEmulator.cpp
  src/InfraredEmulator.cpp
  src/SensorFrontend.cpp
  main.cpp
 )

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
