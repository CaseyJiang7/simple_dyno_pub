#pragma once

#include "candle.hpp"
#include "loop_timer.h"
#include "motor_utils.h"
#include "ADS1115.h"
#include <fstream>
#include <iostream>
#include <thread>
#include <wiringPi.h>
#include <unistd.h>
#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <tuple>
#include <filesystem>

void createSigIntHandler();

/// @brief Catches Ctrl+C while middleware is running
/// @param s SIGINT
void handleKill(int s);