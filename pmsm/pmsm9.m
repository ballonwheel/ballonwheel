% PMSM Discrete-Time Simulation in Octave

clear; clc; close all;

% Motor Parameters
R = 0.5;          % Stator resistance (Ohms)
Ld = 0.01;        % d-axis inductance (H)
Lq = 0.01;        % q-axis inductance (H)
J = 0.01;         % Moment of inertia (kg.m^2)
B = 0.001;        % Friction coefficient (N.m.s)
P = 4;            % Pole pairs
lambda_m = 0.1;   % PM Flux linkage (Wb)
Ts = 1e-4;        % Sampling time (s)
Tsim = 1;         % Total simulation time (s)

% Discretization using Euler Approximation
A = [-R/Ld,  -1/Ld,      0;
      1/Lq, -R/Lq, lambda_m/Lq;
      0,    (3/2)*P*lambda_m/J, -B/J];

B = [1/Ld,  0;
     0,  1/Lq;
     0,  0];

Ad = eye(3) + A * Ts;   % Discrete A matrix
Bd = B * Ts;            % Discrete B matrix

% Initial conditions
id = 0; iq = 0; omega = 0;
x = [id; iq; omega];

% Input Voltages (Step input at t=0.1s)
Vd = 0; Vq = 10;  % Constant Vq to generate torque

% Time vector
time = 0:Ts:Tsim;
x_log = zeros(length(time), 3);

% Simulation loop
for k = 1:length(time)
    % Save state for plotting
    x_log(k, :) = x';
    
    % Update state using discrete-time equations
    u = [Vd; Vq];
    x = Ad * x + Bd * u;
end

% Plot results
figure;
subplot(3,1,1);
plot(time, x_log(:,1), 'r');
xlabel('Time (s)'); ylabel('i_d (A)'); title('d-axis Current');

subplot(3,1,2);
plot(time, x_log(:,2), 'b');
xlabel('Time (s)'); ylabel('i_q (A)'); title('q-axis Current');

subplot(3,1,3);
plot(time, x_log(:,3), 'g');
xlabel('Time (s)'); ylabel('\omega (rad/s)'); title('Motor Speed');

grid on;

