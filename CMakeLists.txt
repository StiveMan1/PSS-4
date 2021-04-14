cmake_minimum_required(VERSION 3.17)
project(untitled3)

set(CMAKE_CXX_STANDARD -std=c++11 -pthread)

add_executable(untitled3 main.cpp Order.cpp Order.h Address.cpp Address.h Car.cpp Car.h Payment.cpp Payment.h Driver.cpp Driver.h Passenger.cpp Passenger.h DriverGateway.cpp DriverGateway.h PassengerGateway.cpp PassengerGateway.h)