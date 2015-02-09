% generalised expression in 1D
% three masses

clear all
close all

% m �r massa
m = 2;

% h �r en stegl�ngd
% k �r fj�derkonstant
% d �r d�mpningskonstant
h = 0.01; % steglängd
g = 9.81; % gravitation
k = 20; % fjäderkonstant
b = 0.4; % dämpningskonstant
l = 10; % längden från jämviktsläget

%Number of samples
N = 3000;

%Number of masses
M = 8;

x = zeros(N,M); % allokera minne
%begynnelsev�rde
x(1,1) = -1;
x(1,2) = 0;
x(1,3) = 0;
v = zeros(N,M); % allkoera minne
Fk = zeros(M,1);
Fb = zeros(M,1);

for n=2:N
    for i = 1:M
        if i < 2
           Fk(i,1) = k*((x(n-1,i+1)-x(n-1,i))-l);
           Fb(i,1) = b*(v(n-1,i+1)-v(n-1,i));
        elseif i > M-1
           Fk(i,1) = k*((x(n-1,i-1)-x(n-1,i))+l);
           Fb(i,1) = b*(v(n-1,i-1)-v(n-1,i));
        else
            Fk(i,1) = k*(x(n-1,i-1)-x(n-1,i)) + k*(x(n-1,i+1)-x(n-1,i));
            Fb(i,1) = b*(v(n-1,i-1)-v(n-1,i)) + b*(v(n-1,i+1)-v(n-1,i));
        end
         
        %Euler
        v(n,i) = v(n-1,i) + (Fk(i,1)+Fb(i,1))*h/m;
        x(n,i) = x(n-1,i) + v(n,i)*h;
    end
  end
figure(1)
plot(x);


