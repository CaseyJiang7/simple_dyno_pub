#include "motor_utils.h"

using namespace std;

Candle *initializeCandle() {
  Candle *candle = nullptr;
  size_t attempts = 0;
  while (!candle && attempts < 10) {
    try {
      attempts++;
      candle =
      mab::attachCandle(mab::CANdleBaudrate_E::CAN_BAUD_8M, mab::candleTypes::busTypes_t::USB);
    } catch (...) {
      usleep(1e6);
    }
  }
  if (attempts < 10) {
    std::cout << "[DYNO] Candle Initialization " << BOLDGREEN << "[PASSED]"
              << STDTEXT << std::endl;

  } else {
    std::cout << "[DYNO] Candle Initialization failed after " << attempts
              << " attempts " << BOLDRED << "[FAILED]" << STDTEXT << std::endl;
    exit(EXIT_FAILURE);
  }
  return candle;
}

std::vector<mab::canId_t> initializeMotors(Candle *candle) {
  // Ping FDCAN bus in search of drives
  auto ids = mab::MD::discoverMDs(candle);
  // Check that we found exactly two motors
  if (ids.size() != 2) {
    cout << "Expected 2 motors, found " << ids.size()
         << " motors instead. Aborted." << endl;
    exit(EXIT_FAILURE);
  }
  // if (ids[0] != drive_motor_id && ids[1] != drive_motor_id) {
  //   cout << "[DYNO] Failed to find specified drive motor (ID: "
  //        << drive_motor_id << "). " << BOLDRED << "[FAILED]" << STDTEXT << endl;
  //   exit(EXIT_FAILURE);
  // }
  // if (ids[0] != test_motor_id && ids[1] != test_motor_id) {
  //   cout << "[DYNO] Failed to find specified test motor (ID: " << test_motor_id
  //        << "). " << BOLDRED << "[FAILED]" << STDTEXT << endl;
  //   exit(EXIT_FAILURE);
  // }

  std::vector<mab::MD> mds;

  // Add all found to the update list
  for (auto &id : ids) {
    size_t attempts = 0;
    bool succeed = false;
    mab::MD md(id, candle);
    while (!succeed && attempts < 5) {
      cout << "[CANDLE] Attempting to connect to motor with ID " << id
           << std::endl;
      if (md.init() == mab::MD::Error_t::OK){
          mds.push_back(md);
          succeed = true;}
      else{
        attempts++;
      }
      usleep(1e6);
    }
    if (attempts < 5) {
      usleep(2e5);
    } else {
      std::cout << "[CANDLE] Failed to connect to motor with ID " << id
                << " after " << attempts << " attempts " << BOLDRED
                << "[FAILED]" << STDTEXT << std::endl;
      exit(EXIT_FAILURE);
    }
    md.zero();
    // if(id == test_motor_id){
    //   md.setMotionMode(mab::MdMode_E::IMPEDANCE);
    // } else if (id == drive_motor_id)
    // {
    //   md.setMotionMode(mab::MdMode_E::IMPEDANCE);
    // } else{
    //   cout << "[DYNO] Motor ID not specified"<< endl;
    //   exit(EXIT_FAILURE);
    // }
  }
  // mds[0].enable();
  // mds[1].enable();
  return ids;
}