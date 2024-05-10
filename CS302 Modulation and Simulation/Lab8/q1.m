% Given parameters
a = 0.0106;
j = 0.0544;
J0 = 18274;
A0 = 66454;
delta_t = 1; % Time step in days

% Initial conditions
J = J0;
A = A0;
t = 0;

% Lists to store values for plotting
time_list = [];
J_list = [];
A_list = [];

% Euler's method integration
while (J >= 1 && A >= 1)
    % Store current values
    time_list(end+1) = t;
    J_list(end+1) = J;
    A_list(end+1) = A;
    
    % Update equations
    J_dot = -a * A;
    A_dot = -j * J;
    
    J = J + J_dot * delta_t;
    A = A + A_dot * delta_t;
    
    % Increment time
    t = t + delta_t;
end

% Outcome of the battle
if J < 1
    disp('Japanese army defeated.');
    victorious_army = 'American';
    casualties = J0 - J;
    active_troops = A;
elseif A < 1
    disp('American army defeated.');
    victorious_army = 'Japanese';
    casualties = A0 - A;
    active_troops = J;
end

disp(['Victorious army: ' victorious_army]);
disp(['Number of active troops in victorious army: ' num2str(active_troops)]);
disp(['Number of casualties in defeated army: ' num2str(casualties)]);
disp(['Number of days for outcome: ' num2str(t)]);

% Plotting
figure;

% Plot of J and A along with time
%subplot(2,1,1);
plot(time_list, J_list, 'r', 'LineWidth', 2);
hold on;
plot(time_list, A_list, 'b', 'LineWidth', 2);
xlabel('Time (days)');
ylabel('Number of troops');
legend('Japanese', 'American');
title('Number of troops vs Time');
figure;
% Plot with logarithmic vertical axis
%subplot(2,1,2);
semilogy(time_list, J_list, 'r', 'LineWidth', 2);
hold on;
semilogy(time_list, A_list, 'b', 'LineWidth', 2);
xlabel('Time (days)');
ylabel('Number of troops (log scale)');
legend('Japanese', 'American');
xlim([0 30.36]);
title('Number of troops vs Time (log scale)');

% Plot A vs J
figure;
plot(J_list, A_list, 'k', 'LineWidth', 2);
xlabel('Number of Japanese troops');
ylabel('Number of American troops');
title('Number of American troops vs Number of Japanese troops');
