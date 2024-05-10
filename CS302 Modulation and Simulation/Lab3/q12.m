r = 1;
k = 1000;
x0 = 10;

h_values = [0, 100, 500];
colors = ['b', 'r', 'g'];

t_max = 1;  % Maximum time
dt = 0.0001;  % Time step

figure;
hold on;

for i = 1:length(h_values)
    h = h_values(i);
    t = 0:dt:t_max;
    x = zeros(1, length(t));
    x(1) = x0;

    % Euler's method loop
    for j = 1:length(t)-1
        x(j+1) = x(j) + (dt .* (r .* x(j) .* (1 - x(j)./k) - h));
    end

    plot(t, x);
end

xlabel('t');
ylabel('x');
title('Plot of x versus t for different h values using Euler''s method');
legend('h = 0', 'h = 100', 'h = 500');
grid on;
hold off;
