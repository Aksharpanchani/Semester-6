t = readtable('gauss.dat');

plot(t{:,2}, t{:,3},'k',LineWidth=0.5);
sigma = 1.495;
mu = 0.057;
f0 = 18;
del = -13:0.01:18;
%legend('Price of Nifty', 'Analytical value of Nifty', 'Location', 'best');
f = 1 + f0.* exp( (-(del - mu).^2)./(2*sigma*sigma) );
hold on;
plot(del, f,'r', 'LineWidth', 1);
legend('Fluctuations', 'Gaussian Fit', 'Location', 'best');
xlabel('\delta (Fluctuation)');
ylabel('f(\delta)');

hold off;