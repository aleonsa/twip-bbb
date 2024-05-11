#include "kalman.h" // Include the Kalman Filter header file
#include <chrono> // Include the chrono library for time functions

// Constructor for the KalmanFilter class
KalmanFilter::KalmanFilter(double angle, double bias, double measure)
{
    // Initialize the process noise covariances for the angle and bias
    Q_angle = angle;
    Q_bias = bias;

    // Initialize the measurement noise covariance
    R_measure = measure;

    // Initialize the angle and bias
    K_angle = 0;
    K_bias = 0;

    // Initialize the error covariance matrix P
    P[0][0] = 0;
    P[0][1] = 0;
    P[1][0] = 0;
    P[1][1] = 0;

    // Get the current time in microseconds
    kt = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
}

// Update function for the KalmanFilter class
double KalmanFilter::update(double newValue, double newRate)
{
    // Get the current time in microseconds
    double now = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();

    // Calculate the time difference in seconds
    dt = (now - kt) / 1000000.0;

    // Update the rate and angle
    K_rate = newRate - K_bias;
    K_angle += dt * K_rate;

    // Update the error covariance matrix P
    P[0][0] += dt * (P[1][1] + P[0][1]) + Q_angle * dt;
    P[0][1] -= dt * P[1][1];
    P[1][0] -= dt * P[1][1];
    P[1][1] += Q_bias * dt;

    // Calculate the residual
    S = P[0][0] + R_measure;

    // Calculate the Kalman gain
    K[0] = P[0][0] / S;
    K[1] = P[1][0] / S;

    // Calculate the measurement residual
    y = newValue - K_angle;

    // Update the angle and bias
    K_angle += K[0] * y;
    K_bias += K[1] * y;

    // Update the error covariance matrix P
    P[0][0] -= K[0] * P[0][0];
    P[0][1] -= K[0] * P[0][1];
    P[1][0] -= K[1] * P[0][0];
    P[1][1] -= K[1] * P[0][1];

    // Update the time
    kt = now;

    // Return the updated angle
    return K_angle;
};