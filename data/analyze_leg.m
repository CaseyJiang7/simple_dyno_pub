clear
close all

A = readtable("leg_test_sensor");

pos = -A.motor_pos - 0.775+pi/2;
vel = A.motor_vel;
tau_m = A.motor_torque;
tau_s = A.sensor_torque;
clutch = A.clutch_engaged;
clutch = boolean(clutch);

[pos_avg, tau_avg] = binned_averages(pos, tau_s, 100);
plot(pos_avg, tau_avg, LineWidth=3, Color='b')
hold on
s = scatter(pos, tau_s, 2, Marker='o', MarkerEdgeColor= 'none', MarkerFaceColor = 'b', MarkerFaceAlpha=0.1);

B = readtable("leg_test_sensor_lub_2");

pos = -B.motor_pos - 0.2 - 0.775+pi/2;
vel = B.motor_vel;
tau_m = B.motor_torque;
tau_s = B.sensor_torque;
clutch = B.clutch_engaged;
clutch = boolean(clutch);

[pos_avg, tau_avg] = binned_averages(pos, tau_s, 100);
plot(pos_avg, tau_avg, LineWidth=3, Color='r')
hold on
s = scatter(pos, tau_s, 2, Marker='o', MarkerEdgeColor= 'none', MarkerFaceColor = 'r', MarkerFaceAlpha=0.1);

xlabel('Motor Angle (rads)')
ylabel('Motor Torque (Nm)')

grid on

l0 = 0.134;      % spring resting length
l1 = 0.03;      % Parallelogram short side
l2 = 0.1;       % PEA link
l3 = 0.22;      % Parallelogram long side
l4 = 0.02;      % PEA link offset from knee joint
l5 = 0.01;      % orthogonal offset from slider
l6 = 0.003;     % parallel offset from slider
Ks_val = 4115;     % spring coefficient
l_val = [l0; l1; l2; l3; l4; l5; l6];

Q = deg2rad(linspace(50,160,100)) - pi/2;

tau_model = get_spring_torque(Q, Ks_val, l_val);
plot(Q+pi/2, tau_model, Color='g', LineWidth=3)


legend('No Lubrication', "", "Lubrication",  "","Predicted", Location='northwest')

function [x_avgs, y_avgs] = binned_averages(x_in, y_in, num_bins)
    bins = linspace(min(x_in), max(x_in), num_bins+1);
    x_avgs = zeros(num_bins, 1);
    y_avgs = zeros(num_bins, 1);
    for idx = 1:num_bins
        x_avgs(idx) = mean(x_in(x_in >= bins(idx) & x_in <= bins(idx+1)));
        y_avgs(idx) = mean(y_in(x_in >= bins(idx) & x_in <= bins(idx+1)));
    end
end





