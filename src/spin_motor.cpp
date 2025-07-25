#include "spin_motor.h"

static bool RUN = true;
mab::canId_t test_motor_id = 100;
mab::canId_t drive_motor_id = 150;

int main(){
  createSigIntHandler(); // start e stop to catch ctrl + c

  // initialize candle
  Candle *candle = initializeCandle();
  initializeMotors(candle, test_motor_id, drive_motor_id);
  
  // make looptimer
  LoopTimer lt;

  // initialize clutch variable
  bool clutch_engaged = false;


  // check time at start
  uint64_t start_time = rtcNsSinceEpoch();
  uint64_t curr_time = rtcNsSinceEpoch();
  uint64_t time_elapsed = 0;

  // setup output csv file and write header
  std::ofstream output_file;
  output_file.open("../data/gearmotor_test");
  std::string header = "time, drive_pos, drive_torque, test_pos, test_torque, sensor_torque";
  output_file << header << std::endl;

  // // setup gpio pin for clutch
  // wiringPiSetupGpio();
  // pinMode(17, OUTPUT);
  // digitalWrite(17, LOW);

  // initialize adc
  ADS1115 adc_ = ADS1115();
  mab::MD test_motor(test_motor_id, candle);
  mab::MD drive_motor(drive_motor_id, candle);
  drive_motor.setVelocityPIDparam(5, .5, 0, 0.1);



  /* MAIN LOOP */

  std::cout << "[DYNO] " << BOLDBLUE << "MAIN LOOP" << STDTEXT << std::endl;
  // Begin CANdle update loop in the background
  // candle->begin();
  // loop for 10 seconds
  while (RUN) {
    // checkMotorConnections(candle);
    // Grab a ref to motor
    // auto &motor = candle->md80s[0];
    curr_time = rtcNsSinceEpoch();
    time_elapsed = curr_time - start_time;

    double seconds = double(time_elapsed) / 1e9;

    double des_tau = int(seconds) % 10;
    double des_vel = int(int(seconds) / 10);

    std::cout << des_tau << ',' << des_vel << std::endl;

    test_motor.setTargetTorque(des_tau);
    // motor.setTargetTorque(-5);
    drive_motor.setTargetVelocity(des_vel);

    // write things to file
    output_file << time_elapsed << ',' << drive_motor.getPosition().first << ',' << drive_motor.getTorque().first << ','<< test_motor.getPosition().first << ',' << test_motor.getTorque().first << ',' << adc_.readTorque() << std::endl;
    // if(time_elapsed > 5e10 && !clutch_engaged && abs(motor.getPosition()) < 0.2){
    //   clutch_engaged = true;
    //   digitalWrite(17, HIGH);
      
    // }

    // if(time_elapsed > 1e11){
    //   break;
    // }

    // Throttle loop frequency to 1 kHz
    lt.wait(2e6);
    
  }

  // digitalWrite(17, LOW);

  // auto &motor = candle->md80s[0];
  drive_motor.setTargetVelocity(0);
  test_motor.setTargetTorque(0);
  lt.wait(1e9);
  std::cout << "DONE" << std::endl;

}

void createSigIntHandler() {
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = handleKill;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);
  }
  
  void handleKill(int s) { RUN = false;}