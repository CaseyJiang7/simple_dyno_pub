#include "spin_clutch.h"



static bool RUN = true;

int main(){


  Candle *candle = initializeCandle();
  initializeMotors(candle);
  LoopTimer lt;
  bool clutch_engaged = false;

  uint64_t start_time = rtcNsSinceEpoch();
  uint64_t curr_time = rtcNsSinceEpoch();
  uint64_t time_elapsed = 0;

  std::ofstream output_file;
  output_file.open("../data/clutch_test_2");
  std::string header = "time, motor_pos, motor_vel, motor_torque, clutch_engaged";
  output_file << header << std::endl;

  wiringPiSetupGpio();
  pinMode(17, OUTPUT);


  /* MAIN LOOP */

  std::cout << "[CLUTCH] " << BOLDBLUE << "MAIN LOOP" << STDTEXT << std::endl;
  // Begin CANdle update loop in the background
  candle->begin();
  // loop for 10 seconds
  while (RUN) {
    checkMotorConnections(candle);
    // Grab a ref to motor
    auto &motor = candle->md80s[0];
    motor.setImpedanceControllerParams(3.5, 0.25);
    curr_time = rtcNsSinceEpoch();
    time_elapsed = curr_time - start_time;
    motor.setTargetTorque(sin(time_elapsed * 1e-8) * 5 - 5);
    // motor.setTargetTorque(-5);
    // Throttle loop frequency to 1 kHz
    lt.wait(1e6);

    // write things to file
    output_file << time_elapsed << ',' << motor.getPosition() << ',' << motor.getVelocity() << ',' << motor.getTorque() << ',' << clutch_engaged << std::endl;
    if(time_elapsed > 1e9 && !clutch_engaged){
      clutch_engaged = true;
      digitalWrite(17, HIGH);
    }

    if(time_elapsed > 1e10){
      break;
    }
    
  }


  auto &motor = candle->md80s[0];
  motor.setTargetVelocity(0);
  motor.setTargetTorque(0);


    

}

void createSigIntHandler() {
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = handleKill;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);
  }
  
  void handleKill(int s) { RUN = false; }