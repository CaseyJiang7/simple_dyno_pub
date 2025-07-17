if true
    clear
    % A = readtable("clutch_test_1");
    B = readtable("leg_test");
end

close all
% positionA = A.motor_pos;
% velocityA = A.motor_vel;
% torqueA = A.motor_torque;
% timeA = A.time * 1e-9;
% 
% plot(timeA,positionA);
% hold on
% ylabel("Motor Position (rads)")
% yyaxis right
% plot(timeA, torqueA);
% ylabel("Motor Torque (Nm)")
% xlabel("Time (s)")
% legend("Motor Position", "Motor Torque")

figure
posB = B.motor_pos;
velB = B.motor_vel;
torqueB = B.motor_torque;
timeB = B.time * 1e-9;
plot(timeB,posB);
hold on
ylabel("Motor Position (rads)")
yyaxis right
plot(timeB, torqueB);
ylabel("Motor Torque (Nm)")
xlabel("Time (s)")
legend("Motor Position", "Motor Torque")

figure
scatter(posB(timeB < 5.368), torqueB(timeB < 5.368), 1,'red', ".")
hold on
scatter(posB(timeB > 5.368), torqueB(timeB > 5.368), 1,'blue', ".")
ylabel("Motor Torque (Nm)");
xlabel("Motor Position (rads)")
legend("Clutch Disengaged", "Clutch Engaged")
% figure
% scatter(positionA, torqueA, 1, ".")
% ylabel("Motor Torque (Nm)");
% xlabel("Motor Position (rads)")


