% Hook's lag
% F = -kx

% Newtons and och Newtons tredje

% En extra modell för dämpning

% m är massa
m = 1;

% h är en steglängd
h = 0.01;
g = 9.81;
k = 20;
d = 0.4;

%Number of samples
N = 1000;

%Number of masses
M = 2;

%begynnelsevärden
v = zeros(N,M);
v(2,1) = 3;

%F = zeros(M,1);
x = zeros(N,M);

x(1,1) = -5;
x(1,2) = 5;
%x(1,3) = 15;
%x(1,4) = 25;

for n=2:N
    %Varje fjäderkraft & dämpkraft beräknas F för massa 1 och 2
    F = k*((x(n-1,1)-x(n-1,2))+10)+d*(v(1)-v(2));
   % F(2) = k*((x(n-1,2)-x(n-1,1))+10)+d*(v(n-1,2)-v(n-1,1));
    for i=1:M

%       F är en vektor med alla krafter
%       kontrollerar vilket håll vilkterna åker åt
       if mod(i,2)==1
            a = -1/m*F;
        else
            a = 1/m*F;
        end
        v(i) = v(i) + a*h;
        x(n,i) = x(n-1,i) + v(i)*h;
        
%         är inte det här rätt eulerform?
%         v(n,i) = v(n-1,i) + a*h;
%         x(n,i) = x(n-1,i) + v(n,i)*h;
    end
    
end

plot(x);