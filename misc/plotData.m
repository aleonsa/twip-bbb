% Read the CSV file
data = readtable('output.csv');

% Extract the time and pitch data
time = data.Time;
pitch = data.Pitch;

% Create a new figure
figure;

% Plot the pitch data against time
plot(time, pitch);

% Label the axes
xlabel('Time (ms)');
ylabel('Pitch');

% Set the title of the plot
title('Pitch vs Time');