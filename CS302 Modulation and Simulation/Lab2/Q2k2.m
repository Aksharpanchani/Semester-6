clc;
clear all;

k1 = 0.0231;
k2 = 0.0231;
x0 = 1;

c = @(t) ((x0)*exp(-k1*t));

t_max = 500;

t = 0:1:t_max;

x = zeros(1,t_max+1);
y = zeros(1,t_max+1);

c2 = @(t) k1*x0*t*exp(-k1*t);

for i=1:1:t_max+1
    x(i) = c(i-1);
end

for i=1:1:t_max+1
    y(i) = c2(i-1);
end

plot(t,x);

hold on;

plot(t,y);
xlabel('Time (hours)');
ylabel('Concentration');
title('Concentration of Medicine in GI Tract and Bloodstream');
legend('x(t) in GI Tract', 'y(t) in Bloodstream');

tformax = 1/k1
ymax = c2(tformax)