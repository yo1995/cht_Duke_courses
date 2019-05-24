function draw_colorful_blocks(N)
% clear;clf;clc
% N = 1;
% test definition of N, N should belongs to 1-6
% pre-definition part
number_of_orange  = 0;
width_of_graph = 22;
graph = zeros(width_of_graph,width_of_graph);
% define the color map according to the figure shown in textbook
map = [0,0,1;
    0,1,0;
    1,0.5,0];
% blue = 0;
green = 1;
orange = 2;
% plotting part

% draw the red line first since it is more perceivable. edit the program to
% draw green and blue pixels

% check the case. since only 2 cases we can just us if-else-end pattern
if N < 10 && N >= 0
    % draw the red pixels
    if N == 1 
        graph(4,1) = green;
        graph(5,1) = green;
        graph(6,1) = green;
        graph(7,1) = green;
        graph(4,2) = green;
        graph(5,2) = green;
        graph(6,2) = green;
        graph(7,2) = green;
        graph(22,22) = orange;
    elseif N == 2
        graph(4,1) = green;
        graph(5,1) = green;
        graph(6,1) = green;
        graph(7,1) = green;
        graph(4,2) = green;
        graph(5,2) = green;
        graph(6,2) = green;
        graph(7,2) = green;
        
        graph(5,3) = green;
        graph(6,3) = green;
        graph(7,3) = green;
        graph(8,3) = green;
        graph(5,4) = green;
        graph(6,4) = green;
        graph(7,4) = green;
        graph(8,4) = green;
        graph(22,22) = orange;
        
        number_of_orange  = 0;
    % always put orange block at the left-most part of the grid.
    else
        number_of_orange = floor((N+1)/2-1);
        graph(3,1) = orange;
        graph(4,1) = orange;
        graph(5,1) = orange;
        graph(6,1) = orange;
        graph(3,2) = orange;
        graph(4,2) = orange;
        graph(5,2) = orange;
        graph(6,2) = orange;
        number_of_green = N-number_of_orange;
        for ii = 3:2:N*2-1
            graph((ii+1)/2+3,ii) = green;
            graph((ii+1)/2+4,ii) = green;
            graph((ii+1)/2+5,ii) = green;
            graph((ii+1)/2+6,ii) = green;
            graph((ii+1)/2+3,ii+1) = green;
            graph((ii+1)/2+4,ii+1) = green;
            graph((ii+1)/2+5,ii+1) = green;
            graph((ii+1)/2+6,ii+1) = green;            
        end
    end
    % blcorner = ;
    pcolor(graph);colormap(map);axis equal off;
    title(['The number of orange is ' num2str(number_of_orange)])
% beware of the boundary of larger N. it might be tricky. 
% simply dump the e without proceeding it.
else
    disp(['Input Exceeding the drawable region!'])
end