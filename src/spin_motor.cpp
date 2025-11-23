#include "spin_motor.h"
#include "loop_timer.h"

static bool RUN = true;
mab::canId_t test_motor_id = 130;
mab::canId_t drive_motor_id = 130;

int main(int argc, char** argv){
    createSigIntHandler(); // start e stop to catch ctrl + c
    std::cout << "[DEBUG] Signal handler installed" << std::endl;

    // initialize candle
    std::cout << "[DEBUG] Initializing Candle..." << std::endl;
    Candle *candle = initializeCandle();
    if (!candle) {
        std::cerr << "[ERROR] Candle failed to initialize!" << std::endl;
        return -1;
    }

    std::cout << "[DEBUG] Candle initialized at " << candle << std::endl;

    // start setting up output file
    std::ofstream output_file;
    double setVelocity;
    double testTorque;
    std::ostringstream filename;
    parse_motor_flags(argc, argv, setVelocity, testTorque);

    initializeMotors(candle, test_motor_id, drive_motor_id);
    std::cout << "[DEBUG] Motors initialized (test: " << test_motor_id << ", drive: " << drive_motor_id << ")" << std::endl;

    // make looptimer
    LoopTimer lt;
    std::cout << "[DEBUG] LoopTimer created" << std::endl;

    // initialize clutch variable
    bool clutch_engaged = false;

    // check time at start
    uint64_t start_time = rtcNsSinceEpoch();
    uint64_t curr_time = rtcNsSinceEpoch();
    uint64_t time_elapsed = 0;
    std::cout << "[DEBUG] Start time: " << start_time << std::endl;

    // setup output csv file and write header
    
    filename << "../data/drive_log" 
            << "_vel" << setVelocity
            << "_t" << testTorque
            << ".csv";
    output_file.open(filename.str());
    std::string header = "system time, time elapsed, drive_pos, drive_torque";
    output_file << header << std::endl;
    std::cout << "[DEBUG] Output CSV file created" << std::endl;

    // initialize adc
    // ADS1115 adc_ = ADS1115();
    std::cout << "[DEBUG] ADS1115 ADC initialized" << std::endl;

    // mab::MD test_motor(test_motor_id, candle);
    mab::MD drive_motor(drive_motor_id, candle);
    drive_motor.setVelocityPIDparam(5, .5, 0, 0.1);
    std::cout << "[DEBUG] MD motors created and drive PID set" << std::endl;

    /* MAIN LOOP */
    std::cout << "[DYNO] " << BOLDBLUE << "MAIN LOOP" << STDTEXT << std::endl;

    while (RUN) {
        auto now = std::chrono::system_clock::now();
        uint64_t now_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(
                          now.time_since_epoch()).count();
        curr_time = rtcNsSinceEpoch();
        time_elapsed = curr_time - start_time;
        double seconds = double(time_elapsed) / 1e9;

        double des_tau = int(seconds) % 10 - 5;



        // test_motor.setTargetTorque(des_tau);
        drive_motor.setTargetVelocity(setVelocity);
        // drive_motor.setTargetTorque(1);

        // write things to file
        double kt = 0.105;
        output_file << now_ns << ','
                    << time_elapsed << ',' 
                    << drive_motor.getPosition().first << ',' 
                    << drive_motor.getTorque().first << ','
                    // << test_motor.getPosition().first << ',' 
                    // << test_motor.getTorque().first;
                    // << adc_.readTorque() << std::endl;
                    << std::endl;

        std::cout << "[DEBUG] Time: " << seconds 
                //   << ", sensor_torque: " << adc_.readTorque() 
                  << ", drive position: "<< drive_motor.getPosition().first <<", "
                  << "drive torque: "<<drive_motor.getTorque().first << ","
                  << ", current: " <<drive_motor.getTorque().first / kt
                  << std::endl;

        // if(time_elapsed > 1e10){
        //     std::cout << "[DEBUG] 10s elapsed, exiting main loop" << std::endl;
        //     break;
        // }

        // Throttle loop frequency to 1 kHz
        lt.wait(2e6);
    }

    drive_motor.setTargetVelocity(10);
    // test_motor.setTargetTorque(0);
    lt.wait(1e9);
    std::cout << "[DEBUG] Motors stopped, exiting program" << std::endl;
    std::cout << "DONE" << std::endl;
    mab::detachCandle(candle);
}

void createSigIntHandler() {
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = handleKill;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);
    std::cout << "[DEBUG] SIGINT handler created" << std::endl;
}

void handleKill(int s) {
    RUN = false;
    std::cout << "[DEBUG] SIGINT caught, setting RUN = false" << std::endl;
}

void parse_motor_flags(int argc, char** argv,
                              double& velocity,
                              double& torque_cmd)
{
    bool vel_set = false;
    bool torque_set = false;

    int opt;
    while ((opt = getopt(argc, argv, "i:v:t:h")) != -1) {
        switch (opt) {
            case 'v':
                velocity = std::stod(optarg);
                vel_set = true;
                break;

            case 't':
                torque_cmd = std::stod(optarg);
                torque_set = true;
                break;

            case 'h':
                std::cout << "Usage: " << argv[0] << " -v <velocity> -t <torque> [-i <id>]\n"
                          << "  -v <velocity>  REQUIRED: target velocity\n"
                          << "  -t <torque>    REQUIRED: feedforward torque\n"
                          << "  -i <id>        OPTIONAL: servo ID (default = "
                          << "  -h             Show help\n";
                std::exit(0);

            default:
                std::cerr << "Unknown option. Use -h for help.\n";
                std::exit(1);
        }
    }

    if (!vel_set) {
        std::cerr << "Error: missing required argument -v <velocity>\n";
    }

    if (!torque_set) {
        std::cerr << "Error: missing required argument -t <torque>\n";
    }

    if (!vel_set || !torque_set){
        std::exit(1);
    }
}
