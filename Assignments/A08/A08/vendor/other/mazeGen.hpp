#include <list> 
/*
use a map to store all the possible vertexes in the maze 3x3=4x4 vertex multiplied by 2 for cieling 
                   - vertex position (x=col(>),y=heigh(v),z=row(^))
                   - if vertex is used
				   - index of the vertex
then read maze description and imagine creating the maze by repeating the vertices by adding to myList the vertexes needed in order (ex(1,0,0)->(1,1,0)->(0,0,1))
each vertex is stored as the value of its unique key if we start from top left and row by row first from floor than ceiling. 0-1-2
                                                                                                                             3-4-5
																															 6-7-8

in the end iterate over the list and for each vertex if it is the first time it is used (by check on map) add to vpos and store it's index in map, 
if it is already present reuse it as its index is stored in the map
*/

void Assignment08::createMazeMesh(int row, int col, char **maze) {
	
	struct MyStruct {
    	int row;
    	int col;
    	int high;
		bool used;
		int index;
	};
	
	int ccol = col+1;
	int rrow = row+1;
	int index = 0;
	std::list<int> myList;
	MyStruct value;
    std::map<int, MyStruct> myMap;

    //fill map with all possible vertices
    for (int level = 0; level <= 1; ++level) {
		for (int r = 0; r <= row; ++r) {
			for (int c = 0; c <= col; ++c) {
				//-1=vertex was not used yet
				MyStruct newEl = {r, c, level, false,-1};
				myMap[(ccol*rrow*level)+r*ccol+c] = newEl;		
			}
		}
	}
	
	//Start by adding to my list the base of the maze
	myList.push_back(0);myList.push_back(ccol-1);myList.push_back(ccol*rrow-ccol);
	myList.push_back(ccol-1);myList.push_back(ccol*rrow-ccol);myList.push_back(ccol*rrow-1);

	int start = -1;
	int end = -1;
	//add all remaining vertexes in order to the list (i do not care if they are repeated as they will be reused in next step)
	for (int r = 0; r < row; ++r) {
		start = -1;
		end = -1;
		for (int c = 0; c < col; ++c) {
			if(maze[r][c] == '#') {
				if(start == -1){
					start = c;
				}
			}
			if(maze[r][c] == ' ' || c==col-1) {
				end = c;
				if(c==col-1)	{
					end = c+1;
				}
				if(start != -1){

					//uses start and end so that if i have 3 walls in line ### i use only 2 triangles for the wall instead of 6 reducing the vertices used (start#end)

					int lowStart = start+((ccol)*r);
					int highStart = (ccol)*(rrow)+start+((ccol)*r);
					int lowEnd = end+((ccol)*r);
					int highEnd = (ccol)*(rrow)+end+((ccol)*r);
					int lowStartPlus = start+((ccol)*(r+1));
					int highStartPlus = (ccol)*(rrow)+start+((ccol)*(r+1));
					int lowEndPlus = end+((ccol)*(r+1));
					int highEndPlus = (ccol)*(rrow)+end+((ccol)*(r+1));

					// vIdx.push_back(lowStart); vIdx.push_back(lowEnd); vIdx.push_back(highStart);	
					// vIdx.push_back(lowEnd); vIdx.push_back(highStart); vIdx.push_back(highEnd);
					// vIdx.push_back(lowStartPlus); vIdx.push_back(lowEndPlus); vIdx.push_back(highStartPlus);	
					// vIdx.push_back(lowEndPlus); vIdx.push_back(highStartPlus); vIdx.push_back(highEndPlus);
					// vIdx.push_back(lowStart); vIdx.push_back(lowStartPlus); vIdx.push_back(highStart);
					// vIdx.push_back(lowStartPlus); vIdx.push_back(highStart); vIdx.push_back(highStartPlus);
					// vIdx.push_back(lowEnd);vIdx.push_back(lowEndPlus);vIdx.push_back(highEnd);
					// vIdx.push_back(lowEndPlus);vIdx.push_back(highEnd);vIdx.push_back(highEndPlus);
					// vIdx.push_back(highStart);vIdx.push_back(highEnd);vIdx.push_back(highStartPlus);
					// vIdx.push_back(highEnd);vIdx.push_back(highStartPlus);vIdx.push_back(highEndPlus);

                    // + +     start   -   end
					// + +   startPlus - endPlus      low=floor,high=ceiling
                    //front walls backside 
					myList.push_back(lowStart); myList.push_back(lowEnd); myList.push_back(highStart);	
					myList.push_back(lowEnd); myList.push_back(highStart); myList.push_back(highEnd);
                    //front walls front side
					myList.push_back(lowStartPlus); myList.push_back(lowEndPlus); myList.push_back(highStartPlus);	
					myList.push_back(lowEndPlus); myList.push_back(highStartPlus); myList.push_back(highEndPlus);
                    //lateral wall left
					myList.push_back(lowStart); myList.push_back(lowStartPlus); myList.push_back(highStart);
					myList.push_back(lowStartPlus); myList.push_back(highStart); myList.push_back(highStartPlus);
                    //lateral wall right
					myList.push_back(lowEnd);myList.push_back(lowEndPlus);myList.push_back(highEnd);
					myList.push_back(lowEndPlus);myList.push_back(highEnd);myList.push_back(highEndPlus);
                    //ceiling
					myList.push_back(highStart);myList.push_back(highEnd);myList.push_back(highStartPlus);
					myList.push_back(highEnd);myList.push_back(highStartPlus);myList.push_back(highEndPlus);

					start = -1;
					end = -1;
				}
			}	
		}
	}
    
    //
	for (const int& key : myList) {
		auto it = myMap.find(key);
		if (it != myMap.end()) {
			// Return the MyStruct value associated with the key
			value = it->second;
		} else {
			throw std::out_of_range("Key not found in the map");
		}
		if(value.used == false){
			//if never used add the pos for the vertex and add the new vIdx (start from zero) then increment for next vIdx
			vPos.push_back(myMap[key].col); vPos.push_back(myMap[key].high); vPos.push_back(myMap[key].row);	
			myMap[key].index = index;
			vIdx.push_back(myMap[key].index);
			myMap[key].used=true;
			index++;
		}
		else if(value.used == true){
			//if already present just add the known vIdx
			vIdx.push_back(myMap[key].index);
		}
	}

	// // Fill array vPos with the positions of the vertices of the mesh
	// vPos.push_back(0.0); vPos.push_back(0.0); vPos.push_back(0.0);	// vertex 0
	// vPos.push_back(1.0); vPos.push_back(0.0); vPos.push_back(0.0);	// vertex 1
	// vPos.push_back(0.0); vPos.push_back(1.0); vPos.push_back(0.0);	// vertex 2
	// vPos.push_back(1.0); vPos.push_back(1.0); vPos.push_back(0.0);	// vertex 3

	// // Fill the array vIdx with the indices of the vertices of the triangles
	// vIdx.push_back(0); vIdx.push_back(1); vIdx.push_back(2);	// First triangle
	// vIdx.push_back(1); vIdx.push_back(2); vIdx.push_back(3);	// Second triangle
}




