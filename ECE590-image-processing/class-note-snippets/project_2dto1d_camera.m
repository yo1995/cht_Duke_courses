%% perspective coord setup
f = 2;
u0 = 1.23;  % 0

theta = pi / 4 - atan(u0/f);
F = [f, 0, 0
    0, 1, 0];
    % 0, 0, 1];  % take out the last row makes it 1-D and perspective
R = [cos(theta), -sin(theta), 0;
    sin(theta), cos(theta), 0;
    0, 0, 1];
camera_pos = [-3, 2];
T = [eye(2), -camera_pos';
    0, 0, 1];
% T = [1, 0, 3;
%     0, 1, -2;
%     0, 0, 1];
P = F * R * T;

%% u_A = P_a * x'

u = u0;
u_ = [u; 1];
x_ = P \ u_;
P * x_  % should be [0,1]'

x = x_(1:2) / x_(3);
plot([camera_pos(1), x(1)],[camera_pos(2), x(2)], 'o-')
axis equal 
x