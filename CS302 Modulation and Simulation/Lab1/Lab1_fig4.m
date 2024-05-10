rev = IBM(:,1);
HR = IBM(:,2);

HR = table2array(HR);
rev = table2array(rev);


Krinv= 1/100000;
Khinv = 1/ (5*10^5);

V = 1./rev - Krinv;
U = 1./HR - Khinv;

beta= 1.5;
x=U.^beta;
x=x*20000;


loglog(U,V,".")

hold on;
loglog(U,x);
xlim([0 0.001]);
xlabel('U');
ylabel('V');
title('Coorelated growth of H and R');
legend('Actual','Estimated','Location', 'best');