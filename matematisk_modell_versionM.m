% Hook's lag
% F = -kx

% Newtons and och Newtons tredje

% En extra modell f�r d�mpning

% m �r massa
m = 1;

% h �r en stegl�ngd
h = 0.01;
g = 9.81;
k = 20;
d = 0.4;

%Number of samples
N = 1000;

%Number of masses
M = 2;

%begynnelsev�rden
v = zeros(N,M);
v(2,1) = 3;

%F = zeros(M,1);
x = zeros(N,M);

x(1,1) = -5;
x(1,2) = 5;
%x(1,3) = 15;
%x(1,4) = 25;

for n=2:N
    %Varje fj�derkraft & d�mpkraft ber�knas F f�r massa 1 och 2
    F = k*((x(n-1,1)-x(n-1,2))+10)+d*(v(1)-v(2));
   % F(2) = k*((x(n-1,2)-x(n-1,1))+10)+d*(v(n-1,2)-v(n-1,1));
    for i=1:M

%       F �r en vektor med alla krafter
%       kontrollerar vilket h�ll vilkterna �ker �t
       if mod(i,2)==1
            a = -1/m*F;
        else
            a = 1/m*F;
        end
        v(i) = v(i) + a*h;
        x(n,i) = x(n-1,i) + v(i)*h;
        
%         �r inte det h�r r�tt eulerform?
%         v(n,i) = v(n-1,i) + a*h;
%         x(n,i) = x(n-1,i) + v(n,i)*h;
    end
    
end

plot(x);