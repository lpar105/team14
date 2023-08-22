%% This is a shell that you will have to follow strictly. 
% You will use the plotmap() and viewmap() to display the outcome of your algorithm.

% Load sample_data_map_8, three variables will be created in your workspace. These were created as a 
% result of [m,v,s]=dfs('map_8.txt',[14,1],[1,18]);
% The solution can be viewed using 
% plotmap(m,s) 

% write your own function for the DFS algorithm.

function [retmap, retvisited, retsteps] = bfs(mapfile, startlocation, targetlocation)

    i = 0;
    
    viewmap(mapfile, 0); % shows walls
    [m]=map_convert(mapfile); % get map as matrix
    msize = size(m);

    % Initialize data structures for BFS   
    queue = {startlocation}; % Initialize queue with start location
    visited_array = m;
    steps_array = zeros(msize);
    
    % BFS algorithm to find the path
    finished = 0;

    while finished == 0
      
        pause(0.005); % slowing down the map steps 

        % Process the current location
        currentlocation = queue{1}; % Make the first element in queue the current location
        
        % initialising current row and columns
        current_row = currentlocation(1); % store the starting row
        current_col = currentlocation(2); % store the starting col
      
        % placing yellow square in current_row, current_col position
        placestep([current_row, current_col], i);
        i = i + 1;

        % Dequeue a location from the queue
        queue = queue(2:end); % remove the the first element in queue

        % Update visited status
        visited_array(current_row, current_col) = 1;

            % array for neighbours
            neighbour_array = [[(current_row + 1) current_col];
                               [(current_row - 1) current_col];
                               [current_row (current_col + 1)];
                               [current_row (current_col - 1)]];
                    
            % Enqueue adjacent unvisited locations
            for n = 1:4
                nRow = neighbour_array(n, 1);
                nCol = neighbour_array(n, 2);
                
                disp (nRow);
                disp (nCol);
                
                if nRow < 1 || nRow > msize(1) || nCol < 1 || nCol > msize(2)
                    continue; % Skip if out of bound               
                elseif m(nRow, nCol) == 1
                    continue; % If wall, skip to next neighbor
                elseif visited_array(nRow, nCol) == 1
                    continue; % If visited, skip to next neighbor
                else
                    queue{end + 1} = [nRow, nCol]; % Enqueue unvisited neighbor
                    visited_array(nRow, nCol) = 1; % Mark as visited
                    steps_array(nRow, nCol) = steps_array(current_row, current_col) + 1; % Update steps_array
                end % ending if statement
            end % ending for statement
    
    % Check if target reached
    if isequal(currentlocation, targetlocation)
        finished = 1; % Set to 1 to indicate target is reached
        break;
    end
    
    end % ending while statement

    

    % Backtrack to find the shortest path and highlight it
    if finished
        shortest_path = [targetlocation];
        path_location = targetlocation;

        while ~isequal(path_location, startlocation)
            % ... (previous code)
        end

        % Highlight the shortest path using the highlightstep function
        for step = 1:length(shortest_path)
            highlightstep(shortest_path(step, :));
        end
    end
    
    % Return the updated map, visited nodes, and steps
    retmap = m;
    retvisited = visited_array;
    retsteps = steps_array;
    
    % Plot the map after BFS
    plotmap(retmap, retsteps);

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