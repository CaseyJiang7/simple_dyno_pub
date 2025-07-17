clear
close all

A = readtable("clutch_test_4");

pos = A.motor_pos;
vel = A.motor_vel;
tau = A.motor_torque;
clutch = logical(A.clutch_engaged);
time = A.time * 1e-9;


fps = 10;
r = rateControl(fps);
spmd
    fig = figure('Position', [0 0 1920 1080]);
    % set(fig,'defaultLegendAutoUpdate','off');
    tiledlayout(1,2)
    ax1 = nexttile;
    colororder([[ 0    0.4470    0.7410]; 0.8500    0.3250    0.0980])
    xlim([0, 100])
    
    ylabel("Motor Position (rads)")
    yyaxis(ax1, 'right')
    ylabel("Motor Torque (Nm)")
    xlabel("Time (s)")
    ax2 = nexttile;
    % colororder([[ 0    0    1]; [1    0    0]])
    ylabel("Motor Torque (Nm)");
    xlabel("Motor Position (rads)")
end


numFrames = floor(time(end) * fps);

frames = cell(numFrames, 1);


parfor i = 1:numFrames
    now = time * fps < i;

    pos_now = pos(now);
    % vel_now = vel(now);
    tau_now = tau(now);
    clutch_now = clutch(now);
    time_now = time(now);
    % nexttile
    yyaxis(ax1, 'left')
    plot(ax1, time_now, pos_now, Marker='none', LineStyle='-');
    yyaxis(ax1,'right')
    plot(ax1, time_now, tau_now, Marker='none', LineStyle='-');


    % nexttile
    scatter(ax2, pos_now(clutch_now), tau_now(clutch_now), Marker='.', MarkerEdgeColor='r')
    scatter(ax2, pos_now(~clutch_now), tau_now(~clutch_now), Marker='.', MarkerEdgeColor='b')

    legend(ax1, "Motor Position", "Motor Torque")
    legend(ax2, "CLutch Disengaged", "Clutch Engaged")

    frame = getframe(gcf);

    frames(i) = frame.cdata;

end
v = VideoWriter("plot_video_exp4.mp4","MPEG-4");
open(v)
for idx = 1:numel(frames)
    writeVideo(v,frames{idx});
end
close(v)