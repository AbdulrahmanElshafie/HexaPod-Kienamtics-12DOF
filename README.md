# HexaPod Kinematics, Localization, and Remote Control

This code repository encompasses a system designed for controlling a HexaPod robot. The HexaPod, equipped with six legs, each with two joints, is programmed to execute complex movements, localize itself within a room using machine learning (ML) models, WiFi signal strength, move to specified targets, and respond to commands via a web-based remote control interface.

## Overview

The codebase is structured to offer multifaceted functionalities:

### HexaPod Kinematics and Movement

The core of the code is dedicated to defining and controlling the movements of the HexaPod. The kinematics and behaviors are programmed in a modular and structured manner, enabling the HexaPod to perform the following actions:
- **Forward, Backward Movements**: The robot can navigate in these directions using its multi-leg configuration.
- **Spinning Left, Right Movements**: The robot can spin in these directions around itself so you can move to its left and right.
- **Standing Up and Resetting Angles**: The HexaPod can return to its initial standing position and reset joint angles for subsequent movements.
- **Complex Movements like Dancing**: The code includes predefined sequences for intricate movements, such as dancing routines.

### Web-Based Remote Control Interface

The system incorporates a web server enabling remote control of the HexaPod via a user-friendly web interface. Users can interact with the HexaPod by accessing a webpage that provides intuitive buttons for:
- Initiating specific movements (Forward, Backward, Left, Right, Stand up).
- Read surrounding RSSI Signals and locate itself.
- Accessing autonomous movement to predefined targets within the environment.

### Room Localization using Machine Learning and WiFi Signal Strength

The HexaPod is equipped with the capability to ascertain its location within a room using WiFi signal strength and an embedded ML model. This localization process involves:
- Scanning available WiFi networks and analyzing signal strengths.
- Utilizing a trained ML model to infer the room or area the HexaPod is situated in based on WiFi signal characteristics.
- Deriving coordinates or distance estimations to known WiFi access points to determine relative position within the room.

## Code Structure

### Libraries and Dependencies

The code relies on various libraries and dependencies, including:
- **Wire** and **Adafruit_PWMServoDriver**: For PWM servo control and communication.
- **WiFi** and **WebServer**: For WiFi connectivity and handling web server operations.
- **Localization_inferencing**: An ML inference module for room localization.

### Modular Classes and Functions

The code is organized into distinct classes and functions:
- **Leg Class**: Defines individual legs of the HexaPod, allowing easy configuration of joint pins.
- **HexaPod Class**: Manages all six legs and their movements, providing methods for precise control and coordination.

### Functions
#### calc_distance(int rssi)
This function calculates the estimated distance from the WiFi access point based on the received signal strength indicator (RSSI) value. It uses an environmental factor and a reference RSSI value at 1 meter (rssi_1m_distance) to estimate the distance in meters. The environment factor is used to tell the estimated environmental effect in the received signal strength. 

#### find_target(String target)
This function scans the available WiFi networks and searches for a specific target network name. It returns the index of the found target network in the scan results.

#### locate_target(String target)
Utilizing the find_target() function, this function determines the distance to the specified target WiFi network using its RSSI value. It combines the RSSI value retrieval and distance calculation to the target network.

#### go_to_target(String target)
This function initiates movement towards the specified target WiFi network. It continually checks the distance to the target and adjusts the movement direction (forward, backward, or spin) based on the signal strength variations. Initially, the robot tries to find which direction will lead to the target by trying the 4 available directions. When he finds the direction that leads to his target to starts moving towards it until one happens, either reaching the target or finding that the distance doesn't change so he turns right and checks the 2 directions to find which leads to the target. Or he moves away from it so then he tries to re-locate itself and search for the direction again. 

#### my_location(int rssi1, int rssi2, int rssi3)
This function takes three RSSI values from different WiFi access points as inputs. It constructs a signal with these RSSI values and performs inference using a pre-trained machine-learning model. The inference provides an estimation of the device's location based on the provided RSSI values.

#### my_coordinates(int d1, int d2, int d3, int x1, int x2, int x3, int y1, int y2, int y3)
This function computes the coordinates of the device based on the received distances (d1, d2, d3) and reference coordinates (x1, x2, x3, y1, y2, y3). It uses a mathematical formula to estimate the device's x and y coordinates in the specified environment.

### Usage Instructions

1. Upload the code to the HexaPod's control unit.
2. Connect to the HexaPod's WiFi network.
3. Access the control interface via a web browser.
4. Use the provided buttons to control the HexaPod's movement.
