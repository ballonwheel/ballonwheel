clear; clc; close all;
graphics_toolkit("gnuplot")
setenv("GNUTERM", "dumb");  % Use ASCII plotting mode

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
dt = 1e-3;        % Time step (s)
t_end = 0.5;      % Total simulation time (s)
t = 0:dt:t_end;   % Time vector

% PID gains
Kp_id = 10; Ki_id = 100; Kd_id = 0.1;
Kp_iq = 10; Ki_iq = 100; Kd_iq = 0.1;
Kp_speed = 5; Ki_speed = 50; Kd_speed = 0.01;

% Initialize variables
i_d = 0; i_q = 0;
omega = 0;
i_d_ref = 0;
i_q_ref = 1; % Reference q-axis current (determined by speed controller)
v_d = 0; v_q = 0;

% PID error storage
int_error_id = 0; int_error_iq = 0; int_error_speed = 0;
prev_error_id = 0; prev_error_iq = 0; prev_error_speed = 0;

omega_log = zeros(size(t));

for k = 1:length(t)
    % Speed PID Controller
    error_speed = i_q_ref - omega;
    int_error_speed = int_error_speed + error_speed * dt;
    der_error_speed = (error_speed - prev_error_speed) / dt;
    i_q_ref = Kp_speed * error_speed + Ki_speed * int_error_speed + Kd_speed * der_error_speed;
    prev_error_speed = error_speed;
    
    % d-axis Current PID Controller
    error_id = i_d_ref - i_d;
    int_error_id = int_error_id + error_id * dt;
    der_error_id = (error_id - prev_error_id) / dt;
    v_d = Kp_id * error_id + Ki_id * int_error_id + Kd_id * der_error_id;
    prev_error_id = error_id;
    
    % q-axis Current PID Controller
    error_iq = i_q_ref - i_q;
    int_error_iq = int_error_iq + error_iq * dt;
    der_error_iq = (error_iq - prev_error_iq) / dt;
    v_q = Kp_iq * error_iq + Ki_iq * int_error_iq + Kd_iq * der_error_iq;
    prev_error_iq = error_iq;
    
    % PMSM State Equations
    di_d = (v_d - R_s * i_d + omega * L_q * i_q) / L_d;
    di_q = (v_q - R_s * i_q - omega * L_d * i_d - omega * lambda_m) / L_q;
    domega = (P / 2) * (lambda_m * i_q - (L_d - L_q) * i_d * i_q) / J - (B * omega) / J;
    
    % Update states
    i_d = i_d + di_d * dt;
    i_q = i_q + di_q * dt;
    omega = omega + domega * dt;
    
    % Log data
    omega_log(k) = omega;
end

% Plot results
figure;
plot(t, omega_log, 'k');
xlabel('Time (s)'); ylabel('\omega (rad/s)');
title('Motor Speed Response');


