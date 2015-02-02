% Hook's lag
% F = -kx

% Newtons and och Newtons tredje

% En extra modell för dämpning

% m är massa
m = 1;

% h är en steglängd
h = 0.01;
g = 9.81;
k= 100;
d= 0;

%Number of samples
N = 1000;

%Number of masses
M = 2;

%begynnelsevärden, släpps från vila
v = zeros(M,1);
v(2) = 3;
x = 0;


x=zeros(N,M);

x(1,1) = -5;
x(1,2) = 5;
%x(1,3) = 15;
%x(1,4) = 25;

for n=2:N
    %Varje fjäderkraft
    F = k*((x(n-1,1)-x(n-1,2))+10);
    for i=1:M
        
        %F(2) = -d*v(i);
        % F är en vektor med alla krafter
        if mod(i,2)==1
            a = -1/m*(sum(F));
        else
            a = 1/m*sum(F);
        end
        v(i) = v(i) + a*h;
        x(n,i) = x(n-1,i) + v(i)*h;
    end
end

plot(x);