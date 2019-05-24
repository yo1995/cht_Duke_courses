%% matlab default
pt = [0, 0, 1; 0, 0, 0; 1, 0, 0; 0, 1, 0; 1, 1, 1];
plot3(pt(:, 1), pt(:, 2), pt(:, 3), '-o');
set(gca, 'projection', 'perspective')

%% manually project
f = 3;
theta = pi / 6;
F = [f, 0, 0
    0, f, 0;
    0, 0, 1];
P = [1, 0, 0, 0;
    0, 1, 0, 0;
    0, 0, 1, 0];
R = [cos(theta), -sin(theta), 0;
    sin(theta), cos(theta), 0;
    0, 0, 1];
T = [1, 0, 0, -2;
    0, 1, 0, 3;
    0, 0, 1, 0];

H = F*R*T;
pt_homo = [pt, ones(size(pt, 1), 1)];
pt_homo_projd = pt_homo * H';
pt_2 = pt_homo_projd(:, 1:2) ./ pt_homo_projd(:, 3);
plot(pt_2(:, 1), pt_2(:, 2), 'r.' , 'Markersize', 15)
axis equal square;
