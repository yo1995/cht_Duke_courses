function P = inter2(p1,p2,p3,p4)
% calculate the intersection point of two lines
% line1 = p1 -> p2, line2 = p3 -> p4

x1 = p1(1); y1 = p1(2);
x2 = p2(1); y2 = p2(2);
x3 = p3(1); y3 = p3(2);
x4 = p4(1); y4 = p4(2);

A = [(y1 - y2) / (x1 - x2), -1;
    (y3 - y4) / (x3 - x4), -1];
B = [(y1-y2)*x1/(x1-x2)-y1;
    (y3-y4)*x3/(x3-x4)-y3];
P = A\B;

