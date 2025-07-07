#include "motor_utils.h"

using namespace std;

Candle *initializeCandle() {
  Candle *candle = nullptr;
  size_t attempts = 0;
  while (!candle && attempts < 10) {
    try {
      attempts++;
      candle = new Candle(CAN_BAUD_1M, true, BusType_E::USB);
    } catch (...) {
      usleep(1e6);
    }
  }
  if (attempts < 10) {
    std::cout << "[CANDLE] Candle Initialization " << BOLDGREEN << "[PASSED]"
              << STDTEXT << std::endl;

  } else {
    std::cout << "[CANDLE] Candle Initialization failed after " << attempts
              << " attempts " << BOLDRED << "[FAILED]" << STDTEXT << std::endl;
    exit(EXIT_FAILURE);
  }
  return candle;
}

void initializeMotors(Candle *candle) {
  // Ping FDCAN bus in search of drives
  auto ids = candle->ping(mab::CAN_BAUD_8M);

  if (ids.size() != 2) {
    cout << "Expected 2 motors, found " << ids.size()
         << " motors instead. Aborted." << endl;
    exit(EXIT_FAILURE);
  }

  // Add all found to the update list
  for (auto &id : ids) {
    size_t attempts = 0;
    while (!candle->addMd80(id, false) && attempts < 10) {
      cout << "[CANDLE] Attempting to connect to motor with ID " << id
           << std::endl;
      attempts++;
      usleep(2e5);
    }
    if (attempts < 10) {
      usleep(2e5);
    } else {
      std::cout << "[CANDLE] Failed to connect to motor with ID " << id
                << " after " << attempts << " attempts " << BOLDRED
                << "[FAILED]" << STDTEXT << std::endl;
      exit(EXIT_FAILURE);
    }
  }

  // Now we shall loop over all found drives to change control mode and
  // enable them one by one
  for (auto &md : candle->md80s) {
    candle->controlMd80SetEncoderZero(md.getId());
    candle->controlMd80Mode(
        md, mab::Md80Mode_E::IMPEDANCE); // Set mode to impedance control
    candle->controlMd80Enable(md, true); // Enable the drive
    usleep(1e4);
  }
}

void checkMotorConnections(Candle *candle) {
  if (candle->md80s.size() != 2) {

    std::cout << "[CANDLE] Connected Motor IDs: ";
    for (size_t i = 0; i < candle->md80s.size(); ++i) {
      std::cout << candle->md80s[i].getId() << " ";
    }
    std::cout << std::endl;
    std::cout << "[CANDLE] Expected 2 motors, found " << candle->md80s.size()
              << BOLDRED << " [FAILED]" << STDTEXT << std::endl;
    exit(EXIT_FAILURE);
  }
}