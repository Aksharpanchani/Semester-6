% Read data from file

data = load('pareto.dat');

% Data points
x = [1e6, 1e7, 1e8, 1e9]; % Wealth levels
N = [330000, 21000, 1074, 120]; % Frequency distribution

% Initial guess for parameters
A = 60;
alpha = 5/4;

% Define the model function
N_model = @(x, A, B, alpha) A + B .* x.^-alpha;

% Define the objective function
obj_func = @(params) sum((N - N_model(x, params(1), params(2), params(3))).^2);

% Optimize the parameters
params = fminsearch(obj_func, [A, 1, alpha]);

% Generate the model curve
x_model = logspace(5, 10, 100); % Generate x values for the model curve
N_model_curve = N_model(x_model, params(1), params(2), params(3)); % Calculate N values using optimized parameters

% Plot the data and model
loglog(x, N, 'ro', 'MarkerSize', 8, 'LineWidth', 1.5);
hold on; % Plot data points
loglog(x_model, N_model_curve, 'b-', 'LineWidth', 1.5); % Plot model curve
xlabel('Wealth (USD)'); ylabel('Number of Individuals');
legend('Data', 'Model');
title('Pareto Distribution of Wealth in India');

% Display parameters
disp(['Optimized parameters: A = ', num2str(params(1)), ', B = ', num2str(params(2)), ', alpha = ', num2str(params(3))]);
