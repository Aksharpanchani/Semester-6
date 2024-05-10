% C. Plot z along the vertical axis (with and without a logarithmic scale)
figure;

plot(t, z, 'g');
xlabel('Time (days)');
ylabel('Number of Individuals');
title('Recovered (z) Dynamics');
xlim([0 18]);
figure;
semilogy(t, z, 'g');
xlabel('Time (days)');
ylabel('Number of Individuals (log scale)');
xlim([0 25]);
% D. Plot x along the vertical axis and y along the horizontal axis

figure;
plot(y, x, 'k');
xlabel('Susceptible (y)');
ylabel('Infected (x)');
title('Phase Portrait: Infected vs Susceptible');

% Compare with threshold values
rho = B / A; % Threshold value
R = A * y0 / B; % Reproduction number

disp(['Threshold value (rho): ', num2str(rho)]);
disp(['Reproduction number (R): ', num2str(R)]);

if R > 1 || y0 > rho
    disp('An epidemic breaks out.');
else
    disp('No epidemic occurs.');
end
