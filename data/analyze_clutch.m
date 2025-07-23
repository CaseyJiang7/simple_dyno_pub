clear
close all

A = readtable("clutch_test_6");

pos = A.motor_pos;
vel = A.motor_vel;
tau_m = A.motor_torque;
tau_s = A.sensor_torque;
clutch = A.clutch_engaged;
clutch = boolean(clutch);



pos_en = -pos(clutch);
vel_en = vel(clutch);
tau_m_en = tau_m(clutch);
tau_s_en = tau_s(clutch);

pos_disen = -pos(~clutch);
vel_disen = vel(~clutch);
tau_m_disen = tau_m(~clutch);
tau_s_disen = tau_s(~clutch);

[pos_en_avg, tau_en_avg] = binned_averages(pos_en, tau_s_en, 100);
plot(pos_en_avg, tau_en_avg, LineWidth=3, Color='r')
hold on
scatter(pos_en, tau_s_en, 1, Marker='o', MarkerEdgeColor= 'none', MarkerFaceColor = 'r', MarkerFaceAlpha=0.1)
[pos_dis_avg, tau_dis_avg] = binned_averages(pos_disen, tau_s_disen, 100);
scatter(pos_disen, tau_s_disen, 1, Marker='o', MarkerEdgeColor= 'none', MarkerFaceColor = 'b', MarkerFaceAlpha=0.1)
plot(pos_dis_avg, tau_dis_avg, LineWidth=3, Color='b')
hold on

xlabel('Motor Angle (rads)')
ylabel('Sensor Torque (Nm)')

grid on
legend('Engaged', "", "","Disengaged", Location='northwest')
function [x_avgs, y_avgs] = binned_averages(x_in, y_in, num_bins)
    bins = linspace(min(x_in), max(x_in), num_bins+1);
    x_avgs = zeros(num_bins, 1);
    y_avgs = zeros(num_bins, 1);
    for idx = 1:num_bins
        x_avgs(idx) = mean(x_in(x_in >= bins(idx) & x_in <= bins(idx+1)));
        y_avgs(idx) = mean(y_in(x_in >= bins(idx) & x_in <= bins(idx+1)));
    end
end





