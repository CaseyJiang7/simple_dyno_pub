import time
import smbus2

# I2C device address
ADS1115_ADDRESS = 0x48

# Register addresses
ADS1115_CONVERSION_REG = 0x00
ADS1115_CONFIG_REG     = 0x01

# Config values
ADS1115_CONFIG_OS           = 0b0 << 15
ADS1115_CONFIG_MUX          = 0b000 << 12
ADS1115_CONFIG_PGA          = 0b011 << 9   # +/- 1.024V
ADS1115_CONFIG_MODE         = 0b0 << 8     # continous mode (not single shot)
ADS1115_CONFIG_DR           = 0b110 << 5   # 475 SPS
ADS1115_CONFIG_COMP_MODE    = 0b0 << 4
ADS1115_CONFIG_COMP_POL     = 0b0 << 3
ADS1115_CONFIG_COMP_LAT     = 0b1 << 2
ADS1115_CONFIG_COMP_QUE     = 0b00

# Combine all into one config word
config = (
    ADS1115_CONFIG_OS |
    ADS1115_CONFIG_MUX |
    ADS1115_CONFIG_PGA |
    ADS1115_CONFIG_MODE |
    ADS1115_CONFIG_DR |
    ADS1115_CONFIG_COMP_MODE |
    ADS1115_CONFIG_COMP_POL |
    ADS1115_CONFIG_COMP_LAT |
    ADS1115_CONFIG_COMP_QUE
)



# Initialize I2C bus
bus = smbus2.SMBus(1)
time.sleep(0.5)

# Read current config (for debug)
before = bus.read_word_data(ADS1115_ADDRESS, ADS1115_CONFIG_REG)
time.sleep(0.5)
before = ((before << 8) & 0xFF00) | (before >> 8)
print(f"Config register before write: {hex(before)}")

# Swap bytes (since ADS1115 expects MSB first)
config_swapped = ((config >> 8) & 0xFF) | ((config & 0xFF) << 8)
print(f"Config register aimed: {hex(config)}")
# Write config
bus.write_word_data(ADS1115_ADDRESS, ADS1115_CONFIG_REG, config_swapped)

# Wait for conversion to settle
time.sleep(0.01)

# Read back config (for debug)
after = bus.read_word_data(ADS1115_ADDRESS, ADS1115_CONFIG_REG)
after = ((after << 8) & 0xFF00) | (after >> 8)
print(f"Config register after write: {hex(after)}")
