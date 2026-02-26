
clear
pkg load control


alpha_w = 0.6007
beta_w = 56.434
gamma_w = 26.960
alpha_z = 10.950
beta_z = 17.906
gamma_z = 8.5542

% Állapottér mátrixok
A = [ -beta_w   0        alpha_w;
      -beta_z   0        alpha_z;
       0        1        0 ];

B = [ gamma_w;
      gamma_z;
      0 ];

C = [0 0 1];
D = 0;
sys_phi = ss(A,B,C,D);

s = tf("s");

% --- PLANT (a te kiirt tf-ed alapjan) ---
G = (8.554*s) / (s^3 + 56.43*s^2 - 10.95*s - 607.2);

% --- CEL CUTOFF (tapadasbarat elso probanak) ---
wc = 7;

wz = 1.5;
wp = 30;
Lead = (1 + s/wz) / (1 + s/wp);

Gj = freqresp(G, wc);
Lj = freqresp(Lead, wc);
K  = 1 / abs(Gj * Lj);
C  = -K * Lead;


L = minreal(C*G);
T = minreal(feedback(L, 1));   % zart hurku: y/r
S = minreal(1/(1+L));          % erzekenyseg

printf("\n--- DESIGN SUMMARY ---\n");
printf("wc target      = %.3f rad/s\n", wc);
printf("lead wz/wp     = %.3f / %.3f rad/s\n", wz, wp);
printf("computed K     = %.3f\n", K);

% --- Polusok / zerusok ellenorzes (minimalfazis, stabilitas) ---
pG = pole(G);
zG = zero(G);

tol = 1e-9;
is_unstable = any(real(pG) > tol);
is_nonmin   = any(real(zG) > tol);

printf("\nPlant poles:\n"); disp(pG);
printf("Plant zeros:\n"); disp(zG);

if is_nonmin
  disp("Plant: NON-minimum-phase (RHP zero).");
else
  disp("Plant: minimum-phase (no RHP zero).");
end

if is_unstable
  disp("Plant: UNSTABLE (has RHP pole).");
else
  disp("Plant: stable.");
end

% --- Zart hurku polusok ---
printf("\nClosed-loop poles (T):\n");
disp(pole(T));

% --- Marginok (ha van ertelmezheto atlepes) ---
printf("\nMargins (open-loop L=C*G):\n");
try
  [gm, pm, wgc, wpc] = margin(L);
  printf("GM = %.3f (%.2f dB) at wpc=%.3f rad/s\n", gm, 20*log10(gm), wpc);
  printf("PM = %.2f deg at wgc=%.3f rad/s\n", pm, wgc);
catch
  disp("margin() failed (no crossover etc).");
end

% --- Plotok ---
figure(1); margin(L); grid on; title("Open-loop L(s)=C(s)G(s) margin");
figure(2); nyquist(L); grid on; title("Nyquist of L(s)=C(s)G(s)");
figure(3); step(T); grid on; title("Closed-loop step: phi2 / phi2_ref");

% --- Opcionális: referencia szuro tapadasra (lassabb parancs) ---
% 1. rendu low-pass a referenciara, hogy ne kerjen nagy gyorsitast
Tr = 0.5;                % s (0.3-1.0s tipikusan)
Fref = 1/(Tr*s + 1);

Tref = minreal(T*Fref);  % y/r, ahol r mar szurve
figure(4); step(Tref); grid on; title("Closed-loop with reference filter Fref");

printf("L(jwc) magnitude = %.3f, phase = %.2f deg\n", abs(freqresp(L,wc)), angle(freqresp(L,wc))*180/pi);

