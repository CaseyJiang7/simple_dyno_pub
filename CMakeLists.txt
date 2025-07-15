# Set CMake and C++ requirements
cmake_minimum_required(VERSION 2.8.12)
set(CMAKE_CXX_STANDARD 14)

# Define project name
project(hopper)

# find_package(lcm REQUIRED)
# include(${LCM_USE_FILE})

# Add subdirectories for MAB CANdle package and VectorNav package
add_subdirectory(external/candle candle)

# This line prevents examples 1-15 from being compiled during a typical "make" command
set_property(DIRECTORY external/candle/ PROPERTY EXCLUDE_FROM_ALL YES)

# Allow all executables to access headers from VectorNav
include_directories(
    external/libvncxx/include
    /usr/local/include/eigen3/Eigen
)

# Create middleware executable
# add_executable(middleware src/middleware.cpp
#     src/hopper.cpp
#     src/global_utils.cpp
#     src/imu_utils.cpp
#     src/motor_utils.cpp
#     src/loop_timer.cpp
#     src/kalman_filter.cpp
# )
# Headers are in include/
# target_include_directories(middleware PUBLIC
#     include
# )
# Link it to MAB CANdle, VectorNav, and pthread packages
# target_link_libraries(middleware
#     candle
#     # libvncxx
#     # pthread
#     # lcm
# )

# Create upper computer simulator executable
# add_executable(upper_computer_simulator src/upper_computer_simulator.cpp
#     src/global_utils.cpp
#     src/loop_timer.cpp
# )
# Headers are in include/
# target_include_directories(upper_computer_simulator PUBLIC
#     include
# )
# # Link it to MAB CANdle, VectorNav, and pthread packages
# target_link_libraries(upper_computer_simulator
#     pthread
#     lcm
# )


# Create encoder test executable
add_executable(clutch src/spin_clutch.cpp
    src/loop_timer.cpp
    src/global_utils.cpp
    src/motor_utils.cpp
    src/ADS1115.cpp
)
target_include_directories(clutch PUBLIC
    include
)
target_link_libraries(clutch
    candle
    wiringPi
)