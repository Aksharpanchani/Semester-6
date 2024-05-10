r = 1;
k = 1000;
x0 = 10;
h = 0;  % No harvesting (h = 0)
t_max = 10;  % Simulation time
dt = 0.01;

t = 0:dt:t_max;

% Analytical solution
analytical_x = k ./ (1 + (k/x0 - 1) * exp(-r*t));

% Numerical solution (Euler's method)
numerical_x = zeros(1, length(t));
numerical_x(1) = x0;  % Initial condition
for i = 2:length(t)
    numerical_x(i) = numerical_x(i-1) + dt * (r * numerical_x(i-1) * (1 - numerical_x(i-1)/k) - h);
end

% Calculate relative error
relative_error = abs(analytical_x - numerical_x) ./ analytical_x;

% Plot analytical and numerical solutions
figure;
subplot(2,1,1);
plot(t, analytical_x, 'b-', 'LineWidth', 1.5, 'DisplayName', 'Analytical Solution');
hold on;
plot(t, numerical_x, 'r--', 'LineWidth', 1.5, 'DisplayName', 'Numerical Solution (Euler Method)');
xlabel('t');
ylabel('x');
title('Comparison of Analytical and Numerical Solutions');
legend;
grid on;

% Plot relative error
subplot(2,1,2);
plot(t, relative_error, 'g-', 'LineWidth', 1.5);
xlabel('t');
ylabel('Relative Error');
title('Relative Error between Analytical and Numerical Solutions');
grid on;

% Optional: Save the plot
% print('comparison_plot.png', '-dpng');
