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
  [t, y] = ode45(@(t, y) pmsm_dynamics(t, y, V(1), V(2), Ld, Lq, Rs, y(3), lambda_m, J, B), [0 0.1], y0);
  
  id_iq_data = [y(:,1), y(:,2)];
  speed_torque_data = [t, y(:,3), 1.5 * (lambda_m * y(:,2) + (Ld - Lq) * y(:,1) .* y(:,2))];
  
  save -ascii "pmsm_data.dat" id_iq_data;
  save -ascii "pmsm_speed_torque.dat" speed_torque_data;
  
  system("gnuplot -e 'set terminal dumb; plot \"pmsm_data.dat\" using 1:2 with lines title \"Id vs Iq\"'");
  system("gnuplot -e 'set terminal dumb; plot \"pmsm_speed_torque.dat\" using 1:2 with lines title \"Speed\", \"pmsm_speed_torque.dat\" using 1:3 with lines title \"Torque\"'");
endfunction

simulate_pmsm();

