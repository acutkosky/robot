

#include "floodfill.h"
//#include<iostream>
//#include<cstdlib>
//using std::cout;

typedef unsigned int uint;

Maze::Maze(void) {

  for(int x=0;x<GRID_SIZE;x++) {
    for(int y=0;y<GRID_SIZE;y++) {
      grid[x][y].walls_visits = 0;
      grid[x][y].distance = 127;
    }
  }
  grid[GRID_SIZE/2][GRID_SIZE/2].distance = 0;
  grid[GRID_SIZE/2][(GRID_SIZE-1)/2].distance = 0;
  grid[(GRID_SIZE-1)/2][(GRID_SIZE-1)/2].distance = 0;
  grid[(GRID_SIZE-1)/2][(GRID_SIZE)/2].distance = 0;
  /*
      char xdist,ydist;
      if(x>=GRID_SIZE/2) {
	xdist = x-GRID_SIZE/2;
      } else {
	xdist = (GRID_SIZE-1)/2 - x;
      }

      if(y>=GRID_SIZE/2) {
	ydist = y-GRID_SIZE/2;
      } else {
	ydist = (GRID_SIZE-1)/2 - y;
      }

      grid[x][y].distance = xdist+ydist;//MIN(ABS(x-GRID_SIZE/2),ABS(x-(GRID_SIZE-1)/2))+MIN(ABS(y-GRID_SIZE/2),ABS(y-(GRID_SIZE-1)/2))+'a';
    }
  }
  */
  for(int i=0;i<GRID_SIZE;i++) {
    ADD_WALL((grid[i][0].walls_visits),NORTH);
    ADD_WALL((grid[i][GRID_SIZE-1].walls_visits),SOUTH);
  }

  for(int i=0;i<GRID_SIZE;i++) {
    ADD_WALL((grid[GRID_SIZE-1][i].walls_visits),EAST);
    ADD_WALL((grid[0][i].walls_visits),WEST);
  }
  Flood_Fill(0,0);

}

void Maze::Reset_Goal(char x, char y) {
  for(int i=0;i<GRID_SIZE;i++) {
    for(int j=0;j<GRID_SIZE;j++) {
      grid[i][j].distance = 127;
      //set visits to 0
      //grid[i][j].walls_visits &= WALLS_MASK;
    }
  }
  grid[x][y].distance = 0;
  Flood_Fill(0,0);
}

char Maze::Update_Distance(char x, char y) {
  
  
  char old_distance = grid[x][y].distance;
  
  /*are we at the center yet?*/
  if(old_distance == 0) {
    return 1;
  }
  
  char best_distance = 255;
  char walls = grid[x][y].walls_visits;
  //  cout<<"x: "<<(int)x<<"y: "<<(int)y<<"\n";  
  if(!NORTH_WALL(walls)) {
    if((uint)best_distance > (uint)grid[x][y-1].distance)
      best_distance = grid[x][y-1].distance;
  }// else {cout<<"there is a north wall\n";}

  //cout<<"after NORTH: "<<(uint) best_distance<<"\n";

  if(!SOUTH_WALL(walls)) {
    if((uint)best_distance > (uint)grid[x][y+1].distance)
      best_distance = grid[x][y+1].distance;
  }// else {cout<<"there is a south wall\n";}
  //cout<<"after SOUTH: "<<(uint) best_distance<<"\n";




  if(!EAST_WALL(walls)) {
    if((uint)best_distance > (uint)grid[x+1][y].distance)
      best_distance = grid[x+1][y].distance;
  }// else {cout<<"there is a east wall\n";}
  //cout<<"after EAST: "<<(uint) best_distance<<"\n";



  if(!WEST_WALL(walls)) {
    if((uint)best_distance > (uint)grid[x-1][y].distance)
      best_distance = grid[x-1][y].distance;
  }// else {cout<<"there is a west wall\n";}
  //cout<<"after WEST: "<<(uint) best_distance<<"\n";


  grid[x][y].distance = best_distance+1;
  return (best_distance+1 == old_distance);

}

char Maze::Get_Direction(char x, char y) {
  char walls = grid[x][y].walls_visits;

  char target_distance = grid[x][y].distance-1;
  char best_visits = 15;
  char best_direction;

  /*check if we've already solved the maze*/
  if(target_distance == -1) 
    return 0;
  //cout<<"target distance: "<<(int)target_distance<<"\n";
  if(!NORTH_WALL(walls)) {
    if(grid[x][y-1].distance == target_distance && VISITS(grid[x][y-1].walls_visits) <= best_visits) {
      best_direction = NORTH;
      //cout<<"it's north!\n";
      best_visits = VISITS(grid[x][y-1].walls_visits);
   
    }
  }


  if(!SOUTH_WALL(walls)) {
    if(grid[x][y+1].distance == target_distance && VISITS(grid[x][y+1].walls_visits) <= best_visits) {
      best_direction = SOUTH;
      //cout<<"it's south!\n";
      best_visits = VISITS(grid[x][y+1].walls_visits);
    }
  }


  if(!EAST_WALL(walls)) {
    if(grid[x+1][y].distance == target_distance && VISITS(grid[x][y].walls_visits) <= best_visits) {
      best_direction = EAST;
      //cout<<"it's east!\n";
      best_visits = VISITS(grid[x+1][y].walls_visits);
    }
  }


  if(!WEST_WALL(walls)) {
    if(grid[x-1][y].distance == target_distance && VISITS(grid[x-1][y].walls_visits) <= best_visits) {
      best_direction = WEST;
      //cout<<"it's west!\n";
      best_visits = VISITS(grid[x-1][y].walls_visits);
    }
  }

  return best_direction;
}

void Maze::Visit(char x,char y, char walls) {

  /*
   *increment visit count.
   *since visits are stored in lower bits, this is just a normal increment.
   */
  if(VISITS(grid[x][y].walls_visits)<15)
    grid[x][y].walls_visits++;

  if(WALLS(grid[x][y].walls_visits)!=walls)
    Add_Walls(x,y,walls);
}
  

void Maze::Add_Walls(char x, char y, char wall) {
  ADD_WALL(grid[x][y].walls_visits,wall);



  /*add in the other sides of the walls*/
  if(NORTH_WALL(wall) && y>0)
    ADD_WALL(grid[x][y-1].walls_visits,SOUTH);

  if(SOUTH_WALL(wall) && y<GRID_SIZE-1)
    ADD_WALL(grid[x][y+1].walls_visits,NORTH);

  if(EAST_WALL(wall) && x<GRID_SIZE-1)
    ADD_WALL(grid[x+1][y].walls_visits,WEST);

  if(WEST_WALL(wall) && x>0) 
    ADD_WALL(grid[x-1][y].walls_visits,EAST);

}


char Maze::Flood_Fill(char x, char y) {
  char stable = 0;
  while(!stable) {
    stable = 1;
    for(char x=0;x<GRID_SIZE;x++) {
      for(char y=0;y<GRID_SIZE;y++) {
	if(Update_Distance(x,y) != 1) //it was changed
	  stable = 0;
      }
    }
  }
}
