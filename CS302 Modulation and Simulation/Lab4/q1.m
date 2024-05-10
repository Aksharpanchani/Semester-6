
gompertz = @(X) -X.*log(X);


X = linspace(0.01, 5, 100);
for i = 1: 3 
    X_dot = gompertz(X);
    plot(X, X_dot, 'LineWidth', 1);
end

xlabel('X');
ylabel('X''');
title('Plot of X'' versus X for the Gompertz Equation');
grid on;


legend('Initial value 1', 'Initial value 2', 'Initial value 3');
