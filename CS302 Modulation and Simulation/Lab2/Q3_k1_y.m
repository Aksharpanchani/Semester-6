k = 0.6931;
I = 1;

c = @(t) (I/k)*(1-exp(-k*t));

t_max = 15;

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

plot(t,y);

limiting = I/k