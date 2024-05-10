year = Prof(:,1);
profit = Prof(:,2);

year = table2array(year);
profit = table2array(profit);

plot(year,profit);
xlim([0 90])
xlabel('t in years');
ylabel('P (Millions US Dollars');
title('The net annual earnings of IBM');
legend('Profit of IBM','Location', 'best');