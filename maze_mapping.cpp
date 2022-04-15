#include <iostream>
#include <climits>
using namespace std;
#include <stack>
#include <queue>
class cellclass {
public:
	int cellvalue = 0, x, y;
	bool west = false, south = false, north = false, east = false;

};
class block_prop {
public:
	int x, y, blockvalue;
	bool found = false;
	char relative_direction; //direction corresponding to the block from where it is found
};
//printing maze
void printmaze(cellclass** maze, int n, int m, cellclass* currcell = NULL, char direction = ' ') {
	for (int i = 0; i<n; i++) {
		for (int j = 0; j<m; j++) {
			if (maze[i][j].north) {
				cout << "  ---";
			}
			else {
				cout << "     ";
			}
		}cout << endl;
		for (int j = 0; j<m; j++) {
			if (maze[i][j].west) {

				cout << "|";
			}
			else {
				cout << " ";
			}
			if (maze[i][j].cellvalue < 10) {
				cout << " ";
			}
			cout << maze[i][j].cellvalue << " ";
			if (currcell != NULL) {
				if (i == currcell->x && j == currcell->y) {
					if (direction == 'n') {
						cout << "^";
					}
					else if (direction == 's') {
						cout << "*";
					}
					else if (direction == 'e') {
						cout << ">";
					}
					else if (direction == 'w') {
						cout << "<";
					}
				}
			}
			if (maze[i][j].east) {
				cout << "|";
			}
			else {
				cout << " ";
			}
		}

		cout << endl;
	}
	for (int j = 0; j<m; j++) {
		if (maze[n - 1][j].south) {
			cout << "  ---";
		}
		else {
			cout << "      ";
		}
	}cout << endl;

}
//creating target maze
cellclass** createmaze(int n, int m) {
	cellclass** tmaze = new cellclass*[n];
	//creating maze
	for (int i = 0; i<n; i++) {
		tmaze[i] = new cellclass[m];
		for (int j = 0; j<m; j++) {
			cellclass cell;
			tmaze[i][j] = cell;
			cell.x = i;
			cell.y = j;
		}
	}
	//setting boundary walls
	for (int i = 0; i<n; i++) {
		tmaze[i] = new cellclass[m];
		for (int j = 0; j<m; j++) {
			cellclass cell;
			tmaze[i][j] = cell;
			tmaze[i][j].x = i;
			tmaze[i][j].y = j;
			if (i == 0 && j == 0) {
				tmaze[i][j].north = true;
				tmaze[i][j].west = true;
			}
			if (i == 0 && j != 0) {
				tmaze[i][j].north = true;
			}
			if (i != 0 && j == 0) {
				tmaze[i][j].west = true;
			}
			if (i == n - 1 && j == m - 1) {
				tmaze[i][j].south = true;
				tmaze[i][j].east = true;
			}
			if (i == n - 1 && j != m - 1) {
				tmaze[i][j].south = true;
			}
			if (i != n - 1 && j == m - 1) {
				tmaze[i][j].east = true;
			}
		}
	}
	//creating internal walls
	tmaze[0][0].east = true; tmaze[0][1].west = true;
	tmaze[0][2].east = true; tmaze[0][3].west = true;
	tmaze[0][2].south = true; tmaze[1][2].north = true;
	tmaze[1][2].west = true; tmaze[1][1].east = true;
	tmaze[2][0].east = true; tmaze[2][1].west = true;
	tmaze[2][1].south = true; tmaze[3][1].north = true;
	tmaze[2][2].south = true; tmaze[3][2].north = true;
	tmaze[2][2].east = true; tmaze[2][3].west = true;
	tmaze[2][3].south = true; tmaze[3][3].north = true;
	tmaze[4][1].east = true; tmaze[4][2].west = true;
	tmaze[4][2].north = true; tmaze[3][2].south = true;
	tmaze[4][4].north = true; tmaze[3][4].south = true;
	tmaze[3][0].east = true; tmaze[3][1].west = true;
	tmaze[2][0].south = true; tmaze[3][0].north = true;
	/*tmaze[1][2].cellvalue=1;tmaze[1][2].south = true; tmaze[2][2].north = true;tmaze[1][2].east = true; tmaze[1][2].west = true;
	tmaze[0][0].cellvalue=2;tmaze[0][0].south = true; tmaze[1][0].north = true;
	tmaze[3][0].cellvalue=1;tmaze[3][0].south = true; tmaze[4][0].north = true;*/
	return tmaze;
	//setting blocks



}
//no wall:false wall:true
//moving to the new cell
void _move(char & curr_direction, char & nextcell_direction, cellclass& currcell, cellclass ** maze) {
	if (nextcell_direction == 'n') {
		currcell = maze[currcell.x - 1][currcell.y];
	}
	else if (nextcell_direction == 'e') {
		currcell = maze[currcell.x][currcell.y + 1];
	}
	else if (nextcell_direction == 's') {
		currcell = maze[currcell.x + 1][currcell.y];
	}
	else if (nextcell_direction == 'w') {
		currcell = maze[currcell.x][currcell.y - 1];
	}
	//changing direction
	curr_direction = nextcell_direction;

}
//------------------------------------------------------------------------------------------------------------------------------------------------
bool walls_visited(cellclass** maze, block_prop* blocks, bool ** visited, int n, int m) {
	for (int i = 0; i<n; i++) {
		if (i % 2 == 0) {
			for (int j = 0; j<m; j += 2) {
				if (!visited[i][j]) {
					if ((i - 1 >= 0 && !visited[i - 1][j]) || (i + 1<n && !visited[i + 1][j]) || (j - 1 >= 0 && !visited[i][j - 1]) || (j + 1 < m && !visited[i][j + 1])) {
						return false;
					}
				}
			}
		}


		else if (i % 2 != 0) {
			for (int j = 1; j<m; j += 2) {
				if (!visited[i][j]) {
					if ((i - 1 >= 0 && !visited[i - 1][j]) || (i + 1<n && !visited[i + 1][j]) || (j - 1 >= 0 && !visited[i][j - 1]) || (j + 1 < m && !visited[i][j + 1])) {
						return false;
					}
				}
			}
		}
	}
	return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

//evaluating block number
int block_found2(int x, int y, cellclass** targetmaze, block_prop * blocks, char rel_dir) {
	if (targetmaze[x][y].cellvalue == 1) {
		if (!blocks[0].found) {
			blocks[0].found = true; blocks[0].x = x; blocks[0].y = y; blocks[0].relative_direction = rel_dir;
			return 1;
		}
		else if (!blocks[1].found) {
			blocks[1].found = true; blocks[1].x = x; blocks[1].y = y; blocks[1].relative_direction = rel_dir;
			return 2;
		}
		else if (!blocks[3].found) {
			blocks[3].found = true; blocks[3].x = x; blocks[3].y = y; blocks[3].relative_direction = rel_dir;
			return 4;
		}

	}
	else if (targetmaze[x][y].cellvalue == 2) {
		if (!blocks[2].found) {
			blocks[2].found = true; blocks[2].x = x; blocks[2].y = y; blocks[2].relative_direction = rel_dir;
			return 3;
		}
		else if (!blocks[4].found) {
			blocks[4].found = true; blocks[4].x = x; blocks[4].y = y; blocks[4].relative_direction = rel_dir;
			return 5;
		}
	}
	return 0;
}
int block_found(const cellclass element, char direction, cellclass** targetmaze, block_prop * blocks, int n, int m) {
	if (direction == 'n' && element.x - 1 >= 0) {
		return block_found2(element.x - 1, element.y, targetmaze, blocks, direction);
	}
	else if (direction == 's' && element.x + 1<n) {
		return block_found2(element.x + 1, element.y, targetmaze, blocks, direction);

	}
	else if (direction == 'e' && element.y + 1 <m) {
		return block_found2(element.x, element.y + 1, targetmaze, blocks, direction);
	}
	else if (direction == 'w' && element.y - 1 >= 0) {
		return block_found2(element.x, element.y - 1, targetmaze, blocks, direction);

	}
	return 0;

}
//------------------------------------------------------------------------------------------------------------------------------------------------
//updating walls and checking for blocks
int wall_update(cellclass* element, char direction, cellclass** maze, cellclass ** targetmaze, block_prop* blocks, const int& n, const int& m) {
	int block_discovered;
	bool wall_discovered;
	if (direction == 'n') {

		if (targetmaze[element->x][element->y].north) {
			wall_discovered = true;
			block_discovered = block_found(*element, 'n', targetmaze, blocks, n, m);
			if(block_discovered == 0) element->north = true;
            if (element->x - 1 >= 0) {
				maze[element->x - 1][element->y].south = true;
			}
		}
		if (targetmaze[element->x][element->y].west) {
			wall_discovered = true;
			block_discovered = block_found(*element, 'w', targetmaze, blocks, n, m);
			if(block_discovered == 0) element->west = true;
			if (element->y - 1 >= 0) {
				maze[element->x][element->y - 1].east = true;
			}

		}
		if (targetmaze[element->x][element->y].east) {
			wall_discovered = true;
			block_discovered = block_found(*element, 'e', targetmaze, blocks, n, m);
			if(block_discovered == 0) element->east = true;
			if (element->y + 1<m) {
				maze[element->x][element->y + 1].west = true;
			}
		}



	}
	else if (direction == 's') {

		if (targetmaze[element->x][element->y].south) {
			wall_discovered = true;
			block_discovered = block_found(*element, 's', targetmaze, blocks, n, m);
			if(block_discovered == 0) element->south = true;
			if (element->x + 1<n) {
				maze[element->x + 1][element->y].north = true;
			}
		}
		if (targetmaze[element->x][element->y].west) {
			wall_discovered = true;
			block_discovered = block_found(*element, 'w', targetmaze, blocks, n, m);
			if(block_discovered == 0) element->west = true;
			if (element->y - 1 >= 0) {
				maze[element->x][element->y - 1].east = true;
			}

		}
		if (targetmaze[element->x][element->y].east) {
			wall_discovered = true;
			block_discovered = block_found(*element, 'e', targetmaze, blocks, n, m);
			if(block_discovered == 0) element->east = true;
			if (element->y + 1<m) {
				maze[element->x][element->y + 1].west = true;
			}
		}


	}
	else if (direction == 'e') {

		if (targetmaze[element->x][element->y].north) {
			wall_discovered = true;
			block_discovered = block_found(*element, 'n', targetmaze, blocks, n, m);
			if(block_discovered == 0) element->north = true;
			if (element->x - 1 >= 0) {
				maze[element->x - 1][element->y].south = true;
			}
		}
		if (targetmaze[element->x][element->y].south) {
			wall_discovered = true;
			block_discovered = block_found(*element, 's', targetmaze, blocks, n, m);
			if(block_discovered == 0) element->south = true;
			if (element->x + 1<n) {
				maze[element->x + 1][element->y].north = true;
			}
		}
		if (targetmaze[element->x][element->y].east) {
			wall_discovered = true;
			block_discovered = block_found(*element, 'e', targetmaze, blocks, n, m);
			if(block_discovered == 0) element->east = true;
			if (element->y + 1<m) {
				maze[element->x][element->y + 1].west = true;
			}
		}


	}
	else if (direction == 'w') {

		if (targetmaze[element->x][element->y].north) {
			wall_discovered = true;
			block_discovered = block_found(*element, 'n', targetmaze, blocks, n, m);
			if(block_discovered == 0) element->north = true;
			if (element->x - 1 >= 0) {
				maze[element->x - 1][element->y].south = true;
			}
		}
		if (targetmaze[element->x][element->y].west) {
			wall_discovered = true;
			block_discovered = block_found(*element, 'w', targetmaze, blocks, n, m);
			if(block_discovered == 0) element->west = true;
			if (element->y - 1 >= 0) {
				maze[element->x][element->y - 1].east = true;
			}
		}
		if (targetmaze[element->x][element->y].south) {
			wall_discovered = true;
			block_discovered = block_found(*element, 's', targetmaze, blocks, n, m);
			if(block_discovered == 0) element->south = true;
			if (element->x + 1<n) {
				maze[element->x + 1][element->y].north = true;
			}
		}


	}

	return 0;
}

//------------------------------------------------------------------------------------------------------------------------------------------------
//recursive method to map the maze......prioritizing direction
bool step(cellclass currcell, char direction, cellclass** maze, cellclass** targetmaze, bool** visited, block_prop* blocks, const int& n, const int& m) ;

bool stephelper(cellclass currcell, char direction, cellclass** maze, cellclass** targetmaze, bool** visited, block_prop* blocks, const int& n, const int& m) {
	bool path_found;
	if (direction == 'n') {
		if (currcell.x - 1 >= 0 && !visited[currcell.x - 1][currcell.y]){

			path_found = step(maze[currcell.x - 1][currcell.y], 'n', maze, targetmaze, visited, blocks, n, m);
		    if (path_found) return true;
    		else {
    			printmaze(maze, n, m, &currcell, 's');
    			return false;
		}
	}
}
	else if (direction == 's') {
			if (currcell.x + 1 <n && !visited[currcell.x + 1][currcell.y]) {
                path_found = step(maze[currcell.x + 1][currcell.y], 's', maze, targetmaze, visited, blocks, n, m);
				if (path_found) return true;
				else {
					printmaze(maze, n, m, &currcell, 'n');
					return false;
				}
			}
    }
	else if (direction == 'w') {
		if (currcell.y - 1 >= 0 && !visited[currcell.x][currcell.y - 1]) {

			path_found = step(maze[currcell.x][currcell.y - 1], 'w', maze, targetmaze, visited, blocks, n, m);
			if (path_found) return true;
			else {
				printmaze(maze, n, m, &currcell, 'e');
				return false;
			}
		}

	}
	else if (direction == 'e') {
		if (currcell.y + 1 < m && !visited[currcell.x][currcell.y + 1]) {
			path_found = step(maze[currcell.x][currcell.y + 1], 'e', maze, targetmaze, visited, blocks, n, m);
			if (path_found) return true;
			else {
				printmaze(maze, n, m, &currcell, 'w');
				return false;
			}
		}

	}
	return false;
}
bool step(cellclass currcell, char direction, cellclass** maze, cellclass** targetmaze, bool** visited, block_prop* blocks, const int& n, const int& m) {

	if (walls_visited(maze, blocks, visited, n, m)) {
		return true;
	}
	bool path_found;
	int block = wall_update(&maze[currcell.x][currcell.y], direction, maze, targetmaze, blocks, n, m);
	currcell = maze[currcell.x][currcell.y];
	visited[currcell.x][currcell.y] = true;
	printmaze(maze, n, m, &currcell, direction);
	if (block != 0) {
		// if block1 or block4 or block 5 or block 2 such that block 3 is not found
		maze[blocks[block - 1].x][blocks[block - 1].y].cellvalue = block;
		if (block == 1 || block == 4 || (block == 2 && !blocks[2].found)) {
			//wait till block removed
			targetmaze[blocks[block - 1].x][blocks[block - 1].y].cellvalue = 0;
			path_found = step(maze[blocks[block-1].x][blocks[block-1].y], blocks[block - 1].relative_direction, maze, targetmaze, visited, blocks, n, m);
			if (path_found) return true;
		}
		//if block 2 found and block3 not found
		else if (block == 2 && blocks[2].found) {
			targetmaze[blocks[block - 1].x][blocks[block - 1].y].cellvalue = 0;
            return false;
		}
		// if block3 found and block 2 not found
		else if (block == 3 && !blocks[1].found) {
			//moving bot in direction other than the block direction
			targetmaze[blocks[block - 1].x][blocks[block - 1].y].cellvalue = 0;

			if (blocks[2].relative_direction != 'n' && !currcell.north) {
				if (stephelper(currcell, 'n', maze, targetmaze, visited, blocks, n, m)) return true;
				else if (blocks[1].found) {
					if (stephelper(currcell, blocks[1].relative_direction, maze, targetmaze, visited, blocks, n, m)) return true;
				}
			}
            if (blocks[2].relative_direction != 'e' && !currcell.east) {
				if (stephelper(currcell, 'e', maze, targetmaze, visited, blocks, n, m)) return true;
				else if (blocks[1].found) {
					stephelper(currcell, blocks[1].relative_direction, maze, targetmaze, visited, blocks, n, m);
				}
			}
			if (blocks[2].relative_direction != 'w' && !currcell.west) {
				if (stephelper(currcell, 'w', maze, targetmaze, visited, blocks, n, m)) return true;
				else if (blocks[1].found) {
					stephelper(currcell, blocks[1].relative_direction, maze, targetmaze, visited, blocks, n, m);
				}
			}
            if (blocks[2].relative_direction != 's' && !currcell.south) {
				if (stephelper(currcell, 's', maze, targetmaze, visited, blocks, n, m)) return true;
				else if (blocks[1].found) {
					stephelper(currcell, blocks[1].relative_direction, maze, targetmaze, visited, blocks, n, m);
				}
			}

		}

	}

	if (!currcell.north) {
		if (stephelper(currcell, 'n', maze, targetmaze, visited, blocks, n, m)) return true;
	}
	if (!currcell.east) {

		if (stephelper(currcell, 'e', maze, targetmaze, visited, blocks, n, m)) return true;

	}
	if (!currcell.west) {

		if (stephelper(currcell, 'w', maze, targetmaze, visited, blocks, n, m)) return true;

	}
	if (!currcell.south) {

		if (stephelper(currcell, 's', maze, targetmaze, visited, blocks, n, m)) return true;

	}
	//add moving back function
	return false;
}

//------------------------------------------------------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------------------------------------

int main()
{
	int n, m;
	cout << "enter rows and column" << endl;
	cin >> n >> m;
	cellclass** maze = new cellclass*[n];
	//creating maze and setting boundary wall;
	for (int i = 0; i<n; i++) {
		maze[i] = new cellclass[m];
		for (int j = 0; j<m; j++) {
			cellclass cell;
			maze[i][j] = cell;
			maze[i][j].x = i;
			maze[i][j].y = j;
			if (i == 0 && j == 0) {
				maze[i][j].north = true;
				maze[i][j].west = true;
			}
			if (i == 0 && j != 0) {
				maze[i][j].north = true;
			}
			if (i != 0 && j == 0) {
				maze[i][j].west = true;
			}
			if (i == n - 1 && j == m - 1) {
				maze[i][j].south = true;
				maze[i][j].east = true;
			}
			if (i == n - 1 && j != m - 1) {
				maze[i][j].south = true;
			}
			if (i != n - 1 && j == m - 1) {
				maze[i][j].east = true;
			}
		}
	}
	//-------------------------------------
	cellclass ** targetmaze = createmaze(n, m);
	printmaze(targetmaze, n, m, NULL);
	//-------------------------------------
	//creating visited array using previously visited array
	bool ** visited = new bool*[n];
	for (int i = 0; i<n; i++) {
		visited[i] = new bool[m];
		for (int j = 0; j<m; j++) {
			visited[i][j] = false;
		}
	}
	//-------------------------------------
	int sa, sb;
	cout << "enter starting  position" << endl;
	cin >> sa >> sb;
	cellclass currcell = maze[sa][sb];
	char direction = 'n';
	block_prop* blocks = new block_prop[5];
	for (int i = 0; i<5; i++) {
		blocks[i].x = 0;
		blocks[i].y = 0;
	}
	bool _final = step(currcell, direction, maze, targetmaze, visited, blocks, n, m);
	printmaze(maze, n, m, &currcell);
	cout << "-------------------------------------------------------------------------------------" << endl;
    for(int i=0;i<5;i++){
        if(blocks[i].found){
            cout<<blocks[i].x<<' '<<blocks[i].y<<endl;
        }
    }
    printmaze(targetmaze,n,m);

	for (int i = 0; i<n; i++) {
		delete[] maze[i];
	}delete[] maze;
	for (int i = 0; i<n; i++) {
		delete[] targetmaze[i];
		delete[] visited[i];
	}delete[] targetmaze;
	delete[] visited;
	delete[]blocks;
	//remember to delete all the heap cells

}
