#include "global_utils.h"

using namespace std;

uint64_t rtcNsSinceEpoch() {
  struct timespec ts;
  if (clock_gettime(CLOCK_REALTIME, &ts) == 0) {
    // return 1e9 * (seconds) + (nanoseconds)
    return static_cast<uint64_t>(ts.tv_sec) * 1000000000ULL +
           static_cast<uint64_t>(ts.tv_nsec);
  } else {
    cout << "Failed to retrieve time from real-time clock. Aborted." << endl;
    exit(EXIT_FAILURE);
  }
}

// ostream &operator<<(ostream &os,
//                     const hopper_lcm_msgs::hopper_control_t &command) {
//   os << command.timestamp << ',' << command.qj_tau[0] << ','
//      << command.qj_tau[1] << ',' << command.qj_pos[0] << ','
//      << command.qj_pos[1] << ',' << command.qj_vel[0] << ','
//      << command.qj_vel[1] << ',' << command.kp[0] << ',' << command.kp[1] << ','
//      << command.kd[0] << ',' << command.kd[1];
//   return os;
// }

// ostream &operator<<(ostream &os, const hopper_lcm_msgs::hopper_state_t &state) {
//   os << state.timestamp << ',' << state.rpy[0] << ',' << state.rpy[1] << ','
//      << state.rpy[2] << ',' << state.velocity[0] << ',' << state.velocity[1]
//      << ',' << state.velocity[2] << ',' << state.omega[0] << ','
//      << state.omega[1] << ',' << state.omega[2] << ',' << state.qj_pos[0] << ','
//      << state.qj_pos[1] << ',' << state.qj_pos[2] << ',' << state.qj_vel[0]
//      << ',' << state.qj_vel[1] << ',' << state.qj_vel[2] << ','
//      << state.qj_tau[0] << ',' << state.qj_tau[1] << ',' << state.qj_tau[2];
//   return os;
// }