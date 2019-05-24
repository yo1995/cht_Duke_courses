function draw_colorful_blocks(N)
% clear
% the whole method is to devide circumstances into N<9 and N>=9 when N is
% eligible. process each case seperately and then print the figure. beware
% of the boundary of input and just throw exception when it does not meet
% the requirement, for a simple hmwk. pcolor have issues about initial axis
% values so minor modification was made.

% test definition of N, N should belongs to 0-17 so that pixels will be properly colored with 3 colors
% pre-definition part
width_of_graph = 11;
graph = zeros(width_of_graph,width_of_graph);
% define the color map according to the figure shown in textbook
map = [1,1,1;
    1,0,0;
    0,1,0;
    0,0,1];
% white = 0;
red = 1;
green = 2;
blue = 3;
% plotting part

% draw the red line first since it is more perceivable. edit the program to
% draw green and blue pixels

% check the case. since only 2 cases we can just us if-else-end pattern
if N < 10 && N >= 0
    % draw the red pixels
    for ii = 1:N+1
        graph(N-ii+2,ii) = red;
    end
    % draw the green pixels
    for jj = 1:2:N+2
        graph(jj,N-jj+3) = green;
    end
    % draw the blue pixels
    for kk = 2:2:N+2
        graph(kk,N-kk+3) = blue;
    end
    pcolor(graph);colormap(map);axis equal off; 
% beware of the boundary of larger N. it might be tricky.
elseif N >= 10 && N < 18
    % draw the red line
    for ii = N - width_of_graph+2: width_of_graph
        graph(N-ii+2,ii) = red;
    end
    % draw the green line
    for jj = N - width_of_graph+3:2: width_of_graph
        graph(jj,N-jj+3) = green;
    end
    % draw the blue line
    for kk = N - width_of_graph+4:2: width_of_graph
        graph(kk,N-kk+3) = blue;
    end
    pcolor(graph);colormap(map);axis equal off; 
% simply dump the e without proceeding it.
else
    disp(['Input Exceeding the drawable region!'])
end