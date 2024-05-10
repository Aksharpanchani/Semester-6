deltat = 0.0001;
D = 1;
dx = sqrt(2*D*deltat);
deltax = 0.02;

x = -10:deltax:10;
t = 0.1:deltat:10;

xlen = length(x);
tlen = length(t);

phi = zeros(xlen,tlen);

mid = 10/deltax + 1;

phi(501,1) = 1000;

for tind = 1:tlen-1
    for xind = 2:xlen-1

        k = phi(xind+1,tind) + phi(xind-1,tind) - 2*phi(xind,tind);

        phi(xind,tind+1) = phi(xind,tind) + (D*deltat*k)/(deltax^2);

    end
end

p1 = phi(mid,:);
plot(t,p1,'k',LineWidth=1.5);

xlabel('t');
ylabel('\psi (0,t)');
title('\psi v/s t');