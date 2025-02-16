pkg load parallel;

function dydt = pmsm_dynamics(t, y, Vd, Vq, Ld, Lq, Rs, wm, lambda_m)
  id = y(1);
  iq = y(2);
  
  did_dt = (Vd - Rs*id + wm*Lq*iq) / Ld;
  diq_dt = (Vq - Rs*iq - wm*(Ld*id + lambda_m)) / Lq;
  
  dydt = [did_dt; diq_dt];
endfunction

function V = input_voltage(t)
  Vd = 100 * sin(2*pi*50*t); 
  Vq = 100 * cos(2*pi*50*t);
  V = [Vd, Vq];
endfunction

function simulate_pmsm()
  Ld = 0.005;
  Lq = 0.005;
  Rs = 0.1;
  wm = 314; 
  lambda_m = 0.02;
  
  tspan = [0 0.1];
  y0 = [0; 0];
  
  ts = linspace(tspan(1), tspan(2), 200);
  ids = zeros(size(ts));
  iqs = zeros(size(ts));
  
  for i = 1:length(ts)
    V = input_voltage(ts(i));
    [t, y] = ode45(@(t, y) pmsm_dynamics(t, y, V(1), V(2), Ld, Lq, Rs, wm, lambda_m), [ts(i) ts(i)+0.001], y0);
    y0 = y(end, :);
    ids(i) = y0(1);
    iqs(i) = y0(2);
  end
  
  save -ascii "pmsm_data.dat" ids iqs;
  system("gnuplot -e 'set terminal dumb; plot \"pmsm_data.dat\" using 1:2 with lines title \"Id vs Iq\"'");
endfunction

simulate_pmsm();

