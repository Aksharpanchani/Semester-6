clc;
clear all;
% Define parameters
k = 0.6931;
x0 = 1;

% Define time range
t = 0:0.1:25;  % Time in hours, with a step of 0.1 hour

% Solve the differential equations using Euler's method
x =@(t) x0 * exp(-k*t);
y = @(t) k*x0*t*exp(-k*t);


% Plot the results
plot(t, x, 'r');
hold on
plot( t, y, 'b--');
xlabel('Time (hours)');
ylabel('Concentration');
title('Concentration of Medicine in GI Tract and Bloodstream');
legend('x(t) in GI Tract', 'y(t) in Bloodstream');


