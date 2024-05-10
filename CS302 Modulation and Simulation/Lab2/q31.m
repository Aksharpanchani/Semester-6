clc;
clear all;
% Define parameters
k1 = 0.6931;
k2 = 0.0231;
I = 1;

% Define time range
t = 0:0.5:35;  % Time in hours, with a step of 0.1 hour

% Solve the differential equations using analytical solutions
x = I ./ k1 * (1 - exp(-k1*t));
x_lim = I / k1;  % x approaches I/k1 as t approaches infinity
y_lim = I * k1 / (k1 - k2);  % y approaches I*k1/(k1-k2) as t approaches infinity

% Plot x(t)
figure;
plot(t, x, 'r');
xlabel('Time (hours)');
ylabel('x(t)');
title('Concentration of Medicine in GI Tract');
ylim([0 x_lim + 0.1]);  % adjust y-axis limits slightly to show limiting value
hold on;  % to overlay horizontal line for limiting value
plot([0 10], [x_lim x_lim], 'k--', 'LineWidth', 0.5);  % plot horizontal line for limiting value
legend('x(t)', 'Limiting Value');
grid on;

% Plot y(t)

c = @(t) (I/k1)*(1-exp(-k1*t));

t_max = 300;

t = 0:1:t_max;

x = zeros(1,t_max+1);
y = zeros(1,t_max+1);

c2 = @(t) ( (I/k2)*(1-exp(-k2*t)) - (I/(k2-k1))*( exp(-k1*t) - exp(-k2*t)) );

for i=1:1:t_max+1
    x(i) = c(i-1);
end

for i=1:1:t_max+1
    y(i) = c2(i-1);
end
figure;
plot(t,y);
ylim([0 45]);
xlim([0 300]);

xlabel('Time (hours)');
ylabel('y(t)');
title('Concentration of Medicine in Bloodstream');

hold on;  

legend('y(t)', 'Limiting Value');
grid on;

% Display limiting values
fprintf('Limiting value of x(t): %.4f\n', 1/k1);
fprintf('Limiting value of y(t): %.4f\n', 1/k2);
