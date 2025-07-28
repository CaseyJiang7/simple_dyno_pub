clear
close all


A = readtable("gearmotor_test");



plot(A.time, A.sensor_torque, A.time,A.test_torque)