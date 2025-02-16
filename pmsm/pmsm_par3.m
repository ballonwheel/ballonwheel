

pkg load parallel;

function dydt = pmsm_dynamics(t, y, Vd, Vq, Ld, Lq, Rs, wm, lambda_m, J, B)
  id = y(1);
  iq = y(2);
  omega = y(3);
  
  did_dt = (Vd - Rs*id + omega*Lq*iq) / Ld;
  diq_dt = (Vq - Rs*iq - omega*(Ld*id + lambda_m)) / Lq;
  Te = 1.5 * (lambda_m * iq + (Ld - Lq) * id * iq);
  domega_dt = (Te - B * omega) / J;
  
  dydt = [did_dt; diq_dt; domega_dt];
endfunction

function V = input_voltage()
  Vd = 100; 
  Vq = 100;
  V = [Vd, Vq];
endfunction

function simulate_pmsm()
  Ld = 0.005;
  Lq = 0.005;
  Rs = 0.1;
  lambda_m = 0.02;
  J = 0.01; 
  B = 0.001;
  
  y0 = [0; 0; 0];
  V = input_voltage();
  [t, y] = ode45(@(t, y) pmsm_dynamics(t, y, V(1), V(2), Ld, Lq, Rs, y(3), lambda_m, J, B), [0 10], y0);
  
  % Extract the last values (steady state) after simulation
  id_iq_data = [y(end,1), y(end,2)];
  
  % Save steady state values
  save -ascii "pmsm_steady_state.dat" id_iq_data;
  
  % Plot steady state id vs iq
  system("gnuplot -e 'set terminal dumb; plot \"pmsm_steady_state.dat\" using 1:2 with points title \"Steady State Id vs Iq\"'");
endfunction

simulate_pmsm();

