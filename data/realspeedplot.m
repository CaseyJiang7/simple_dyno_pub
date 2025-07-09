clear
close all

A = readtable("clutch_test_3");

v = VideoWriter("plot_video.mp4","MPEG-4");

pos = A.motor_pos;
vel = A.motor_vel;
tau = A.motor_torque;
clutch = logical(A.clutch_engaged);
time = A.time * 1e-9;


fps = 30;
r = rateControl(fps);

figure('Position', [0 0 1920 1080])
tiledlayout(1,2)
ax1 = nexttile;
hold on
colororder([[ 0    0.4470    0.7410]; 0.8500    0.3250    0.0980])
xlim([0, 10])

ylabel("Motor Position (rads)")
yyaxis(ax1, 'right')
ylabel("Motor Torque (Nm)")
xlabel("Time (s)")
ax2 = nexttile;
hold on
% colororder([[ 0    0    1]; [1    0    0]])
ylabel("Motor Torque (Nm)");
xlabel("Motor Position (rads)")


open(v)

for i = time(1):1/fps:time(end)
    now = time > i & time < i + 1/fps;

    pos_now = pos(now);
    % vel_now = vel(now);
    tau_now = tau(now);
    clutch_now = clutch(now);
    time_now = time(now);
    % nexttile
    yyaxis(ax1, 'left')
    plot(ax1, time_now, pos_now, Marker='none', LineStyle='-', DisplayName="Motor Position");
    yyaxis(ax1,'right')
    plot(ax1, time_now, tau_now, Marker='none', LineStyle='-', DisplayName="Motor Torque");
    
    % nexttile
    scatter(ax2, pos_now(clutch_now), tau_now(clutch_now), Marker='.', MarkerEdgeColor='r', DisplayName="Clutch Disengaged")
    scatter(ax2, pos_now(~clutch_now), tau_now(~clutch_now), Marker='.', MarkerEdgeColor='b', DisplayName="Clutch Engaged")

    frame = getframe(gcf);
    writeVideo(v, frame.cdata)
end
close(v)