#pragma once

#include "global_utils.h"
#include "candle.hpp"
#include "MD.hpp"
#include <unistd.h>
// #include <vn/sensors.h>

// using namespace vn::sensors;
using namespace mab;

/// @brief Function to safely construct candle instance.
/// @param candle Pointer to candle object (can be nullptr)
Candle *initializeCandle();

/// @brief Initializes motors of the hopper robot. Pings CAN bus and listens for
/// responses. Ensures exactly two motors respond, adds them to CANdle instance,
/// zeros their encodes, sets them into impedance control mode, and enables them
/// @param candle CANdle instance to add the motors too
std::vector<mab::canId_t> initializeMotors(Candle *candle);

// /// @brief Ensures there are two motors connected to the candle
// /// @param candle Candle object
// void checkMotorConnections(Candle *candle);