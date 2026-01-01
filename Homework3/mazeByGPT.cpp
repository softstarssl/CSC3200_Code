#include "maze.h"

/**
 * 迷宫搜索函数 - 使用深度优先搜索（DFS）
 * 
 * @param maze: 迷宫矩阵，包含墙壁、自由空间、起点和终点
 * @param start: 搜索起始位置
 * @param end: 搜索终止位置
 * @param stack: 用于维护搜索边界的栈（frontier）
 * @param visited: 访问标记矩阵，记录哪些单元格已被发现
 */
void search_maze(maze_t const& maze, maze_pos start, maze_pos end,
                 std::stack<maze_pos>& stack, visited_t& visited) {
    
    // 获取迷宫的尺寸（行数和列数）
    int rows = maze.size();
    int cols = maze[0].size();
    
    // 初始化：将起始位置压入栈，并标记为已访问
    stack.push(start);
    visited[start.x][start.y] = true;
    
    // 定义四个移动方向：上、下、左、右
    // dx[i] 和 dy[i] 配合使用可以得到当前位置的四个相邻位置
    int dx[] = {-1, 1, 0, 0};  // x方向的偏移量
    int dy[] = {0, 0, -1, 1};  // y方向的偏移量
    
    // DFS 主循环：当栈不为空时继续搜索
    while (!stack.empty()) {
        // 从栈顶取出当前要探索的位置
        maze_pos current = stack.top();
        stack.pop();
        
        // 如果当前位置是终点，不再从此位置继续探索邻居
        // 但继续处理栈中的其他位置（如果有的话）
        if (current == end) {
            continue;  // 跳过对终点邻居的探索
        }
        
        // 探索当前位置的四个方向的邻居
        for (int i = 0; i < 4; i++) {
            // 计算邻居的坐标
            int new_x = current.x + dx[i];
            int new_y = current.y + dy[i];
            
            // 边界检查：确保新位置在迷宫范围内
            if (new_x >= 0 && new_x < rows && new_y >= 0 && new_y < cols) {
                // 检查该位置是否满足以下条件：
                // 1. 尚未被访问过
                // 2. 不是墙壁（即是自由空间、起点或终点）
                if (!visited[new_x][new_y] && 
                    maze[new_x][new_y] != maze_cell::wall) {
                    
                    // 标记该位置为已访问
                    // 注意：在压入栈之前就标记，避免重复压入
                    visited[new_x][new_y] = true;
                    
                    // 将该位置压入栈，等待后续探索
                    stack.push(maze_pos(new_x, new_y));
                }
            }
        }
    }
}