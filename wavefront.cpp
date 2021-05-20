/* Author Helen Wubneh
Course CSCI 4360 
Due Date 4/25/2021*/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <random>
using namespace std;

//constant size
const int ROWS = 8;
const int COLUMNS = 16;

//a Node class 
Class Node{
	int row;
	int col;
	int val;
};

void InputWorld(int world[][COLUMNS], int& grow, int& gCol) {
	cout << "Please enter which world file you would like to use: ";
	ifstream myFile;
	string fileName;
	cin >> fileName;
	
	myFile.open(fileName);

	//input the data from the world file
	for (int i = 0; i < ROWS; i++) {
		string line;
		getline(worldFile, line);
		stringstream ss(line);
		intvalues;
		for (int j = 0; j < COLUMNS; j++) {
			ss >> values;
			if (values == 1)
				world[i][j] = 1;
		}
	}

	//get valid goal point
	bool valid = false;
	while (!valid) {
		cout << "Enter Starting Point [ROW][COL]: ";
		cin >> goalRow >> goalCol; //get X Y of goal
		if (world[goalRow][goalCol] == 1) {
			cout << "ERROR: Wall Obstruction." << endl; //display error message if goal point is invalid
		}
		else {
			valid = true;
		}
	}
}

//Create_wavefront_map() creates wavefront map 
void Create_wavefront_map(int world[][COLUMNS], int& goalRow, int& goalCol) {
	//initialize queue
	qNode queue[(ROWS * COLUMNS * 4)]; //maximum slots needed for worst case
	int qCount = 0; //counter to keep track of nodes in que
	qNode currentNode = queue[0] = createNode(goalRow, goalCol, 2); // initialize currentNode & queue[0] to goal point
	qCount++;

	int currentIndex = 0;
	while ((currentNode.row != 0) || (currentNode.col != 0)) { //while not in goal point
		currentNode = queue[currentIndex];
		if ((world[currentNode.row][currentNode.col] == 0)) {
			world[currentNode.row][currentNode.col] = currentNode.val; //set current point to wave value (previous + 1)
			//add above to queue
			if (isValid(currentNode.row - 1, currentNode.col)) {
				queue[qCount] = createNode(currentNode.row - 1, currentNode.col, currentNode.val + 1);
				qCount++;
			}
			//add below to queue
			if (isValid(currentNode.row + 1, currentNode.col)) {
				queue[qCount] = createNode(currentNode.row + 1, currentNode.col, currentNode.val + 1);
				qCount++;
			}
			//add right to queue
			if (isValid(currentNode.row, currentNode.col + 1)) {
				queue[qCount] = createNode(currentNode.row, currentNode.col + 1, currentNode.val + 1);
				qCount++;
			}
			//add left to queue
			if (isValid(currentNode.row, currentNode.col - 1)) {
				queue[qCount] = createNode(currentNode.row, currentNode.col - 1, currentNode.val + 1);
				qCount++;
			}
		}
		currentIndex++;
	}
}

//Find_Path() uses wavefront map to generate a list of moves to get to goal
void Find_Path(int world[][COLUMNS], qNode path[], int& pathCount) {
	qNode currentNode = createNode(0, 0, world[0][0]); //begin at point 0,0
	pathCount = 0;

	while (currentNode.val != 2) {
		int direction = rand() % 8 + 1; //pick a radom number for direction (8-connect for diagonal moves)
		switch (direction) {
		case 1: //UP
			if (isValid(currentNode.row - 1, currentNode.col)) {
				qNode move = createNode(currentNode.row - 1, currentNode.col, world[currentNode.row - 1][currentNode.col]);
				if ((move.val != 1) && (move.val != 0) && (move.val < currentNode.val)) {
					currentNode = path[pathCount] = move; //set current point to move
					pathCount++; //increment count of nodes in path
				}
			}
			break;
		case 2: //DOWN
			if (isValid(currentNode.row + 1, currentNode.col)) {
				qNode move = createNode(currentNode.row + 1, currentNode.col, world[currentNode.row + 1][currentNode.col]);
				if ((move.val != 1) && (move.val != 0) && (move.val < currentNode.val)) {
					currentNode = path[pathCount] = move; //set current point to move
					pathCount++; //increment count of nodes in path
				}
			}
			break;
		case 3: //RIGHT
			if (isValid(currentNode.row, currentNode.col + 1)) {
				qNode move = createNode(currentNode.row, currentNode.col + 1, world[currentNode.row][currentNode.col + 1]);
				if ((move.val != 1) && (move.val != 0) && (move.val < currentNode.val)) {
					currentNode = path[pathCount] = move; //set current point to move
					pathCount++; //increment count of nodes in path
				}
			}
			break;
		case 4: //LEFT
			if (isValid(currentNode.row, currentNode.col - 1)) {
				qNode move = createNode(currentNode.row, currentNode.col - 1, world[currentNode.row][currentNode.col - 1]);
				if ((move.val != 1) && (move.val != 0) && (move.val < currentNode.val)) {
					currentNode = path[pathCount] = move; //set current point to move
					pathCount++; //increment count of nodes in path
				}
			}
			break;
		}
	}
}

//isValid() determines if a point is within the bounds of the map
bool isValid(int row, int col) {
	if ((row >= 0) && (row < ROWS) && (col >= 0) && (col < COLUMNS))
		return true; //return true if new coords are within bounds
	else
		return false; //return false if new coords are not within bounds
}

//createNode() creates a new new node with given paremeters
Node createNode(int row, int col, int val) {
	qNode newNode;
	newNode.row = row;
	newNode.col = col;
	newNode.val = val;
	return newNode;
}


int main() {
	int world[ROWS][COLUMNS] = { 0 }; 

	int goalRow, goalCol; //goal coordinate integers
	Node path[100]; //array to store pathway
	int pathCount; //counter of how many steps in path

	//build world map with get goal point
	Build_World(world, goalRow, goalCol);

	//build wavefront world map
	Create_wavefront_map(world,goalRow,goalCol);

	//find a path
	Find_Path(world,path,pathCount);

	//output path
	cout << endl << "The number of steps from (0,0) to (" << goalRow << "," << goalCol << ") is " << pathCount << endl;
	cout << "The path is:" << endl << "\t(0,0)" << endl;
	for (int i = 0; i < pathCount; i++) {
		cout << '\t' << "(" << path[i].row << "," << path[i].col << ")" << endl;
	}
	cout << endl;

	system("PAUSE");
}

//Build_World() gets input world and goal point to generate a map
void Build_World(int world[][COLUMNS], int &goalRow, int &goalCol) {
	cout << "Enter World Name: ";
	string worldName;
	cin >> worldName;
	cout << endl;
	//worldName = "world1.dat";
	ifstream worldFile(worldName);

	//build world model walls from data
	for (int r = 0; r < ROWS; r++) {
		string line;
		getline(worldFile, line);
		stringstream ss(line);
		int point;
		for (int c = 0; c < COLUMNS; c++) {
			ss >> point;
			if (point == 1)
				world[r][c] = 1;
		}
	}

	//get valid goal point
	bool valid = false;
	while (!valid) {
		cout << "Enter Starting Point [ROW][COL]: ";
		cin >> goalRow >> goalCol; //get X Y of goal
		if (world[goalRow][goalCol] == 1) {
			cout << "ERROR: Wall Obstruction." << endl; //display error message if goal point is invalid
		}
		else {
			valid = true; 
		}
	}
}

//Create_wavefront_map() creates wavefront map 
void Create_wavefront_map(int world[][COLUMNS], int &goalRow, int &goalCol) {
	//initialize queue
	qNode queue[(ROWS*COLUMNS*4)]; //maximum slots needed for worst case
	int qCount = 0; //counter to keep track of nodes in que
	qNode currentNode = queue[0] = createNode(goalRow, goalCol, 2); // initialize currentNode & queue[0] to goal point
	qCount++;

	int currentIndex = 0;
	while ((currentNode.row != 0) || (currentNode.col != 0)) { //while not in goal point
		currentNode = queue[currentIndex]; 
		if ((world[currentNode.row][currentNode.col] == 0)) { 
			world[currentNode.row][currentNode.col] = currentNode.val; //set current point to wave value (previous + 1)
			//add above to queue
			if (isValid(currentNode.row - 1, currentNode.col)) {
				queue[qCount] = createNode(currentNode.row - 1, currentNode.col, currentNode.val + 1);
				qCount++;
			}
			//add below to queue
			if (isValid(currentNode.row + 1, currentNode.col)) {
				queue[qCount] = createNode(currentNode.row + 1, currentNode.col, currentNode.val + 1);
				qCount++;
			}
			//add right to queue
			if (isValid(currentNode.row, currentNode.col + 1)) {
				queue[qCount] = createNode(currentNode.row, currentNode.col + 1, currentNode.val + 1);
				qCount++;
			}
			//add left to queue
			if (isValid(currentNode.row, currentNode.col - 1)) {
				queue[qCount] = createNode(currentNode.row, currentNode.col - 1, currentNode.val + 1);
				qCount++;
			}
		}
		currentIndex++;
	}
}

//Find_Path() uses wavefront map to generate a list of moves to get to goal
void Find_Path(int world[][COLUMNS],qNode path[], int& pathCount) {
	qNode currentNode = createNode(0, 0, world[0][0]); //begin at point 0,0
	pathCount = 0;

	while (currentNode.val != 2) {
		int direction = rand() % 8 + 1; //pick a radom number for direction (8-connect for diagonal moves)
		switch (direction) {
		case 1: //UP
			if (isValid(currentNode.row - 1, currentNode.col)) {
				qNode move = createNode(currentNode.row - 1, currentNode.col, world[currentNode.row - 1][currentNode.col]);
				if ((move.val != 1) && (move.val != 0) && (move.val < currentNode.val)) { 
					currentNode = path[pathCount] = move; //set current point to move
					pathCount++; //increment count of nodes in path
				}
			}
			break;
		case 2: //DOWN
			if (isValid(currentNode.row + 1, currentNode.col)) {
				qNode move = createNode(currentNode.row + 1, currentNode.col, world[currentNode.row + 1][currentNode.col]);
				if ((move.val != 1) && (move.val != 0) && (move.val < currentNode.val)) {
					currentNode = path[pathCount] = move; //set current point to move
					pathCount++; //increment count of nodes in path
				}
			}
			break;
		case 3: //RIGHT
			if (isValid(currentNode.row, currentNode.col + 1)) {
				qNode move = createNode(currentNode.row, currentNode.col + 1, world[currentNode.row][currentNode.col + 1]);
				if ((move.val != 1) && (move.val != 0) && (move.val < currentNode.val)) { 
					currentNode = path[pathCount] = move; //set current point to move
					pathCount++; //increment count of nodes in path
				}
			}
			break;
		case 4: //LEFT
			if (isValid(currentNode.row, currentNode.col - 1)) {
				qNode move = createNode(currentNode.row, currentNode.col - 1, world[currentNode.row][currentNode.col - 1]);
				if ((move.val != 1) && (move.val != 0) && (move.val < currentNode.val)) {
					currentNode = path[pathCount] = move; //set current point to move
					pathCount++; //increment count of nodes in path
				}
			}
			break;
		}
	}
}

//isValid() determines if a point is within the bounds of the map
bool isValid(int row, int col) {
	if ((row >= 0) && (row < ROWS) && (col >= 0) && (col < COLUMNS))
		return true; //return true if new coords are within bounds
	else
		return false; //return false if new coords are not within bounds
}

//createNode() creates a new new node with given paremeters
Node createNode(int row, int col, int val) {
	qNode newNode;
	newNode.row = row;
	newNode.col = col;
	newNode.val = val;
	return newNode;
}