HR = IBM(:,2);
year = IBM(:,3);



HR = table2array(HR);
year = table2array(year);

a = 0.09;
k = 5*10^5;
x0 = 1300;


x = @(t) ((k*x0*(exp(a*t)))/(k+x0*((exp(a*t))-1)));
expected = zeros(1,93);

semilogy(year,HR,".");

for i=1:1:93
    expected(i) = x(i);
end

hold on;
semilogy(year,expected,"-");
xlim([0 90]);
xlabel('t in years');
ylabel('HR');
title('Modelling the Human resource growth of IBM');
legend('HR count','Modeled HR count','Location', 'best');