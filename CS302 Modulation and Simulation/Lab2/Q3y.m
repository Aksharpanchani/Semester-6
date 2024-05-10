k1 = 0.6931;
k2 = 0.0231;
I = 1;

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

plot(t,y);

limiting = I/k2