

clear; clc; close all;

% Use GNUplot as the graphics toolkit
graphics_toolkit("gnuplot")
setenv("GNUTERM", "dumb"); % Set dumb terminal for ASCII plotting

% PMSM Parameters
R_s = 0.5;        % Stator resistance (ohms)
L_d = 0.008;      % d-axis inductance (H)
L_q = 0.008;      % q-axis inductance (H)
lambda_m = 0.175; % Permanent magnet flux linkage (Wb)
P = 4;            % Number of poles
J = 0.01;         % Inertia (kg.m^2)
B = 0.001;        % Damping coefficient
T_L = 0;          % Load torque (Nm)

% Simulation parameters
dt = 1e-4;        % Time step (s)
t_end = 0.1;      % Simulation time (s)
t = 0:dt:t_end;   % Time vector

% Initial conditions
i_d = 0;
i_q = 0;
omega = 0;

% Input voltages
v_d = 0;         % Assume v_d = 0 for simplification
v_q = 100;       % Apply voltage in q-axis

% Data storage
i_d_log = zeros(size(t));
i_q_log = zeros(size(t));
omega_log = zeros(size(t));

% Simulation loop
for k = 1:length(t)
    % Electrical dynamics (Euler discretization)
    di_d = (v_d - R_s * i_d + omega * L_q * i_q) / L_d;
    di_q = (v_q - R_s * i_q - omega * L_d * i_d - omega * lambda_m) / L_q;
    
    i_d = i_d + di_d * dt;
    i_q = i_q + di_q * dt;
    
    % Compute torque
    T_e = (3/2) * (P/2) * (lambda_m * i_q + (L_d - L_q) * i_d * i_q);
    
    % Mechanical dynamics (Euler discretization)
    domega = (T_e - T_L - B * omega) / J;
    omega = omega + domega * dt;
    
    % Store results
    i_d_log(k) = i_d;
    i_q_log(k) = i_q;
    omega_log(k) = omega;
end

% Plot using gnuplot dumb terminal
% figure(1);---------------1
subplot(3,1,1);------------2
plot(t, i_d_log, 'b');
xlabel('Time (s)'); ylabel('i_d (A)');
title('d-axis Current');
% grid on;
% drawnow(); % Forces the figure to be displayed ------------------1

%figure(2);
subplot(3,1,2);
plot(t, i_q_log, 'r');
xlabel('Time (s)'); ylabel('i_q (A)');
title('q-axis Current');
% grid on;
% drawnow(); % Forces the figure to be displayed

%figure(3);
subplot(3,1,3);
plot(t, omega_log, 'k');
xlabel('Time (s)'); ylabel('\omega (rad/s)');
title('Rotor Speed');
% grid on;
% drawnow();---------------1
drawnow(); % Forces the figure to be displayed ------------------2 --> a legvegen kell egy drawnow()

% disp(omega_log)


