rev = IBM(:,1);
year = IBM(:,3);

rev = table2array(rev);
year = table2array(year);

a = 0.145;
k = 10^5;
x0 = 2;

x = @(t) ((k*x0*(exp(a*t)))/(k+x0*((exp(a*t))-1)));
expected = zeros(1,93);

semilogy(year,rev,".");

for i=1:1:93
    expected(i) = x(i);
end

hold on;
semilogy(year,expected,"-");
xlim([0 90]);
xlabel('t in years');
ylabel('R (Millions US Dollars)');
title('Modelling the Annual Growth of IBM');
legend('Actual Revenue','Estimated Revenue');
