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
M = 3;

x = zeros(N,M); % allokera minne
%begynnelsev�rde
x(1,1) = -1;
x(1,2) = 0;
x(1,3) = 0;
v = zeros(N,M); % allkoera minne

for n=2:N
        %Varje fj�derkraft & d�mpkraft ber�knas F f�r massa 1 och 2
         Fk1 = k*((x(n-1,2)-x(n-1,1))-l);
         Fb1 = b*(v(n-1,2)-v(n-1,1));
         
         Fk2 = k*((x(n-1,1)-x(n-1,2))) + k*((x(n-1,3)-x(n-1,2)));
         Fb2 = b*(v(n-1,1)-v(n-1,2)) + b*(v(n-1,3)-v(n-1, 2));
         
         Fk3 = k*((x(n-1,2)-x(n-1,3))+l);
         Fb3 = b*(v(n-1,2)-v(n-1,3));
         
        %Euler
        v(n,1) = v(n-1,1) + (Fk1+Fb1)*h/m;
        x(n,1) = x(n-1,1) + v(n,1)*h;
        v(n,2) = v(n-1,2) + (Fk2+Fb2)*h/m;
        x(n,2) = x(n-1,2) + v(n,2)*h;
        v(n,3) = v(n-1,3) + (Fk3 + Fb3)*h/m;
        x(n,3) = x(n-1,3) + v(n,3)*h;
        
  end
figure(1)
plot(x);


