clear
close all

A = readtable("clutch_test_6");

v = VideoWriter("leg_spring_plot.mp4","MPEG-4");

pos = A.motor_pos;
vel = A.motor_vel;
tau = A.sensor_torque;
clutch = logical(A.clutch_engaged);
time = A.time * 1e-9;


fps = 5;
r = rateControl(fps);
v.FrameRate = fps;

fig = figure('Position', [0 0 1920 1080]);
set(fig,'defaultLegendAutoUpdate','off');
tiledlayout(1,2)
ax1 = nexttile;
colororder([[ 0    0.4470    0.7410]; 0.8500    0.3250    0.0980])
xlim([0, 100])

yyaxis(ax1, 'right')
ax2 = nexttile;
% colororder([[ 0    0    1]; [1    0    0]])


open(v)

writelegend = true;

for i = time(1):1/fps:time(end)
    now = time < i + 1/fps;

    pos_now = pos(now);
    % vel_now = vel(now);
    tau_now = tau(now);
    clutch_now = clutch(now);
    time_now = time(now);
    % nexttile
    yyaxis(ax1, 'left')
    plot(ax1, time_now, pos_now, Marker='none', LineStyle='-');
    ylabel(ax1, "Motor Position (rads)")
    yyaxis(ax1,'right')
    plot(ax1, time_now, tau_now, Marker='none', LineStyle='-');
    ylabel(ax1, "Motor Torque (Nm)")
    xlabel(ax1, "Time (s)")
    
    
    % nexttile
    scatter(ax2, pos_now(~clutch_now), tau_now(~clutch_now), Marker='.', MarkerEdgeColor='b')
    hold on
    scatter(ax2, pos_now(clutch_now), tau_now(clutch_now), Marker='.', MarkerEdgeColor='r')
    scatter(ax2, pos_now(end-100: end), tau_now(end-100: end), Marker='.', MarkerEdgeColor='g')
    hold off
    ylabel(ax2, "Motor Torque (Nm)");
    xlabel(ax2, "Motor Position (rads)")

    legend(ax1, "Motor Position", "Motor Torque")
    legend(ax2, "CLutch Disengaged", "Clutch Engaged")

    frame = getframe(gcf);
    writeVideo(v, frame.cdata)
    waitfor(r)
end
close(v)