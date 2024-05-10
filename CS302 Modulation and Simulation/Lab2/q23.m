% Define parameters
k1 = 0.0231;
k2 = 0.0231;
x0 = 1;

% Define time range
t = 0:0.1:400;  % Time in hours, with a step of 0.1 hour

% Solve the differential equations using Euler's method
x = x0 * exp(-k1*t);
y = zeros(size(t));
for i = 2:length(t)
    y(i) = y(i-1) + (k1*x(i-1) - k2*y(i-1)) * (t(i) - t(i-1));
end

% Find peak value and time of y(t)
[peak_y, peak_time_index] = max(y);
peak_time = t(peak_time_index);

% Plot the results
plot(t, x, 'r', t, y, 'b--');
xlabel('Time (hours)');
ylabel('Concentration');
title('Concentration of Medicine in GI Tract and Bloodstream');
legend('x(t) in GI Tract', 'y(t) in Bloodstream');


% Display peak information
fprintf('Peak value of y(t): %.4f\n', peak_y);
fprintf('Time of peak y(t): %.2f hours\n', peak_time);
