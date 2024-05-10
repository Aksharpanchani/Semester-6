a = 1;
b = 2;
r = 1;
x01 = x_fixed - 0.01;  % Slightly below fixed point
x02 = x_fixed + 0.01;
  % Parameter-free function
  f = @(x) x * (r - a * (x - b)^2);

  % Fixed point
  x_fixed = sqrt(r/a) + b;

  % Time range
  t = linspace(0, 10, 1000);

  % Euler's method
  for i = 2:length(t)
    X1(i) = X1(i-1) + dt * f(X1(i-1));
    X2(i) = X2(i-1) + dt * f(X2(i-1));
  end

  % Plot
  plot(t, X1);
  plot(t, X2);
  plot([0 max(t)], [x_fixed x_fixed], 'LineWidth', 2, 'Color', 'gray', 'LineStyle', '--');

  % Labels and legend
  xlabel('t');
  ylabel('x');
  title('Allee Effect: Numerical Solutions with Euler''s Method');
  legend();
  grid on;
  show();



