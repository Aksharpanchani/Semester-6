% Parameters
k = 0.5;
l = 0.6;
alpha = 0.8;

% Time parameters
t_start = 0;
t_end = 20;
dt = 0.01; % Choose a suitable time step

% Initial conditions
x0 = 1;
y0 = 0;

% Number of steps
num_steps = ceil((t_end - t_start) / dt);

% Arrays to store results
t_values = zeros(1, num_steps);
y_values = zeros(1, num_steps);

% Initial values
y_values(1) = y0;
t_values(1) = t_start;

% Euler's method for scenario C
for i = 1:num_steps-1
    y_dot = l * x0 - alpha * y_values(i);
    
    y_values(i+1) = y_values(i) + dt * y_dot;
    
    t_values(i+1) = t_values(i) + dt;
end

% Plotting scenario C
figure;
plot(t_values, y_values, 'b');
xlabel('Time');
ylabel('War-waging potential (y)');
title('Unilateral Disarmament: War-waging potential of Nation y over time');
