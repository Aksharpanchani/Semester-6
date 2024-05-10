% Define the phi function based on Equation 2
phi_equation_2 = @(x, eta, lambda, mu, alpha) (eta + (x + lambda).^(-mu) .* alpha).^(-1/mu);

% Define the phi function based on Equation 5
phi_equation_5 = @(x, eta, lambda, c) eta + (c ./ x + lambda).^2;

% Load data and plot for each release
releases = {'Etch', 'Lenny', 'Squeeze'};
for i = 1:length(releases)
    release = releases{i};
    
    % Load data files for incoming and outgoing dependency links
    incoming_data = load(['incoming_dependency_', release, '.dat']);
    outgoing_data = load(['outgoing_dependency_', release, '.dat']);
    
    % Extract data
    incoming_links = incoming_data(:, 1);
    incoming_freq = incoming_data(:, 2);
    outgoing_links = outgoing_data(:, 1);
    outgoing_freq = outgoing_data(:, 2);
    
    % Plot for incoming dependency links
    figure;
    plot(incoming_links, incoming_freq, 'ro', 'MarkerSize', 8, 'LineWidth', 1.5);
    hold on;
    x_values = linspace(min(incoming_links), max(incoming_links), 1000);
    y_values_equation_2 = phi_equation_2(x_values, 0.005, 0.2, 2, 0.5); % Parameters are adjusted based on the paper
    plot(x_values, y_values_equation_2, 'b-', 'LineWidth', 1.5);
    xlabel('Number of Links');
    ylabel('Frequency');
    title(['Incoming Dependency Links Distribution - ', release]);
    legend('Data', 'Model (Equation 2)');
    grid on;
    
    % Plot for outgoing dependency links
    figure;
    plot(outgoing_links, outgoing_freq, 'ro', 'MarkerSize', 8, 'LineWidth', 1.5);
    hold on;
    y_values_equation_5 = phi_equation_5(x_values, 0.005, 0.2, 0.01); % Parameters are adjusted based on the paper
    plot(x_values, y_values_equation_5, 'b-', 'LineWidth', 1.5);
    xlabel('Number of Links');
    ylabel('Frequency');
    title(['Outgoing Dependency Links Distribution - ', release]);
    legend('Data', 'Model (Equation 5)');
    grid on;
end
