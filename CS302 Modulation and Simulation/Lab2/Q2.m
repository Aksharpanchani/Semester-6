k1 = 0.6931;
k2 = 0.0231;
x0 = 1;

c = @(t) ((x0)*exp(-k1*t));

t_max = 100;

t = 0:1:t_max;

x = zeros(1,t_max+1);
y = zeros(1,t_max+1);

c2 = @(t) ((k1*x0)/(k2-k1))*(exp(-k1*t) - exp(-k2*t));

for i=1:1:t_max+1
    x(i) = c(i-1);
end

for i=1:1:t_max+1
    y(i) = c2(i-1);
end

plot(t,x);

hold on;

plot(t,y);

tformax = (log(k1/k2))/(k1-k2)
ymax = c2(tformax)