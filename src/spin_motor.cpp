#include "spin_motor.h"

static bool RUN = true;
mab::canId_t test_motor_id = 100;
mab::canId_t drive_motor_id = 150;

int main(){
  createSigIntHandler(); // start e stop to catch ctrl + c

  // initialize candle
  Candle *candle = initializeCandle();
  std::vector<mab::canId_t> ids = initializeMotors(candle);
  
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
  output_file.open("../data/gearmotor_test_2");
  std::string header = "time, vel0, torque0, vel1, torque1";
  output_file << header << std::endl;

  // // setup gpio pin for clutch
  // wiringPiSetupGpio();
  // pinMode(17, OUTPUT);
  // digitalWrite(17, LOW);

  // initialize adc
  // ADS1115 adc_ = ADS1115();
  mab::MD test_motor(ids[0], candle);
  mab::MD drive_motor(ids[1], candle);
  test_motor.setMotionMode(mab::MdMode_E::RAW_TORQUE);
  drive_motor.setMotionMode(mab::MdMode_E::IMPEDANCE);
  test_motor.enable();
  drive_motor.enable();
  test_motor.setMaxTorque(15);
  drive_motor.setMaxTorque(15);
  // drive_motor.setVelocityPIDparam(20, 2, .5,2);
  test_motor.setImpedanceParams(20, 2);
  drive_motor.setImpedanceParams(0, 10);



  /* MAIN LOOP */

  std::cout << "[DYNO] " << BOLDBLUE << "MAIN LOOP" << STDTEXT << std::endl;
  // Begin CANdle update loop in the background
  // candle->begin();
  // loop for 10 seconds
  double des_pos = 0;
  uint64_t loop_time = 2e6;
  while (RUN) {
    // checkMotorConnections(candle);
    // Grab a ref to motor
    // auto &motor = candle->md80s[0];
    curr_time = rtcNsSinceEpoch();
    time_elapsed = curr_time - start_time;

    double seconds = double(time_elapsed) / 1e9;

    double des_tau = double((int(seconds) % 22 - 11)/ abs(int(seconds) % 22 - 11)) * ((int(seconds) % 11) - 5) / 2.5;
    // double des_tau = sin(seconds);
    // if( seconds > 5){
    //   des_tau = des_tau * 2;}
    double des_vel = - int(int(seconds) / 11) % 5;
    // des_pos += des_vel * loop_time * 1e-9;
    // double des_tau = 0;
    // double des_vel = 0;
    // double des_pos1 = sin(seconds)/2;
    // double des_pos2 = cos(seconds)/2;


    // test_motor.setTargetPosition(des_pos1);
    // drive_motor.setTargetPosition(des_pos2);
    test_motor.setTargetTorque(des_tau);

    // test_motor.setTargetVelocity(-des_vel);
    // test_motor.setTargetVelocity(drive_motor.getVelocity().first);
    drive_motor.setTargetVelocity(des_vel);
    // drive_motor.setTargetPosition(des_pos);
    drive_motor.setTargetTorque(-des_tau);
    // test_motor.setTargetPosition(0);
    // motor.setTargetTorque(-5);
    // drive_motor.setTargetPosition(time_elapsed * 1e-9);


    // write things to file
    output_file << time_elapsed << ',' << test_motor.getVelocity().first << ',' << test_motor.getTorque().first << ','<< drive_motor.getVelocity().first << ',' << drive_motor.getTorque().first  << std::endl;
    // if(time_elapsed > 5e10 && !clutch_engaged && abs(motor.getPosition()) < 0.2){
    //   clutch_engaged = true;
    //   digitalWrite(17, HIGH);
      
    // }
    std::cout << seconds << ',' << test_motor.getVelocity().first << ',' << test_motor.getTorque().first << ','<< drive_motor.getVelocity().first << ',' << drive_motor.getTorque().first <<std::endl;

    if(time_elapsed > 60e9){
      break;
    }

    // Throttle loop frequency
    lt.wait(loop_time);
    
  }

  // digitalWrite(17, LOW);

  // auto &motor = candle->md80s[0];
  test_motor.setTargetVelocity(0);
  drive_motor.setTargetVelocity(0);
  test_motor.setTargetPosition(0);
  drive_motor.setTargetPosition(0);
  // test_motor.setTargetTorque(0);
  // drive_motor.setTargetTorque(0);
  lt.wait(1e9);
  mab::detachCandle(candle);
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