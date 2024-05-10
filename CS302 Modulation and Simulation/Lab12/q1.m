% Load Data
price_data = load('price.dat');
fluct_data = load('fluct.dat');
gauss_data = load('gauss.dat');
wiene_data = load('wiene.dat');
trade_data = load('trade.dat');

% Plotting Figure 1
figure;
plot(price_data(:, 2), price_data(:, 3), 'b'); % assuming time vs. daily average price
xlabel('Time');
ylabel('Daily Average Price');
title('Figure 1: Daily Average Price of NIFTY Stocks');

% Plotting Figure 2
figure;
plot(fluct_data(:, 2), fluct_data(:, 3), 'r'); % assuming time vs. daily percentage fluctuation
xlabel('Time');
ylabel('Daily Percentage Fluctuation');
title('Figure 2: Daily Percentage Fluctuation of NIFTY Stocks');

% Repeat the above steps for Figures 3 to 6...
figure;
plot(gauss_data(:, 2), gauss_data(:, 3), 'r'); % assuming time vs. daily percentage fluctuation
xlabel('Time');
ylabel('Daily Percentage Fluctuation');
title('Figure 2: Daily Percentage Fluctuation of NIFTY Stocks');

figure;
plot(wiene_data(:, 2), wiene_data(:, 3), 'r'); % assuming time vs. daily percentage fluctuation
xlabel('Time');
ylabel('Daily Percentage Fluctuation');
title('Figure 2: Daily Percentage Fluctuation of NIFTY Stocks');

figure;
plot(trade_data(:, 2), trade_data(:, 3), 'r'); % assuming time vs. daily percentage fluctuation
xlabel('Time');
ylabel('Daily Percentage Fluctuation');
title('Figure 2: Daily Percentage Fluctuation of NIFTY Stocks');


% Analysis and Conclusion
% Based on the plots and any additional information provided in the paper, draw conclusions about the Bachelier-Wiener process modeling of NIFTY stock price variations.

