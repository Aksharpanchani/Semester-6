% Parameters
k = 0.5;
l = 0.6;
h = 0.2;
g = 0.3;
alpha = 0.8;
beta = 0.7;

% Time parameters
t_start = 0;
t_end = 35;
dt = 0.01; % Choose a suitable time step

% Initial conditions
x0 = 1;
y0 = 1;

% Number of steps
num_steps = ceil((t_end - t_start) / dt);

% Arrays to store results
t_values = zeros(1, num_steps);
x_values = zeros(1, num_steps);
y_values = zeros(1, num_steps);

% Initial values
x_values(1) = x0;
y_values(1) = y0;
t_values(1) = t_start;

% Euler's method
for i = 1:num_steps-1
    x_dot = k * y_values(i) + g - alpha * x_values(i);
    y_dot = l * x_values(i) + h - beta * y_values(i);
    
    x_values(i+1) = x_values(i) + dt * x_dot;
    y_values(i+1) = y_values(i) + dt * y_dot;
    
    t_values(i+1) = t_values(i) + dt;
end

% Plotting
figure;
subplot(2,1,1);
semilogy(t_values, x_values, 'b', t_values, y_values, 'r');
xlabel('Time');
ylabel('War-waging potential');
legend('x(t)', 'y(t)');
title('War-waging potential over time');
xlim([0 35]);
ylim([1 1.5]);
subplot(2,1,2);
plot(x_values, y_values, 'k');
xlabel('x');
ylabel('y');
title('Phase plot: y vs x');

