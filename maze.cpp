#include "maze.h"

int main()
{
    // Generate Mazes
    srand(time(0));
    int mazeCount, M, N;
    cout << "Enter the number of mazes: ";
    cin >> mazeCount;
    cout << "Enter the number of rows and columns (M and N): ";
    cin >> M >> N;
    vector <maze> mazes;
    for(int i = 1; i <= mazeCount; i++)
    {
        maze maze(N, M, to_string(i));
        maze.genRandomMaze();
        maze.drawMaze();
        mazes.push_back(maze);
    }
    cout << "All maze are generated." << endl << endl;

    // Path finding
    int entX, entY, extX, extY, mazeID;
    cout << "Enter a maze ID between 1 to " << mazeCount << " inclusive to find a path: ";
    cin >> mazeID;
    cout << "Enter x and y coordinates of the entry points (x,y) or (column,row): ";
    cin >> entX >> entY;
    cout << "Enter x and y coordinates of the exit points (x,y) or (column,row): ";
    cin >> extX >> extY;
    mazes[mazeID-1].drawMazeWithPath(entX, entY, extX, extY);
    return 0;
}
