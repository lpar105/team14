%% This is a shell that you will have to follow strictly.
% You will use the plotmap() and viewmap() to display the outcome of your algorithm.
% Load sample_data_map_8, three variables will be created in your workspace. These were created as a
% result of [m,v,s]=dfs('map_8.txt',[14,1],[1,18]);
% The solution can be viewed using
% plotmap(m,s)
% write your own function for the DFS algorithm.
% G cost = distance from starting node
% H cost = distance from end node
% F cost = GCost + HCost
% THIS IS A* NOT DFS!!!

function [retmap,retvisited,retsteps] = astar(mapfile,start,target)
viewmap(mapfile, 0); % shows walls
[m]=map_convert(mapfile); % get map as matrix
msize = size(m);
% create parent set
parents = zeros(msize);
% Create open set and add the start position by giving it an fcost
open = inf(msize);
open(start(1), start(2)) = compute_fcost(start(1), start(2));
% Create closed set
closed = false(msize);
disp("Entering While Loop")
while (true)
    % curRow & curCol = node in OPEN with lowest f_cost
    minVal =  min(open,[],"all");
    [curRow, curCol] = find(open == minVal, 1);
    % remove current from open
    open(curRow, curCol) = 999999;
    % add current to closed and display
    closed(curRow, curCol) = minVal;
    % if current is the target node return
    if (target(1) == curRow & target(2) == curCol)
        statement = "path found";
        fpos1 = curRow;
        fpos2 = curCol;
        while (start(1) ~= fpos1 | start(2) ~= fpos2)
            if (parents(fpos1, fpos2) == 1)
                fpos1 = fpos1 + 1;
            elseif (parents(fpos1, fpos2) == 2)
                fpos2 = fpos2 + 1;
            elseif (parents(fpos1, fpos2) == 3)
                fpos1 = fpos1 - 1;
            elseif (parents(fpos1, fpos2) == 4)
                fpos2 = fpos2 - 1;
            end
            highlightstep([fpos1, fpos2]);
        end
        highlightstep([curRow, curCol]);
        return
    end
    n1 = [(curRow + 1) curCol]
    n2 = [(curRow - 1) curCol]
    n3 = [curRow (curCol + 1)]
    n4 = [curRow (curCol - 1)]
    neighbours = [n1; n2; n3; n4]
    % for each neighbour of the current node
    for n = 1:4
        nRow = neighbours(n,1);
        nCol = neighbours(n,2);
        % if neighnour is not traversable or neighbour is in closed
        % skip to next neighbour
        if (isValid(nRow, nCol))
            % if new path to neighbour is shorter OR neighbour is not in OPEN
            if(shouldUpdate(nRow, nCol))
                % set parent of neighbour to current
                if (nRow + 1 == curRow)
                    parents(nRow, nCol) = 1;
                elseif (nCol + 1 == curCol)
                    parents(nRow, nCol) = 2;
                elseif (nRow - 1 == curRow)
                    parents(nRow, nCol) = 3;
                elseif (nCol - 1 == curCol)
                    parents(nRow, nCol) = 4;
                else
                    statement = "ERROR"
                    nRow, nCol, curRow, curCol
                end
                % set f_cost of neighbour
                closed(nRow, nCol) = compute_fcost(nRow, nCol);
                % if neighbour is not in open
                if(isinf(open(nRow, nCol)))
                    % add neighbour to open
                    open(nRow, nCol) = compute_fcost(nRow, nCol);
                end
            end
        end
    end
end
    function valid = isValid(row, col)
        valid = true;
        if (row < 1 | col < 1 | row > msize(1) | col > msize(2))
            valid = false;
        elseif (m(row, col) == 1)
            valid = false;
        elseif (closed(row, col) ~= 0)
            valid = false;
        end
    end
    function update = shouldUpdate(row, col)
        % if new path to neighbour is shorter OR neighbour is not in OPEN
        if(isinf(open(row, col)))
            update = true; return
        end
        
        update = true; % TODO fix parentage
    end
    function cost = compute_fcost(curRow, curCol)
        cost = 0;
        pos1 = curRow;
        pos2 = curCol;
        while (start(1) ~= pos1 | start(2) ~= pos2)
            disp([cost, pos1, pos2])
            cost = cost + 1;
            if (parents(pos1, pos2) == 1)
                pos1 = pos1 + 1;
            elseif (parents(pos1, pos2) == 2)
                pos2 = pos2 + 1;
            elseif (parents(pos1, pos2) == 3)
                pos1 = pos1 - 1;
            elseif (parents(pos1, pos2) == 4)
                pos2 = pos2 - 1;
            else 
                parents
                pause(5000)
            end
        end
        statement = "gcost is = " + cost
        cost = cost + abs(curRow - target(1));
        cost = cost + abs(curCol - target(2));
        statement = "cost is = "+ cost 
        placestep([curRow, curCol], cost);
        pause(0.015)
    end
end
function placestep(position,i)
% This function will plot a insert yellow rectangle and also print a number in this rectangle. Use with plotmap/viewmap.
position = [16-position(1) position(2)];
position=[position(2)+0.1 position(1)+0.1];
rectangle('Position',[position,0.8,0.8],'FaceColor','y');
c=sprintf('%d',i);
text(position(1)+0.2,position(2)+0.2,c,'FontSize',10);
end
function highlightstep(position)
% This function will plot a insert green rectangle
position = [16-position(1) position(2)];
position=[position(2)+0.1 position(1)+0.1];
rectangle('Position',[position,0.3,0.3],'FaceColor','g');
end