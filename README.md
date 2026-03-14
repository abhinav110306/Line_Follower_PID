# PID Controlled Line Follower Robot

This project implements an autonomous line-following robot using an analog IR sensor array and a PID control algorithm to maintain stable tracking of the line.

## Features
- PID based steering correction
- Analog IR sensor array for line detection
- Differential drive robot platform
- Motor control using TB6612FNG driver
- Designed custom chassis for stability and component layout

## Hardware
- Microcontroller (ESP32 / Arduino)
- Analog IR sensor array
- TB6612FNG motor driver
- DC motors with differential drive
- Custom 3D printed chassis

## Control Algorithm
The robot calculates the position of the line using the IR sensor array and computes an error relative to the center position.  
A PID controller adjusts the speed difference between the two motors to minimize this error.

## Status
Project currently under development. PID tuning and power distribution debugging are ongoing.

## Author
Developed as part of personal robotics projects.
