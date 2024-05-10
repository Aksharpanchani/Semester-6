clc;
clear all;

F = 5*10^8;
V = 10^12;
Cin = 0;
C0 = 10;

a = (F*Cin)/V;
b = F/V;

c = @(t) (Cin - ((Cin-C0)*exp((-1*F*t)/(V))));

tmax = 20000;

t=0:1:tmax;

con = zeros(1,tmax+1);

for i=1:1:tmax+1
    con(i) = c(i-1);
end

plot(t,con,"R",LineWidth=1);
xlabel('t');
ylabel('C(t)');
title('Concentration of pollutants in lake');

C_find = 0.5*C0;
t_half = (V/F)*log((Cin-C0)/(Cin-C_find));
