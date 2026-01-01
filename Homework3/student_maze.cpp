#include "maze.h"
#include <vector>
#include <stack>

// maze.h  
// maze_cell : The state of a point
// maze_t : 2-D arrays , thr grid maze_cell ( vector<vector<maze_cell>> )
// maze_pos : Point (x,y)
// visited_t : Whether (x,y) is visited or not ( vector<vector<bool>>)

// const int dx[] = {0,0,-1,1},dy[] = {1,-1,0,0};
const maze_pos d[] = {{0,-1},{0,1},{-1,0},{1,0}};
void search_maze(maze_t const& maze, maze_pos start, maze_pos end, std::stack<maze_pos>& stack, visited_t& visited)
{
    
    int n = maze.size();
    int m = (maze.empty()) ? 0 : maze[0].size();

    auto check = [&](int x,int y) ->bool
    {
        return (x >= 0) && (x < n) && (y >= 0) && (y < m) && (maze[x][y] != maze_cell::wall) && (!visited[x][y]);
    };

    auto [u,v] = start;
    if(!check(u,v)) return ;
    stack.push(start), visited[u][v] = true;

    while(!stack.empty())
    {
        maze_pos now = stack.top();
        // stack.pop();

        int x = now.x,y = now.y;
        if(now == end) break ;

        bool path = false;
        // for(int k = 0;k < 4;++k)
        // {
        //     int i = x + dx[k],j = y + dy[k];
        //     if(check(i,j))
        //     {
        //         path = true ;
        //         visited[i][j] = true;
        //         stack.push(maze_pos{i,j});
        //         break ;
        //     }
        // }
        for(auto [dx,dy] : d)
        {
            int i = x + dx,j = y + dy;
            if(check(i,j))
            {
                path = true ;
                visited[i][j] = true;
                stack.push(maze_pos{i,j});
                break ;
            }
        }
        if(!path) stack.pop();
    }
    
}