rev = IBM(:,1);
year = IBM(:,3);

rev = table2array(rev);
year = table2array(year);

r0 = 0.145;
Kr = 10^5;
x0 = 2;


R = @(t) ((Kr*x0*(exp(r0*t)))/(Kr+x0*((exp(r0*t))-1)));
expected = zeros(1,93);

semilogy(year,rev,".");

for i=1:1:93
    expected(i) = R(i);
end

hold on;
semilogy(year,expected,".");
xlim([0 90]);