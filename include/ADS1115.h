#pragma once
#include <wiringPiI2C.h>
#include <iostream>
#include <cstdint>

// I2C ADDRESS
#define ADS1115_I2C_ADDRESS         0x48

// Registers
#define ADS1115_CONVERSION_REG      0b00000000
#define ADS1115_CONFIG_REG          0b00000001
#define ADS1115_LOW_THRES_REG       0b00000010
#define ADS1115_HIGH_THRES_REG      0b00000011

// Configuration bits (did not config here, config in the python)
// #define ADS1115_CONFIG_OS           0b1
// #define ADS1115_CONFIG_MUX          0b000
// #define ADS1115_CONFIG_PGA          0b011           // change ADS1115_FSR after changing this value
// #define ADS1115_CONFIG_MODE         0b1
// #define ADS1115_CONFIG_DR           0b110
// #define ADS1115_CONFIG_COMP_MODE    0b0
// #define ADS1115_CONFIG_COMP_POL     0b0
// #define ADS1115_CONFIG_COMP_LAT     0b1
// #define ADS1115_CONFIG_COMP_QUE     0b00

// Configuration dependent
#define ADS1115_FSR                 1.024 // V
const int excitation_voltage = 10;                  // Load cell power supply 0V to 10V, so +/- 5V
const double torque_constant_futek = 22.60/0.4509; // Nm / (mV/V) [torque ratio to normalized voltage]

class ADS1115{
    public:
        /// @brief Constructor for the ads1115 class, i2c address unspecified
        ADS1115();
        /// @brief Constructor for the ads1115 class, i2c class specified
        /// @param i2c_address Address of the ads1115
        ADS1115(const uint16_t i2c_address);
        /// @brief Reads the channel 1 voltage to the ADC
        double readVoltage();
        /// @brief Converts to voltage to the torque value
        double readTorque();
    private:
        /// @brief File descriptor for the i2c device
        int fd;
        /// @brief Configuration written to the ads1115
        uint16_t config;
        /// @brief Function to set up ADS1115
        void setup(uint16_t i2c_address);
        /// @brief Function to set the config register
        void setConfigReg();
};