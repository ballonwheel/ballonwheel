


pkg load  parallel;

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
  Vd = 0.1; 
  Vq = 1;
  V = [Vd, Vq];
endfunction

function simulate_pmsm()
  Ld = 300e-6;
  Lq = 300e-6;
  Rs = 300e-3;
  lambda_m = 2.6e-4;
  J = 10e-4; 
  B = 10e-5;
  
  y0 = [0; 0; 0];
  V = input_voltage();
  [t, y] = ode45(@(t, y) pmsm_dynamics(t, y, V(1), V(2), Ld, Lq, Rs, y(3), lambda_m, J, B), [0 10], y0);
  
  % Extract steady-state values for id and iq
  id_iq_data = [y(end,1), y(end,2)];
  
  % Calculate speed and torque at each time step
  speed_torque_data = [t, y(:,3), 1.5 * (lambda_m * y(:,2) + (Ld - Lq) * y(:,1) .* y(:,2))];
  
  % Save steady-state data
  save -ascii "pmsm_steady_state.dat" id_iq_data;
  save -ascii "pmsm_speed_torque.dat" speed_torque_data;
  
 
  disp("speed, torque"), disp(speed_torque_data) 
  % system("gnuplot -e 'set terminal dumb; plot \"pmsm_speed_torque.dat\" using 1:2 with lines title \"Speed\", \"pmsm_speed_torque.dat\" using 1:3 with lines title \"Torque\"'");
  

 
  % Plot id and iq (direct and quadrature axes)
  disp("steady state:"), disp(id_iq_data)
  % system("gnuplot -e 'set terminal dumb; \  
  %  plot \"pmsm_steady_state.dat\" using 1:2 with lines title \"Id (Direct Axis)\",\ 
  %       \"pmsm_steady_state.dat\" using 1:3 with lines title \"Iq (Quadrature Axis)\"'");

  % system("gnuplot -e 'set terminal dumb; plot \"pmsm_steady_state.dat\"'");

endfunction

simulate_pmsm();

