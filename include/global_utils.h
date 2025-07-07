#pragma once

// #include "hopper_lcm_msgs/hopper_control_t.hpp"
// #include "hopper_lcm_msgs/hopper_state_t.hpp"
// #include <Eigen>
#include <iostream>
#include <mutex>
#include <string>

/// @brief Macro escape sequences for bold colorful text
const std::string BOLDRED = "\033[1;31m";
const std::string BOLDBLUE = "\033[1;34m";
const std::string BOLDGREEN = "\033[1;32m";
const std::string STDTEXT = "\033[1;0m";

/// @brief Multicast address for LCM traffic to be routed through
const std::string MULTICAST_ADDRESS = "udpm://239.255.76.67:7667?ttl=1";

/// @brief LCM channel for the hopper to publish its state to
const std::string HOPPER_STATE_CHANNEL = "HOPPER_STATE";

/// @brief LCM channel for the upper computer to publish motor commands to
const std::string MOTOR_COMMAND_CHANNEL = "MOTOR_COMMANDS";

/// @brief Index of the hip motor (assuming its ID is 185)
const int HIP_MOTOR_INDEX = 0;

/// @brief Index of the knee motor (assuming its ID is 190)
const int KNEE_MOTOR_INDEX = 1;

/// @brief Queries the Raspberry Pi's Real Time Clock for the time since
/// Epoch (1970-01-01 00:00:00 UTC)
/// @return Time since Epoch in ns
uint64_t rtcNsSinceEpoch();

// /// @brief Output operator for hopper_control_t messages
// /// @param os output stream
// /// @param command message to print
// /// @return output stream
// std::ostream &operator<<(std::ostream &os,
//                          const hopper_lcm_msgs::hopper_control_t &command);

// /// @brief Output operator for hopper_state_t messages
// /// @param os output stream
// /// @param state message to print
// /// @return output stream
// std::ostream &operator<<(std::ostream &os,
//                          const hopper_lcm_msgs::hopper_state_t &state);