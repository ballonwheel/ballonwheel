clear
pkg load control

g=9.81
Ra=0.3
La=18e-3
k2 = km=2.518
kM = k2
k1 = ku=2.093
kU = ku

rw=0.27
mwheel = 6.6 #//mass of wheel [kg]
Town = 87.5 #///* ms */                                     /* time of speedup w/o load*/
mload = 1.08 #///* kg */                                    /* mass of load (tried to be a equally divided around the circle) */
Tload = 110 #///* ms */                                     /* time of speedup w/ load */
Iw = Town * ((mload * rw^2) / (Tload - Town))     # /* inertia of wheel */
Tm=(Iw*3*Ra)/(km*ku)
Te=(La)/(3*Ra)
rr = rv=2.3e-3
mb=0.203
rbf=30e-3
edge=30e-3
rb=sqrt(rbf^2-(edge/2)^2)
Ib=(2/5) * mb * rbf^2

% g = 9.8100
% Ra = 0.3000
% La = 0.018000
% k2 = 2.5180
% k1 = 2.0930
% rw = 0.2700
% mwheel = 6.6000
% Town = 87.500
% mload = 1.0800
% Tload = 110
% Iw = 0.3062
% Tm = 0.052287
% Te = 0.020000
% rr = 2.3000e-03
% mb = 0.2030
% rbf = 0.030000
% edge = 0.030000
% rb = 0.025981
% Ib = 7.3080e-05

% Common denominators
Den_w = Ib*Iw + mb*rb^2*Iw + mb*Ib*rw^2;
Den_z = (rw + rb)*(Ib*Iw + mb*rb^2*Iw + mb*Ib*rw^2);
 
alpha_w = (Ib*rw*mb*g) / Den_w;
beta_w = ((Ib + mb*rb^2)*(ku*kM + rv*Ra)) / (Ra*Den_w);
gamma_w = (k2*(Ib + mb*rb^2)) / (Ra*Den_w);
alpha_z = (Iw*rb^2 + Ib*rw^2) / Den_z;
beta_z = (Ib*rw*(ku*kM + Ra*rr)) / (Ra*Den_z);
gamma_z = (Ib*rw*kM) / (Ra*Den_z);
alpha_w
beta_w
gamma_w
alpha_z
beta_z
gamma_z
% alpha_w = 0.6007
% beta_w = 56.434
% gamma_w = 26.960
% alpha_z = 10.950
% beta_z = 17.906
% gamma_z = 8.5542

% Állapottér mátrixok
A = [ -beta_w   0        alpha_w;
      -beta_z   0        alpha_z;
       0        1        0 ];

B = [ gamma_w;
      gamma_z;
      0 ];

C = eye(3);   % ha minden állapotot ki akarunk vezetni
D = zeros(3,1);
#{
A =
  -56.4343         0    0.6007
  -17.9063         0   10.9501
         0    1.0000         0
B =
   26.9598
    8.5542
         0
C =
Diagonal Matrix
   1   0   0
   0   1   0
   0   0   1
D =
   0
   0
   0
#}

% Lineáris rendszer létrehozása
sys = ss(A,B,C,D);
#{
sys.a =
           x1      x2      x3
   x1  -56.43       0  0.6007
   x2  -17.91       0   10.95
   x3       0       1       0
sys.b =
          u1
   x1  26.96
   x2  8.554
   x3      0
sys.c =
       x1  x2  x3
   y1   1   0   0
   y2   0   1   0
   y3   0   0   1

sys.d =
       u1
   y1   0
   y2   0
   y3   0
Continuous-time model.
#}

polesA = eig(A)
#{
ans =
  -56.4377
   -3.2784
    3.2818
#}

% nekem csak fi2 kell
C = [0 0 1];
D = 0;
sys_phi = ss(A,B,C,D);
figure
bode(sys_phi)
grid on
title("Bode diagram - phi2 / Ua")
drawnow
pause(10)

nyquist(sys_phi)
grid on
drawnow
pause(30)

G = tf(sys_phi);
disp("Átviteli függvény:")
G
disp("Zérusok:")
z = zero(G) 
disp("Pólusok:")
p = pole(G) 

tol = 1e-9;

is_unstable = any(real(p) > tol);
is_nonmin   = any(real(z) > tol);

if is_nonmin
  disp("A rendszer NEM minimalfazisu (van RHP zerus).");
else
  disp("A rendszer minimalfazisu (nincs RHP zerus).");
end

if is_unstable
  disp("A rendszer instabil (van RHP polus).");
else
  disp("A rendszer stabil.");
end


