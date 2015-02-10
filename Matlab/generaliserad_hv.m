% generalized expression in 2D
% three masses

clear all
close all

m = 2; % mass
h = 0.01; % step
g = 9.81; % gravitation
k = 20; % feather constant
b = 0.4; % damping constant
l = 10; % distance from equlibrium point

% Number of samples
N = 3000;

% Number of masses
Mx = 8;
My = 2;
Mtot = Mx + My;
 
x = zeros(N,Mtot,2); % allocate memory for position
v = zeros(N,Mtot,2); % allocate memory for velocity
% inital values
x(1,1,1) = -1;

% allocate memory for forces
Fk = zeros(Mtot,2);
Fb = zeros(Mtot,2);

% right is positive, up is positive
for n=2:N
    for hor = 1:Mx
        for ver = Mx+1:Mx+My
            % for all the horizontal masses
            % adding the forces from all directions acting on the mass
            if hor == 1
               Fk(hor,1) = k*((x(n-1,hor+1,1)-x(n-1,hor,1))-l);
               Fb(hor,1) = b*(v(n-1,hor+1,1)-v(n-1,hor,1));
               if ver == Mx+1
                   Fk(hor,2) = k*((x(n-1,ver+1,2) - x(n-1,ver,2))+l);
                   Fb(hor,2) = b*(v(n-1,ver+1,2) - v(n-1,ver,2));
               elseif ver == Mx+My
                   Fk(hor,2) = k*((x(n-1,ver-1,2)-x(n-1,ver,2))-l);
                   Fb(hor,2) = b*(v(n-1,ver-1,2)-v(n-1,ver,2));
               else
                   Fk(hor,2) = k*(x(n-1,ver+1,2)-x(n-1,ver,2)) + k*(x(n-1,ver-1,2) - x(n-1, ver,2));
                   Fb(hor,2) = b*(v(n-1,ver+1,2)-v(n-1,ver,2)) + b*(v(n-1,ver-1,2) - v(n-1, ver,2));
               end   
            elseif hor == Mx
               Fk(hor,1) = k*((x(n-1,hor-1,1)-x(n-1,hor,1))+l);
               Fb(hor,1) = b*(v(n-1,hor-1,1)-v(n-1,hor,1));
               if ver == Mx+1
                   Fk(hor,2) = k*((x(n-1,ver+1,2) - x(n-1,ver,2))+l);
                   Fb(hor,2) = b*(v(n-1,ver+1,2) - v(n-1,ver,2));
               elseif ver == Mx+My
                   Fk(hor,2) = k*((x(n-1,ver-1,2)-x(n-1,ver,2))-l);
                   Fb(hor,2) = b*(v(n-1,ver-1,2)-v(n-1,ver,2));
               else
                   Fk(hor,2) = k*(x(n-1,ver+1,2)-x(n-1,ver,2)) + k*(x(n-1,ver-1,2)-x(n-1,ver,2));
                   Fb(hor,2) = b*(v(n-1,ver+1,2)-v(n-1,ver,2)) + b*(v(n-1,ver-1,2)-v(n-1,ver,2));
               end   
            else
                Fk(hor,1) = k*(x(n-1,hor-1,1)-x(n-1,hor,1)) + k*(x(n-1,hor+1,1)-x(n-1,hor,1));
                Fb(hor,1) = b*(v(n-1,hor-1,1)-v(n-1,hor,1)) + b*(v(n-1,hor+1,1)-v(n-1,hor,1));
                if ver == Mx+1
                   Fk(hor,2) = k*((x(n-1, ver+1,2) - x(n-1, ver,2))+l);
                   Fb(hor,2) = b*(v(n-1, ver+1,2) - v(n-1, ver,2));
                elseif ver == Mx+My
                   Fk(hor,2) = k*((x(n-1,ver-1,2)-x(n-1,ver,2))-l);
                   Fb(hor,2) = b*(v(n-1,ver-1,2)-v(n-1,ver,2));
                else
                   Fk(hor,2) = k*(x(n-1,ver+1,2)-x(n-1,ver,2)) + k*(x(n-1,ver-1,2)-x(n-1, ver,2));
                   Fb(hor,2) = b*(v(n-1,ver+1,2)-v(n-1,ver,2)) + b*(v(n-1,ver-1,2) - v(n-1, ver,2));
               end  
            end
            % for all the vertical masses
            % adding the forces from all directions acting on the mass
            if ver == Mx+1
                Fk(ver,2) = k*((x(n-1,ver+1,2)-x(n-1,ver,2))+l);
                Fb(ver,2) = b*(v(n-1,ver+1,2)-v(n-1,ver,2));
                if hor == 1
                    Fk(ver,1) = k*((x(n-1,hor+1,1)-x(n-1,hor,1))-l);
                    Fb(ver,1) = b*(v(n-1, hor+1,1) - v(n-1,hor,1));
                elseif hor == Mx
                    Fk(ver,1) = k*((x(n-1,hor-1,1)-x(n-1,hor,1))+l);
                    Fb(ver,1) = b*(v(n-1,hor-1,1) - v(n-1,hor,1));
                else
                    Fk(ver,1) = k*(x(n-1,hor+1,1)-x(n-1,hor,1)) + k*(x(n-1,hor-1,1)-x(n-1,hor,1));
                    Fb(ver,1) = b*(v(n-1,hor+1,1)-v(n-1,hor,1)) + b*(v(n-1,hor-1,1)-v(n-1,hor,1));
                end
            elseif ver == My
                Fk(ver,2) = k*((x(n-1,ver-1,2)-x(n-1,ver,2))-l);
                Fb(ver,2) = b*(v(n-1,ver-1,2)-v(n-1,ver,2));
                if hor == 1
                    Fk(ver,1) = k*((x(n-1,hor+1,1)-x(n-1,hor,1))-l);
                    Fb(ver,1) = b*(v(n-1, hor+1,1) - v(n-1,hor,1));
                elseif hor == Mx
                    Fk(ver,1) = k*((x(n-1,hor-1,1)-x(n-1,hor,1))+l);
                    Fb(ver,1) = b*(v(n-1,hor-1,1) - v(n-1,hor,1));
                else
                    Fk(ver,1) = k*(x(n-1,hor+1,1)-x(n-1,hor,1)) + k*(x(n-1,hor-1,1)-x(n-1,hor,1));
                    Fb(ver,1) = b*(v(n-1,hor+1,1)-v(n-1,hor,1)) + b*(v(n-1,hor-1,1)-v(n-1,hor,1));
                end
            else
                Fk(ver,2) = k*(x(n-1,ver+1)-x(n-1,ver)) + k*(x(n-1,ver-1)-x(n-1,ver));
                Fb(ver,2) = b*(v(n-1,ver+1)-v(n-1,ver)) + b*(v(n-1,ver-1)-v(n-1,ver));
                if hor == 1
                    Fk(ver,1) = k*((x(n-1,hor+1,1)-x(n-1,hor,1))-l);
                    Fb(ver,1) = b*(v(n-1, hor+1,1) - v(n-1,hor,1));
                elseif hor == Mx
                    Fk(ver,1) = k*((x(n-1,hor-1,1)-x(n-1,hor,1))+l);
                    Fb(ver,1) = b*(v(n-1,hor-1,1) - v(n-1,hor,1));
                else
                    Fk(ver,1) = k*(x(n-1,hor+1,1)-x(n-1,hor,1)) + k*(x(n-1,hor-1,1)-x(n-1,hor,1));
                    Fb(ver,1) = b*(v(n-1,hor+1,1)-v(n-1,hor,1)) + b*(v(n-1,hor-1,1)-v(n-1,hor,1));               
                end  
            end
        end
    end
    for i = 1:Mtot
        % Euler explicit
        v(n,i,1) = v(n-1,i,1) + (Fk(i,1)+Fb(i,1))*h/m;
        v(n,i,2) = v(n-1,i,2) + (Fk(i,2)+Fb(i,2)-m*g)*h/m;
        x(n,i,1) = x(n-1,i,1) + v(n,i,1)*h;
        x(n,i,2) = x(n-1,i,2) + v(n,i,2)*h;
    end
end
  
figure(1)
plot(x(:,:,1));
figure(2)
plot(x(:,:,2));
