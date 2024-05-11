% Import the data
data = readtable('upright_angle_correction.csv');

% Plot the data
figure;
plot(data.Sample, data.Pitch);
title('Pitch vs Sample');
xlabel('Sample');
ylabel('Pitch');

% Filter the signal using a moving average filter
windowSize = 50; % Adjust this value based on your data
filteredPitch = movmean(data.Pitch, windowSize);

% Plot the filtered signal
figure;
plot(data.Sample, filteredPitch);
title('Filtered Pitch vs Sample');
xlabel('Sample');
ylabel('Filtered Pitch');

% Calculate the average of the pitch ignoring outliers
% Use the Median Absolute Deviation (MAD) method to detect outliers
MAD = mad(data.Pitch, 1);
threshold = 3; % Adjust this value based on your data
outliers = abs(data.Pitch - median(data.Pitch)) > threshold * MAD;
averagePitch = mean(data.Pitch(~outliers));

fprintf('The average pitch ignoring outliers is %f\n', averagePitch);