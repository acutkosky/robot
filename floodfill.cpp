

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
      unsigned char xdist,ydist;
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

void Maze::Reset_Goal(unsigned char x, unsigned char y) {
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

unsigned char Maze::Update_Distance(unsigned char x, unsigned char y) {
  
  
  unsigned char old_distance = grid[x][y].distance;
  
  /*are we at the center yet?*/
  if(old_distance == 0) {
    return 1;
  }
  
  unsigned char best_distance = 255;
  unsigned char walls = grid[x][y].walls_visits;
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

unsigned char Maze::Get_Direction(unsigned char x, unsigned char y) {
  unsigned char walls = grid[x][y].walls_visits;

  unsigned char best_distance = 255;
  unsigned char best_visits = 15;
  unsigned char best_direction;

  /*check if we've already solved the maze*/
  if(grid[x][y].distance == 0) 
    return 0;
  //cout<<"target distance: "<<(int)target_distance<<"\n";
  if(!NORTH_WALL(walls)) {
    if(grid[x][y-1].distance < best_distance || (grid[x][y-1].distance==best_distance && VISITS(grid[x][y-1].walls_visits) <= best_visits)) {
      best_direction = NORTH;
      //cout<<"it's north!\n";
      best_visits = VISITS(grid[x][y-1].walls_visits);
      best_distance = grid[x][y-1].distance;
   
    }
  }


  if(!SOUTH_WALL(walls)) {
    if(grid[x][y+1].distance < best_distance || (grid[x][y+1].distance == best_distance && VISITS(grid[x][y+1].walls_visits) <= best_visits)) {
      best_direction = SOUTH;
      //cout<<"it's south!\n";
      best_visits = VISITS(grid[x][y+1].walls_visits);
      best_distance = grid[x][y+1].distance;
    }
  }


  if(!EAST_WALL(walls)) {
    if(grid[x+1][y].distance < best_distance || (grid[x+1][y].distance == best_distance && VISITS(grid[x+1][y].walls_visits) <= best_visits)) {
      best_direction = EAST;
      //cout<<"it's east!\n";
      best_visits = VISITS(grid[x+1][y].walls_visits);
      best_distance = grid[x+1][y].distance;
    }
  }


  if(!WEST_WALL(walls)) {
    if(grid[x-1][y].distance < best_distance || (grid[x-1][y].distance == best_distance && VISITS(grid[x-1][y].walls_visits) <= best_visits)) {
      best_direction = WEST;
      //cout<<"it's west!\n";
      best_visits = VISITS(grid[x-1][y].walls_visits);
      best_distance = grid[x-1][y].distance;
    }
  }

  return best_direction;
}

unsigned char Maze::Visit(unsigned char x,unsigned char y, unsigned char walls) {

  /*
   *increment visit count.
   *since visits are stored in lower bits, this is just a normal increment.
   */
  if(VISITS(grid[x][y].walls_visits)<15)
    grid[x][y].walls_visits++;
  unsigned char ret = 0;
  if(WALLS(grid[x][y].walls_visits)!=walls)
    ret = Set_Walls(x,y,walls);
  return ret;
}
  

unsigned char Maze::Add_Walls(unsigned char x, unsigned char y, unsigned char wall) {
  unsigned char ret = (wall != WALLS(grid[x][y].walls_visits));
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
  return ret;
}

unsigned char Maze::Set_Walls(unsigned char x, unsigned char y, unsigned char wall) {
  unsigned char ret = (wall != WALLS(grid[x][y].walls_visits));
  unsigned char visits = VISITS(grid[x][y].walls_visits);
  unsigned char oldwalls = grid[x][y].walls_visits;
  if(ret) {    
  /*add in the other sides of the walls*/
    if(NORTH_WALL(wall)) {
      if(y>0)
	ADD_WALL(grid[x][y-1].walls_visits,SOUTH);
    } else if(NORTH_WALL(oldwalls) && y>0) {
      grid[x][y-1].walls_visits ^= SOUTH;
    }
      

    if(SOUTH_WALL(wall)) {
      if(y<GRID_SIZE-1)
	ADD_WALL(grid[x][y+1].walls_visits,NORTH);
    } else if(SOUTH_WALL(oldwalls) && y<GRID_SIZE-1) {
      grid[x][y+1].walls_visits ^= NORTH;
    }

    if(EAST_WALL(wall)) {
      if(x<GRID_SIZE-1)
	ADD_WALL(grid[x+1][y].walls_visits,WEST);
    } else if(EAST_WALL(oldwalls) && x< GRID_SIZE-1) {
      grid[x+1][y].walls_visits ^= WEST;
    }

    if(WEST_WALL(wall)) {
      if(x>0)
	ADD_WALL(grid[x-1][y].walls_visits,EAST);
    } else if (WEST_WALL(oldwalls) && x>0) {
      grid[x-1][y].walls_visits ^= EAST;
    }

  }
    grid[x][y].walls_visits = visits|wall;
}


unsigned char Maze::Flood_Fill(unsigned char x, unsigned char y) {
  unsigned char stable = 0;
  unsigned char changed = 0; //set to one if any part of the maze was changed
  while(!stable) {
    stable = 1;
    for(unsigned char x=0;x<GRID_SIZE;x++) {
      for(unsigned char y=0;y<GRID_SIZE;y++) {
	if(Update_Distance(x,y) != 1) {//it was changed
	  stable = 0;
	  changed = 1;
	}
      }
    }
  }
  return changed;
}
