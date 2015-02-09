clear all
close all

% m är massa
m = 2;

% h är en steglängd
% k är fjäderkonstant
% d är dämpningskonstant
h = 0.01;
g = 9.81;
k = 20;
d = 0.4;

%Number of samples
N = 3000;

%Number of masses
M = 2;

%begynnelsevärde
x(1,1) = -1;
x(1,2) = 0;
v(1,1) = 0;
v(1,2) = 0;

for n=2:N
        %Varje fjäderkraft & dämpkraft beräknas F för massa 1 och 2
         F1 = k*((x(n-1,2)-x(n-1,1))-10)+d*(v(n-1,2)-v(n-1,1));
         F2 = k*((x(n-1,1)-x(n-1,2))+10)+d*(v(n-1,1)-v(n-1,2));
         
        %Euler
        v(n,1) = v(n-1,1) + F1*h/m;
        x(n,1) = x(n-1,1) + v(n,1)*h;
        v(n,2) = v(n-1,2) + F2*h/m;
        x(n,2) = x(n-1,2) + v(n,2)*h;
        
  end
figure(1)
plot(x);
title('Horizontal movement with 2 masses');
xlabel('Number of samples');
ylabel('Position');


