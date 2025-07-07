#include "spin_clutch.h"



static bool RUN = true;

int main(){


  Candle *candle = initializeCandle();
  initializeMotors(candle);
  LoopTimer lt;

  /* MAIN LOOP */

  std::cout << "[HOPPER] " << BOLDBLUE << "MAIN LOOP" << STDTEXT << std::endl;
  // Begin CANdle update loop in the background
  candle->begin();
  // loop for 10 seconds
  while (RUN) {
    checkMotorConnections(candle);
    // Grab a ref to motor
    auto &motor = candle->md80s[0];
    // The upper computer is not transmitting -- send soft stop
    motor.setImpedanceControllerParams(0.0f, 1.2);
    motor.setTargetVelocity(1);
    motor.setTargetTorque(1);
    // Throttle loop frequency to 1 kHz
    lt.wait(1e6);
  }

    

}

void createSigIntHandler() {
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = handleKill;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);
  }
  
  void handleKill(int s) { RUN = false; }