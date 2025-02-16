

pkg load control  % Load control system package

% System Parameters
Jb = 0.01;  % Ball moment of inertia
b = 0.1;    % Damping coefficient
rw = 0.05;  % Wheel radius
Ts = 0.05;  % Sampling time (discrete step)

% Continuous State-Space Model
A = [0 1 0; 
     0 -b/Jb rw/Jb; 
     0 0 0];

B = [0; 1/Jb; 1];

C = [1 0 0]; 
D = 0;

% Discretization using Zero-Order Hold
sys_c = ss(A, B, C, D);  % Continuous system
sys_d = c2d(sys_c, Ts, 'zoh');  % Convert to discrete system

[Ad, Bd, Cd, Dd] = ssdata(sys_d);  % Get discrete matrices

% LQR Control Design
Q = diag([10, 1, 1]);  % State weight matrix (penalizes position and velocity)
R = 0.1;  % Control effort weight
K = dlqr(Ad, Bd, Q, R);  % Solve for LQR gain

% Simulation Parameters
N = 100;  % Number of steps
x = [0.2; 0; 0];  % Initial state (ball displaced)
X_log = zeros(3, N);
U_log = zeros(1, N);

% Simulate the closed-loop system
for k = 1:N
    u = -K * x;  % LQR state feedback control
    x = Ad * x + Bd * u;  % State update
    X_log(:, k) = x;  % Log states
    U_log(:, k) = u;  % Log control input
end

% Plot Results
t = (0:N-1) * Ts;
figure;
subplot(2,1,1);
plot(t, X_log(1, :), 'b', 'linewidth', 2); hold on;
plot(t, X_log(2, :), 'r', 'linewidth', 2);
plot(t, X_log(3, :), 'g', 'linewidth', 2);
legend('Ball Position', 'Ball Velocity', 'Wheel Velocity');
xlabel('Time (s)');
ylabel('State Variables');
title('LQR-Controlled Response of Ball-on-Wheel System');
grid on;

subplot(2,1,2);
plot(t, U_log, 'm', 'linewidth', 2);
xlabel('Time (s)');
ylabel('Control Input (Torque)');
title('LQR Control Input');
grid on;



