function P = inter(x1,y1,x2,y2, x_guess)
%fit linear polynomial
p1 = polyfit([x1(1), x2(1)],[x1(2), x2(2)],1);
p2 = polyfit([y1(1), y2(1)],[y1(2), y2(2)],1);
%calculate intersection
x_intersect = fzero(@(x)polyval(p1-p2,x),x_guess); % 3 as starting point
y_intersect = polyval(p1,x_intersect);
P = zeros(2,1);
P(1)=x_intersect;
P(2)=y_intersect;
