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
  output_file.open("../data/gearmotor_test_1");
  std::string header = "time, pos0, torque0, pos1, torque1";
  output_file << header << std::endl;

  // // setup gpio pin for clutch
  // wiringPiSetupGpio();
  // pinMode(17, OUTPUT);
  // digitalWrite(17, LOW);

  // initialize adc
  // ADS1115 adc_ = ADS1115();
  mab::MD motor0(ids[0], candle);
  mab::MD motor1(ids[1], candle);
  motor0.setMotionMode(mab::MdMode_E::IMPEDANCE);
  motor1.setMotionMode(mab::MdMode_E::IMPEDANCE);
  motor0.enable();
  motor1.enable();
  motor0.setImpedanceParams(20, 2);
  motor1.setImpedanceParams(20, 2);



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

    // double des_tau = double((int(seconds) % 22 - 11)/ abs(int(seconds) % 22 - 11)) * ((int(seconds) % 11) - 5) / 5.0;
    // double des_tau = sin(seconds);
    // if( seconds > 5){
    //   des_tau = des_tau * 2;}
    // double des_vel = - int(int(seconds) / 11) % 5;
    // double des_tau = 0;
    // double des_vel = 0;
    double des_pos1 = sin(seconds)/2;
    double des_pos2 = cos(seconds)/2;


    motor0.setTargetPosition(des_pos1);
    motor1.setTargetPosition(des_pos2);
    // test_motor.setTargetTorque(des_tau);

    // test_motor.setTargetVelocity(-des_vel);
    // test_motor.setTargetVelocity(drive_motor.getVelocity().first);
    // drive_motor.setTargetVelocity(des_vel);
    // drive_motor.setMaxTorque(des_tau * 1.5);
    // test_motor.setTargetPosition(0);
    // motor.setTargetTorque(-5);
    // drive_motor.setTargetPosition(time_elapsed * 1e-9);


    // write things to file
    output_file << time_elapsed << ',' << motor0.getPosition().first << ',' << motor0.getTorque().first << ','<< motor1.getPosition().first << ',' << motor1.getTorque().first  << std::endl;
    // if(time_elapsed > 5e10 && !clutch_engaged && abs(motor.getPosition()) < 0.2){
    //   clutch_engaged = true;
    //   digitalWrite(17, HIGH);
      
    // }
    std::cout << motor0.getPosition().first << ',' << motor0.getTorque().first << ','<< motor1.getPosition().first << ',' << motor1.getTorque().first <<std::endl;

    if(time_elapsed > 60e9){
      break;
    }

    // Throttle loop frequency to 1 kHz
    lt.wait(2e6);
    
  }

  // digitalWrite(17, LOW);

  // auto &motor = candle->md80s[0];
  motor0.setTargetVelocity(0);
  motor1.setTargetVelocity(0);
  motor0.setTargetPosition(0);
  motor1.setTargetPosition(0);
  // motor0.setTargetTorque(0);
  // motor1.setTargetTorque(0);
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