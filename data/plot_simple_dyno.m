clear
close all


A = readtable("gearmotor_test_9");
time = A.time * 1e-9;


plot(time, A.sensor_torque, time,A.test_torque)
xlabel("Time(s)")
ylabel("Torque (Nm)")
legend("Sensor Torque", "Desired Torque")


figure
plot(A.time, -A.drive_pos, A.time, -A.test_pos)

% plot(A.time, A.test_torque./A.sensor_torque)