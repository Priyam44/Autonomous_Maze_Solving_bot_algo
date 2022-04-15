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
//printing maze
void printmaze(cellclass** maze, int n, int m,cellclass* currcell=NULL,char direction=' ') {
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
			if(currcell!=NULL){
                if(i==currcell->x && j== currcell->y){
                    if(direction=='n'){
                        cout<<"^";
                    }
                    else if(direction=='s'){
                        cout<<"*";
                    }
                    else if(direction=='e'){
                        cout<<">";
                    }
                    else if(direction=='w'){
                        cout<<"<";
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
	tmaze[1][2].west = false; tmaze[1][1].east = false;
	tmaze[2][0].east = false; tmaze[2][1].west = false;
	tmaze[2][1].south = true; tmaze[3][1].north = true;
	tmaze[2][2].south = true; tmaze[3][2].north = true;
	tmaze[2][2].east = true; tmaze[2][3].west = true;
	tmaze[2][3].south = true; tmaze[3][3].north = true;
	tmaze[4][1].east = true; tmaze[4][2].west = true;
	tmaze[4][2].north = true; tmaze[3][2].south = true;
	tmaze[4][4].north = true; tmaze[3][4].south = true;
    tmaze[3][0].east=true;tmaze[3][1].west=true;
    tmaze[2][0].south=false;tmaze[3][0].north=false;
    tmaze[1][0].south=true; tmaze[2][0].north=true;
	return tmaze;



}
//no wall:false wall:true
//moving to the new cell
void move(char & curr_direction, char & nextcell_direction, cellclass& currcell, cellclass ** maze) {
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
	curr_direction=nextcell_direction;

}
//------------------------------------------------------------------------------------------------------------------------------------------------
//updating cell values using stack method
void update_cell_values(cellclass* currcell, cellclass ** maze) {
	queue<cellclass*> oldcells;
	oldcells.push(currcell);
	while (!oldcells.empty()) {
		cellclass* element = oldcells.front();
		oldcells.pop();
		const int x = element->x;
		const int y = element->y;
		int _min = INT_MAX;
		if (!element->west) {
			if (_min>maze[x][y - 1].cellvalue) {
				_min = maze[x][y - 1].cellvalue;
			}
		}
		if (!element->east) {
			if (_min>maze[x][y + 1].cellvalue) {
				_min = maze[x][y + 1].cellvalue;
			}
		}
		if (!element->north) {
			if (_min>maze[x - 1][y].cellvalue) {
				_min = maze[x - 1][y].cellvalue;
			}
		}
		if (!element->south) {
			if (_min>maze[x + 1][y].cellvalue) {
				_min = maze[x + 1][y].cellvalue;
			}
		}
		if (element->cellvalue != _min + 1) {
			element->cellvalue = _min + 1;
			if (!element->west) {
				oldcells.push(&maze[x][y - 1]);
			}
			if (!element->east) {
				oldcells.push(&maze[x][y + 1]);
			}
			if (!element->north) {
				oldcells.push(&maze[x - 1][y]);
			}
			if (!element->south) {
				oldcells.push(&maze[x + 1][y]);
			}
		}
	}

}
//------------------------------------------------------------------------------------------------------------------------------------------------
bool wall_update(cellclass& element, char direction, cellclass** maze, cellclass ** targetmaze) {
	bool wall_discovered = false;
	if (direction=='n') {

		if (targetmaze[element.x][element.y].north) {
            wall_discovered = true;
			element.north = true;
		}
		if (targetmaze[element.x][element.y].west) {
		    wall_discovered = true;
			element.west = true;
		}
		if (targetmaze[element.x][element.y].east) {
		    wall_discovered = true;
			element.east = true;
		}


	}
	else if (direction=='s') {

		if (targetmaze[element.x][element.y].south) {
            wall_discovered = true;
			element.south = true;
		}
		if (targetmaze[element.x][element.y].west) {
		    wall_discovered = true;
			element.west = true;
		}
		if (targetmaze[element.x][element.y].east) {
		    wall_discovered = true;
			element.east = true;
		}


	}
	if (direction=='e') {

		if (targetmaze[element.x][element.y].north) {
            wall_discovered = true;
			element.north = true;
		}
		if (targetmaze[element.x][element.y].south) {
		    wall_discovered = true;
			element.south = true;
		}
		if (targetmaze[element.x][element.y].east) {
		    wall_discovered = true;
			element.east = true;
		}


	}
	if (direction=='w') {

		if (targetmaze[element.x][element.y].north) {
            wall_discovered = true;
			element.north = true;
		}
		if (targetmaze[element.x][element.y].west) {
		    wall_discovered = true;
			element.west = true;
		}
		if (targetmaze[element.x][element.y].south) {
		    wall_discovered = true;
			element.south = true;
		}


	}

	return wall_discovered;
}
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
	//creating maze considering no walls
	bool** visited=new bool* [n];
	for (int i = 0; i<n; i++) {
		visited[i] = new bool[m];
		for (int j = 0; j<m; j++) {
			visited[i][j] = false;
		}
	}
	queue<cellclass> nowallqu;
     int ea,eb;
	cout<<"enter ending coordinates"<<endl;
	cin>>ea>>eb;
	nowallqu.push(maze[ea][eb]);
	visited[ea][eb]=true;
	while(!nowallqu.empty()){
        cellclass currpos=nowallqu.front();
        nowallqu.pop();

            if(currpos.x-1 >=0){
                if(!visited[currpos.x-1][currpos.y]){
                    maze[currpos.x-1][currpos.y].cellvalue=maze[currpos.x][currpos.y].cellvalue+1;
                    visited[currpos.x-1][currpos.y]=true;

                    nowallqu.push(maze[currpos.x-1][currpos.y]);
                }
            }
            if(currpos.x+1 <n){
                if(!visited[currpos.x+1][currpos.y]){
                    maze[currpos.x+1][currpos.y].cellvalue=maze[currpos.x][currpos.y].cellvalue+1;
                    visited[currpos.x+1][currpos.y]=true;
                    nowallqu.push(maze[currpos.x+1][currpos.y]);
                }
            }
            if(currpos.y-1 >=0){
                if(!visited[currpos.x][currpos.y-1]){
                    maze[currpos.x][currpos.y-1].cellvalue=maze[currpos.x][currpos.y].cellvalue+1;
                    visited[currpos.x][currpos.y-1]=true;
                    nowallqu.push(maze[currpos.x][currpos.y-1]);
                }
            }
            if(currpos.y+1 <m){
                if(!visited[currpos.x][currpos.y+1]){
                    maze[currpos.x][currpos.y+1].cellvalue=maze[currpos.x][currpos.y].cellvalue+1;
                    visited[currpos.x][currpos.y+1]=true;
                    nowallqu.push(maze[currpos.x][currpos.y+1]);
                }
            }

	}
	
	printmaze(maze, n, m,NULL);

	//-------------------------------------
	cellclass ** targetmaze = createmaze(n, m);
	printmaze(targetmaze, n, m,NULL);
	//-------------------------------------
	//creating visited array using previously visited array
	for (int i = 0; i<n; i++) {
		for (int j = 0; j<m; j++) {
			visited[i][j] = false;
		}
	}
	//-------------------------------------
	int sa,sb;
	cout<<"enter starting  position"<<endl;
	cin>>sa>>sb;
	cellclass currcell = maze[sa][sb];
	char direction = 'n';
	bool wall_discovered =false;
	while (maze[currcell.x][currcell.y].cellvalue != 0) {
		if (!visited[currcell.x][currcell.y]) {
			//updating walls
			wall_discovered = wall_update(maze[currcell.x][currcell.y], direction, maze, targetmaze);
			if (wall_discovered) {
				//updating cell values
				update_cell_values(&maze[currcell.x][currcell.y], maze);
			}
		}
		//checking minimum accessible neighbour
		int _min = INT_MAX;
		char nextcell_direction = ' ';
		if (!maze[currcell.x][currcell.y].west) {
			if (_min>maze[currcell.x][currcell.y - 1].cellvalue) {
				_min = maze[currcell.x][currcell.y - 1].cellvalue;
				nextcell_direction = 'w';
			}
		}
		if (!maze[currcell.x][currcell.y].east) {
			if (_min>maze[currcell.x][currcell.y + 1].cellvalue) {
				_min = maze[currcell.x][currcell.y + 1].cellvalue;
				nextcell_direction = 'e';
			}
		}
		if (!maze[currcell.x][currcell.y].north) {
			if (_min>maze[currcell.x - 1][currcell.y].cellvalue) {
				_min = maze[currcell.x - 1][currcell.y].cellvalue;
				nextcell_direction = 'n';
			}
		}
		if (!maze[currcell.x][currcell.y].south) {
			if (_min>maze[currcell.x + 1][currcell.y].cellvalue) {
				_min = maze[currcell.x + 1][currcell.y].cellvalue;
				nextcell_direction = 's';
			}
		}
		printmaze(maze, n, m,&currcell,direction);
		//moving to minimum accesible neighbour
		visited[currcell.x][currcell.y] = true;
		move(direction, nextcell_direction, currcell, maze);




	}
	printmaze(maze,n,m,&currcell);
	cout << "-------------------------------------------------------------------------------------" << endl;
	

	for (int i = 0; i<n; i++) {
		delete[] maze[i];
	}delete[] maze;
	for (int i = 0; i<n; i++) {
		delete[] targetmaze[i];
		delete[] visited[i];
	}delete[] targetmaze;
	delete[] visited;
	//remember to delete all the heap cells

}
