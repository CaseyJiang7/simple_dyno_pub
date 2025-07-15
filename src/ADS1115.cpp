#include <ADS1115.h>

ADS1115::ADS1115(){
    setup(ADS1115_I2C_ADDRESS);
}

ADS1115::ADS1115(uint16_t device_id){
    setup(device_id);
}

double ADS1115::readVoltage(){
    uint16_t value = wiringPiI2CReadReg16(fd, ADS1115_CONVERSION_REG);          // read conversion reg
    uint16_t swapped = ((value << 8) & 0xFF00) | (value >> 8);

    // uint16_t config = wiringPiI2CReadReg16(fd, ADS1115_CONFIG_REG); 
    // config = ((config << 8) & 0xFF00) | (config >> 8);                          // Flip the bits on the config register
    // std::cout << "config register value:" << std::hex << config <<std::endl;

    int16_t signed_reading = static_cast<int16_t>(swapped);  // Proper 2's complement
    return (signed_reading / 32768.0) * ADS1115_FSR;                        
}

double ADS1115::readTorque(){
    double voltage = readVoltage();
    // std::cout << "voltage sensor read:" << voltage <<std::endl;
    // Account for the opamp 102 amplification （new amplifier 2025/4/21）
    voltage /= 102;
    // Account for the zero balance
    voltage +=(0.0024*excitation_voltage)/1000;
    // From the calibration data
    double normalized_vol  = voltage*1000/excitation_voltage; // unit: mV/V
    double torque = normalized_vol * torque_constant_futek; // Nm
    // std::cout << "torque sensor read:" << torque <<std::endl;
    return torque; // Nm
}

void ADS1115::setup(uint16_t i2c_address){
    fd = wiringPiI2CSetup(i2c_address);
    if (fd == -1) {
        std::cout << "Failed to init I2C communication.\n";
        exit(EXIT_FAILURE);
    }
    std::cout << "I2C communication successfully setup.\n";
    setConfigReg();
}

void ADS1115::setConfigReg(){
    // config = (ADS1115_CONFIG_OS<<15) | (ADS1115_CONFIG_MUX<<12)
    //     | (ADS1115_CONFIG_PGA<<9) | (ADS1115_CONFIG_MODE<<8) 
    //     | (ADS1115_CONFIG_DR<<5)| (ADS1115_CONFIG_COMP_MODE<<4) 
    //     | (ADS1115_CONFIG_COMP_POL<<3) | (ADS1115_CONFIG_COMP_LAT<<2) 
    //     | (ADS1115_CONFIG_COMP_QUE<<0);

        
    uint16_t config_now = wiringPiI2CReadReg16(fd, ADS1115_CONFIG_REG); 
    config_now = ((config_now << 8) & 0xFF00) | (config_now >> 8);
    std::cout << "config register value now:" << std::hex << config_now <<std::endl;
    if (config_now != 0x6c4){
        fprintf(stderr,"adc config is not set up correctly, need to run the setup python file first\n");
        exit(EXIT_FAILURE);
    }

    // std::cout << "config register value we want to:" << config <<std::endl;
    // // wiringPiI2CWriteReg16(fd, ADS1115_CONFIG_REG, config);                      // write to config reg, note this is [LSB,MSB]
    // uint16_t swapped = ((config >> 8) & 0xFF) | ((config & 0xFF) << 8);
    // wiringPiI2CWriteReg16(fd, ADS1115_CONFIG_REG, swapped);

    // uint16_t config = wiringPiI2CReadReg16(fd, ADS1115_CONFIG_REG); 
    // std::cout << "config register value after write:" << config <<std::endl;
}
