% Parameters
A = 2.18e-3; % Infection rate (per day)
B = 0.44;   % Removal rate (per day)
N = 763;    % Total population size
x0 = 1;     % Initial infected individuals
y0 = N - x0; % Initial susceptible individuals
t_end = 50; % End time (days)
dt = 0.1;   % Time step (days)

% Initialize variables
t = 0:dt:t_end;
x = zeros(size(t));
y = zeros(size(t));
z = zeros(size(t));
x(1) = x0;
y(1) = y0;
z(1) = 0;

% Euler's method
for i = 1:length(t)-1
    dx = A * x(i) * y(i) - B * x(i);
    dy = -A * x(i) * y(i);
    dz = B * x(i);
    
    x(i+1) = x(i) + dx * dt;
    y(i+1) = y(i) + dy * dt;
    z(i+1) = z(i) + dz * dt;
end

% Plotting
figure;
%subplot(2,1,1);
plot(t, x, 'r', t, y, 'b');
xlabel('Time (days)');
ylabel('Number of Individuals');
legend('Infected (x)', 'Susceptible (y)');
title('Epidemic Dynamics');
xlim([0 25]);

figure;
%subplot(2,1,2);
semilogy(t, x, 'r', t, y, 'b');
xlabel('Time (days)');
ylabel('Number of Individuals (log scale)');
legend('Infected (x)', 'Susceptible (y)');
xlim([0 25]);
% Finding the time when x reaches its maximum value
[max_x, idx] = max(x);
time_max_x = t(idx);
disp(['Time when x reaches maximum value: ', num2str(time_max_x), ' days']);
