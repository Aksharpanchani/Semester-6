clc;
clear all;
% Define parameters
k = 0.6931;

I = 1;

% Define time range
t = 0:0.5:35;  % Time in hours, with a step of 0.1 hour

% Solve the differential equations using analytical solutions
x = I ./ k * (1 - exp(-k*t));
x_lim = I / k;  % x approaches I/k1 as t approaches infinity
y_lim = I * k / (k - k);  % y approaches I*k1/(k1-k2) as t approaches infinity

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

c = @(t) (I/k)*(1-exp(-k*t));

t_max = 35;

t = 0:1:t_max;

x = zeros(1,t_max+1);
y = zeros(1,t_max+1);

c2 = @(t) ( (I/k)*(1-((k*t) + 1)*exp(-k*t)) );

for i=1:1:t_max+1
    x(i) = c(i-1);
end

for i=1:1:t_max+1
    y(i) = c2(i-1);
end
figure;
plot(t,y);

xlabel('Time (hours)');
ylabel('y(t)');
title('Concentration of Medicine in Bloodstream');

hold on;  

legend('y(t)', 'Limiting Value');
grid on;

% Display limiting values
fprintf('Limiting value of x(t): %.4f\n', 1/k);
fprintf('Limiting value of y(t): %.4f\n', 1/k);
