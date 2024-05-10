r = 1;
k = 1000;

h_values = [0, 100, 500];
colors = ['b', 'r', 'g'];

x = linspace(0, 2000, 1000);  % Range of x values

figure;
hold on;
    
for i = 1:length(h_values)
    h = h_values(i);
    xdot = r.*x.*(1 - x/k) - h;
    plot(x, xdot);
end

xlabel('x');
ylabel('dx');
title(' dx vs x');
legend('h = 0', 'h = 100', 'h = 500');
grid on;
hold off;
