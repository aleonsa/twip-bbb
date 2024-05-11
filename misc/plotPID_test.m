% Read the CSV file
data = readtable('pid_test.csv');

% Extract the columns
time = data.Time;
pitch = data.Pitch;
error = data.Error;
dError = data.dError;
intError = data.IntError;
controlSignal = data.ControlSignal;

% Create a new figure
figure;

% Plot Pitch
subplot(5,1,1);
plot(time, pitch);
title('Pitch');
xlabel('Time');
ylabel('Pitch');

% Plot Error
subplot(5,1,2);
plot(time, error);
title('Error');
xlabel('Time');
ylabel('Error');

% Plot Derivative of Error
subplot(5,1,3);
plot(time, dError);
title('Derivative of Error');
xlabel('Time');
ylabel('dError');

% Plot Integral of Error
subplot(5,1,4);
plot(time, intError);
title('Integral of Error');
xlabel('Time');
ylabel('IntError');

% Plot Control Signal
subplot(5,1,5);
plot(time, controlSignal);
title('Control Signal');
xlabel('Time');
ylabel('ControlSignal');