#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <ctime>
using namespace std;

struct mazeCell //The struct that will be representing every cell in our Maze, if the boolean value is True, then a wall exists there, if false the wall is broken. Visited flag checks if the cell has already been visited or not.
{
    public:
        mazeCell(int X, int Y, bool up = true, bool down = true, bool left = true,bool right = true ,bool visited = false){
            this->up = up;
            this->down = down;
            this->left = left;
            this->right = right;
            this->visited = visited;
            this->X_Coordinate = X;
            this->Y_Coordinate = Y;
            this->OnThePath = false;
            this->pathChecked = false;
            this->startOrEndPoint = false;
        }
        bool up,down,left,right,visited, OnThePath, pathChecked, startOrEndPoint;
        int X_Coordinate, Y_Coordinate;
};

template <class T>
class Stack //Trivial templated stack class, will be used for generating/traversing the maze.
{
private:
    vector<T> stackArray;
public:
    void push(T input);
    void pop();
    bool isEmpty();
    T top();
    Stack();
    ~Stack();
};


//All of the functions belonging to the Stack class below are trivial and require no explanation.
template <class T>
Stack<T>::Stack()
{
    //Default constructor...
}

template <class T>
Stack<T>::~Stack()
{
    this->stackArray.clear();
}

template <class T>
void Stack<T>::push(T input)
{
    this->stackArray.push_back(input);
}

template <class T>
void Stack<T>::pop()
{
    this->stackArray.pop_back();
}

template <class T>
T Stack<T>::top()
{
    return this->stackArray[stackArray.size()-1];
}

template<class T>
bool Stack<T>::isEmpty()
{
    return stackArray.size() == 0;
}


class maze //The maze class, mazeCell structs stored in a 2-D vector.
{
private:
    int numOfRows, numOfColumns;
    string mazeID;
    vector<vector<mazeCell>> mazeArray;
    int selectRandomWall();
    vector<int> selectRandomCell();
    void findPath(int x1, int y1, int x2, int y2);
public:
    void genRandomMaze();
    void drawMaze();
    void drawMazeWithPath(int x1, int y1, int x2, int y2);
    maze(int numOfRows, int numOfColumns, string mazeId);
    ~maze();

};



maze::maze(int inputColumns, int inputRows, string mazeId)
{
    numOfRows = inputRows;
    numOfColumns = inputColumns;
	mazeCell m(0, 0);
    this->mazeArray = vector<vector<mazeCell>>(inputRows, vector<mazeCell>(inputColumns, m)); //Initiliaze the 2-D vector. None of the walls are broken and none of the cells are visited.
    for (int i = 0; i < numOfRows; i++) 
    {
        for (int j = 0; j < numOfColumns; j++)
        {
            this->mazeArray[i][j] = mazeCell(j, i);
        }
    }
    mazeID = mazeId;
}

maze::~maze()
{
    this->mazeArray.clear();
}

int maze::selectRandomWall()
{
    int result = (rand()) % 4; //Choose a random integer between 0 and 3 (0 == down, 1 == left, 2 == up, 3==right)
    return result;
}

vector<int> maze::selectRandomCell()
{
    vector<int> r;
    r.push_back(0 + rand() % numOfRows); //The Y coordinate
    r.push_back(0 + rand() % numOfColumns); //The X coordinate
    return r;
}

void maze::genRandomMaze()
{
    int remainingCells = numOfRows * numOfColumns; //We will have to iterate through each cell, so let's first calculate or total cell count.
    vector<int> xandy = selectRandomCell(); // Choose random X and Y coordinates to start with.
    Stack<mazeCell*> stackObject = Stack<mazeCell*>(); //Create our stack
    stackObject.push(&this->mazeArray[xandy[0]][xandy[1]]); //Push our first cell
    this->mazeArray[xandy[0]][xandy[1]].visited = true; //Mark it as visited.
    remainingCells -= 1; //Deduct one, as we have visited one cell already.
    while(remainingCells != 0)
    {
        mazeCell *mazeIterator = stackObject.top();
        int x = mazeIterator->X_Coordinate; //Get the X Coordinate.
        int y = mazeIterator->Y_Coordinate; //Get the Y coordinate.
        int nextWall = this->selectRandomWall();
        int Counter = 3;
        while(Counter != -1) 
        {
            if(nextWall == 0 && y > 0){
                y -= 1; //Move one Y coordinate down.
                if(mazeArray[y][x].visited){ //If it is already visited, revert the change and select a wall again.
                     y += 1;
                     Counter--;
                     nextWall = (nextWall + 1) % 4; 
                }
                else{
                    stackObject.push(&mazeArray[y][x]); //If not already visited, push it to the stack.
                    mazeArray[y][x].visited = true; //Mark it as visited.
                    remainingCells--;
                    mazeArray[y][x].up = false; //Break the upper wall of our current cell
                    mazeArray[y+1][x].down = false; //Break the lower wall of our previous cell.
                    break;
                }
            }
            else if(nextWall == 1 && x > 0){
                x -= 1; //Move one X coordinate to the left.
                if(mazeArray[y][x].visited){
                    x += 1;
                    Counter--;
                    nextWall = (nextWall + 1) % 4;
                }
                else{
                    stackObject.push(&mazeArray[y][x]);
                    mazeArray[y][x].visited = true;
                    remainingCells--;
                    mazeArray[y][x].right = false;
                    mazeArray[y][x+1].left = false;
                    break;
                }
            }
            else if(nextWall == 2 && y < numOfRows-1){
                y += 1; //Move one Y coordinate up.
                if(mazeArray[y][x].visited){
                    y -= 1;
                    Counter--;
                    nextWall = (nextWall + 1) % 4;
                }
                else{
                    stackObject.push(&mazeArray[y][x]);
                    mazeArray[y][x].visited = true;
                    remainingCells--;
                    mazeArray[y][x].down = false;
                    mazeArray[y-1][x].up = false;
                    break;
                }
            }
            else if(nextWall == 3 && x < numOfColumns-1){
                x += 1; //Move one X coordinate to the right.
                if(mazeArray[y][x].visited){
                    x -= 1;
                    Counter--;
                    nextWall = (nextWall + 1) % 4;
                }
                else{
                    stackObject.push(&mazeArray[y][x]);
                    mazeArray[y][x].visited = true;
                    remainingCells--;
                    mazeArray[y][x].left = false;
                    mazeArray[y][x-1].right = false;
                    break;
                }
            }
            else{
                nextWall = (nextWall+1) % 4;
                Counter--;
            }
        }
        if(Counter==-1){
            stackObject.pop(); //Means that every cell around this cell have already been visited, thus we pop it out of the stack.
        }
    }
}

void maze::drawMaze(){
    string outFileName = "maze" + mazeID + ".txt";
    ofstream writer(outFileName);
    string result = "     ";
    for(int i = 0; i < numOfColumns; i++){
        result += "____";
    }
    result += "\n";
    for(int i = mazeArray.size()-1; i > -1; i--){
        for(int j = 0; j < mazeArray[i].size(); j++){
            if(j > 0 && i != 0){
                if(mazeArray[i][j].right && mazeArray[i][j].down) {result += "___|";}
                else if(mazeArray[i][j].right && !mazeArray[i][j].down) {result += "   |";}
                else if(!mazeArray[i][j].right && mazeArray[i][j].down) {result += "____";}
                else if(!mazeArray[i][j].right && !mazeArray[i][j].down) {result += "    ";}
            }
            else if(j==0){
                result += to_string(i);
                int numOfSpaces = 5-to_string(i).length();
                for(int i = 0; i < numOfSpaces; i++){
                    result += " ";
                }  
                if(mazeArray[i][j].left && mazeArray[i][j].right && mazeArray[i][j].down) {result += "|__|";}
                else if(mazeArray[i][j].left && !mazeArray[i][j].right && mazeArray[i][j].down) {result += "|___";}
                else if(mazeArray[i][j].left && mazeArray[i][j].right && !mazeArray[i][j].down) {result += "|  |";}
                else if(mazeArray[i][j].left && !mazeArray[i][j].right && !mazeArray[i][j].down) {result += "|   ";}
            }
            else if(j > 0 && i == 0){
                if(mazeArray[i][j].right && mazeArray[i][j].down) {result += "___|";}
                else if(mazeArray[i][j].right && !mazeArray[i][j].down) {result += "   |";}
                else if(!mazeArray[i][j].right && mazeArray[i][j].down) {result += "____";}
                else if(!mazeArray[i][j].right && !mazeArray[i][j].down) {result += "    ";}
            }
        }
        result += "\n";
    }
    result += "\n";
    result += "      ";
    for(int i = 0; i < numOfColumns; i++){
        result += to_string(i);
        for(int j = 0; j < 4-to_string(i).length(); j++){
            result += " ";
        }
    }
    writer << result;
    writer.close();
    outFileName = "maze_cells" + mazeID + ".txt";
    writer.open(outFileName);
    result = to_string(numOfRows) + " " + to_string(numOfColumns) + "\n";
    for(int i = 0; i<mazeArray.size(); i++){
        for(int j= 0; j < mazeArray[i].size(); j++){
            result += "x=" + to_string(mazeArray[i][j].X_Coordinate) + " y=" + to_string(mazeArray[i][j].Y_Coordinate) + " l=" + to_string(mazeArray[i][j].left) + " r=" + to_string(mazeArray[i][j].right) + " u=" + to_string(mazeArray[i][j].up) + " d=" + to_string(mazeArray[i][j].down) + "\n";
        }
    }
    result += "\n";
	writer << result;
    writer.close();
}

void maze::findPath(int x1, int y1, int x2, int y2){ //We will find a path between the two points x1,y1,x2,y2
    ofstream writer("maze_" + this->mazeID + "_path_" + to_string(x1) + "_" + to_string(y1) + "_" + to_string(x2) + "_" + to_string(y2) + ".txt");
    Stack<mazeCell*> stackObject = Stack<mazeCell*>(); //Create our stack
    stackObject.push(&mazeArray[y1][x1]); //Push the starting cell to the stack.
    int x = stackObject.top()->X_Coordinate, y = stackObject.top()->Y_Coordinate;
    stackObject.top()->pathChecked = true;
    mazeArray[y2][x2].startOrEndPoint = true; //mark the points as start or end.
    mazeArray[y1][x1].startOrEndPoint = true;
    while(x != x2 || y != y2){
        mazeCell* mazeIterator = stackObject.top();
        x = mazeIterator->X_Coordinate;
        y = mazeIterator->Y_Coordinate;
        int nextWall = this->selectRandomWall();
        int Counter = 3;
        while(Counter != -1) 
        {
            if(nextWall == 0 && !this->mazeArray[y][x].down){
                y-= 1; //Move one Y coordinate down.
                if(mazeArray[y][x].pathChecked){ //If we have already checked this path ignore it.
                     y += 1;
                     Counter--;
                     nextWall = (nextWall + 1) % 4; 
                }
                else{
                    stackObject.push(&mazeArray[y][x]); //If we haven't checked this path
                    mazeArray[y][x].pathChecked = true; //Mark it as checked.
                    break;
                }
            }
            else if(nextWall == 1 && !this->mazeArray[y][x].left){
                x -= 1; //Move one X coordinate to the left.
                if(mazeArray[y][x].pathChecked){ //If we have already checked this path ignore it.
                     x += 1;
                     Counter--;
                     nextWall = (nextWall + 1) % 4; 
                }
                else{
                    stackObject.push(&mazeArray[y][x]); //If we haven't checked this path
                    mazeArray[y][x].pathChecked = true; //Mark it as checked.
                    break;
                }

            }
            else if(nextWall == 2 && !this->mazeArray[y][x].up){
                y += 1; //Move one Y coordinate up.
                if(mazeArray[y][x].pathChecked){ //If we have already checked this path ignore it.
                     y -= 1;
                     Counter--;
                     nextWall = (nextWall + 1) % 4; 
                }
                else{
                    stackObject.push(&mazeArray[y][x]); //If we haven't checked this path
                    mazeArray[y][x].pathChecked = true; //Mark it as checked.
                    break;
                }

            }
            else if(nextWall == 3 && !this->mazeArray[y][x].right){
                x += 1; //Move one X coordinate to the right.
                if(mazeArray[y][x].pathChecked){ //If we have already checked this path ignore it.
                     x -= 1;
                     Counter--;
                     nextWall = (nextWall + 1) % 4; 
                }
                else{
                    stackObject.push(&mazeArray[y][x]); //If we haven't checked this path
                    mazeArray[y][x].pathChecked = true; //Mark it as checked.
                    break;
                }

            }
            else{
                nextWall = (nextWall+1) % 4;
                Counter--;
            }
        }
        if(Counter==-1){
            stackObject.top()->OnThePath = false;
            stackObject.pop(); //Means that we can't go anywhere from this path, remove it from the stack.
        }
    }
    //We have found path. Now mark the cells that are on the path.
    while(!stackObject.isEmpty()){
        mazeCell *Iterator = stackObject.top();
        writer << Iterator->X_Coordinate << " " << Iterator->Y_Coordinate << endl;
        Iterator->OnThePath = true;
        stackObject.pop();
    }
}

void maze::drawMazeWithPath(int x1, int y1, int x2, int y2){
    this->findPath(x1,y1,x2,y2); //find the path.
    ofstream writer("maze_from_" + to_string(x1) +"," + to_string(y1) + "_to_" + to_string(x2) + "," + to_string(y2) +".txt");
    string result = "     ";
    for(int i = 0; i < numOfColumns; i++){
        result += "____";
    }
    result += "\n";
    for(int i = mazeArray.size()-1; i > -1; i--){
        for(int j = 0; j < mazeArray[i].size(); j++){
            if(mazeArray[i][j].OnThePath == false && mazeArray[i][j].startOrEndPoint == false){
                if(j > 0 && i != 0){
                    if(mazeArray[i][j].right && mazeArray[i][j].down) {result += "___|";}
                    else if(mazeArray[i][j].right && !mazeArray[i][j].down) {result += "   |";}
                    else if(!mazeArray[i][j].right && mazeArray[i][j].down) {result += "____";}
                    else if(!mazeArray[i][j].right && !mazeArray[i][j].down) {result += "    ";}
                }
                else if(j==0){  
                        result += to_string(i);
                        int numOfSpaces = 5-to_string(i).length();
                        for(int i = 0; i < numOfSpaces; i++){
                            result += " ";
                        }  
                    if(mazeArray[i][j].left && mazeArray[i][j].right && mazeArray[i][j].down) {result += "|__|";}
                    else if(mazeArray[i][j].left && !mazeArray[i][j].right && mazeArray[i][j].down) {result += "|___";}
                    else if(mazeArray[i][j].left && mazeArray[i][j].right && !mazeArray[i][j].down) {result += "|  |";}
                    else if(mazeArray[i][j].left && !mazeArray[i][j].right && !mazeArray[i][j].down) {result += "|   ";}
                }
                else if(j > 0 && i == 0){
                    if(mazeArray[i][j].right && mazeArray[i][j].down) {result += "___|";}
                    else if(mazeArray[i][j].right && !mazeArray[i][j].down) {result += "   |";}
                    else if(!mazeArray[i][j].right && mazeArray[i][j].down) {result += "____";}
                    else if(!mazeArray[i][j].right && !mazeArray[i][j].down) {result += "    ";}
                }
            }
            else if (mazeArray[i][j].OnThePath == true && mazeArray[i][j].startOrEndPoint == false){
                if(j > 0 && i != 0){
                    if(mazeArray[i][j].right && mazeArray[i][j].down) {result += "_*_|";}
                    else if(mazeArray[i][j].right && !mazeArray[i][j].down) {result += " * |";}
                    else if(!mazeArray[i][j].right && mazeArray[i][j].down) {result += "_*__";}
                    else if(!mazeArray[i][j].right && !mazeArray[i][j].down) {result += " *  ";}
                }
                else if(j==0){
                    result += to_string(i);
                    int numOfSpaces = 5-to_string(i).length();
                    for(int i = 0; i < numOfSpaces; i++){
                        result += " ";
                    }    
                    if(mazeArray[i][j].left && mazeArray[i][j].right && mazeArray[i][j].down) {result += "|_*|";}
                    else if(mazeArray[i][j].left && !mazeArray[i][j].right && mazeArray[i][j].down) {result += "|_*_";}
                    else if(mazeArray[i][j].left && mazeArray[i][j].right && !mazeArray[i][j].down) {result += "|* |";}
                    else if(mazeArray[i][j].left && !mazeArray[i][j].right && !mazeArray[i][j].down) {result += "|*  ";}
                }
                else if(j > 0 && i == 0){
                    if(mazeArray[i][j].right && mazeArray[i][j].down) {result += "_*_|";}
                    else if(mazeArray[i][j].right && !mazeArray[i][j].down) {result += " * |";}
                    else if(!mazeArray[i][j].right && mazeArray[i][j].down) {result += "_*__";}
                    else if(!mazeArray[i][j].right && !mazeArray[i][j].down) {result += " *  ";}
                }  
            }
            else{
                if(j > 0 && i != 0){
                    if(mazeArray[i][j].right && mazeArray[i][j].down) {result += "_E_|";}
                    else if(mazeArray[i][j].right && !mazeArray[i][j].down) {result += " E |";}
                    else if(!mazeArray[i][j].right && mazeArray[i][j].down) {result += "_E__";}
                    else if(!mazeArray[i][j].right && !mazeArray[i][j].down) {result += " E  ";}
                }
                else if(j==0){ 
                    result += to_string(i);
                    int numOfSpaces = 5-to_string(i).length();
                    for(int i = 0; i < numOfSpaces; i++){
                        result += " ";
                    }   
                    if(mazeArray[i][j].left && mazeArray[i][j].right && mazeArray[i][j].down) {result += "|_E|";}
                    else if(mazeArray[i][j].left && !mazeArray[i][j].right && mazeArray[i][j].down) {result += "|_E_";}
                    else if(mazeArray[i][j].left && mazeArray[i][j].right && !mazeArray[i][j].down) {result += "|E |";}
                    else if(mazeArray[i][j].left && !mazeArray[i][j].right && !mazeArray[i][j].down) {result += "| E ";}
                }
                else if(j > 0 && i == 0){
                    if(mazeArray[i][j].right && mazeArray[i][j].down) {result += "_E_|";}
                    else if(mazeArray[i][j].right && !mazeArray[i][j].down) {result += " E |";}
                    else if(!mazeArray[i][j].right && mazeArray[i][j].down) {result += "_E__";}
                    else if(!mazeArray[i][j].right && !mazeArray[i][j].down) {result += "  E ";}
                }
            }
        }
        result += "\n";
    }
    result += "\n";
    result += "      ";
    for(int i = 0; i < numOfColumns; i++){
        result += to_string(i);
        for(int j = 0; j < 4-to_string(i).length(); j++){
            result += " ";
        }
    }
    writer << result;
    writer.close();
}
