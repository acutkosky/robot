                                                                                                                                                                                                                                                                                                                                                                                                                 
#include "/home/ashok/robot/AccelStepper/AccelStepper.cpp"
#include "/home/ashok/robot/robot.cpp"

DistanceSensor sensor(2);
Driver driver;

Robot robot;

void setup() {
  //driver.setup();
  sensor.calibrate();
  //driver.SetAvgSpeed(250.0);
  Serial.begin(9600);
  robot.driver.setup();
  robot.driver.SetAvgSpeed(250.0);
  
}

void printmaze(void) {
  for(int i=0;i<GRID_SIZE;i++){
    for(int j=0;j<GRID_SIZE;j++) {
      Serial.print((uint)robot.maze.grid[j][i].distance);
      Serial.print(" ");
    }
    Serial.print("\n\r");
  }
}

void loop() {
  float reading = sensor.read();
  Serial.print(robot.driver.left_sensor.read());
  Serial.print("   ");
  Serial.print(robot.driver.forward_sensor.middle_distance);
  Serial.print("   ");
  Serial.print(robot.driver.forward_sensor.read());
  Serial.print("\n\r");
  robot.maze.Reset_Goal(2,1);
  robot.Solve();
  robot.maze.Reset_Goal(0,0);
  robot.Solve();
  robot.maze.Reset_Goal(2,1);
  robot.Solve();
  /*
  robot.Update_Maze();
  printmaze();
  Serial.println("done!");
  robot.Maze_Step();
  robot.Update_Maze();
  printmaze();
  robot.Go(1,EAST);
  */
  //robot.Go(3,SOUTH);
  //robot.Go(3,WEST);
  //robot.Go(3,NORTH);
  //robot.Solve();
  //Advance(1);  //800 ~ 75cm
  //robot.Advance(1); 
  //robot.Advance(2); 
  //robot.Turn(90.0);
  /*
  robot.Advance(4);  //800 ~ 75cm
  robot.Turn(90.0);
  robot.Advance(4);  //800 ~ 75cm
  robot.Turn(90.0);
  robot.Advance(4);  //800 ~ 75cm
  robot.Turn(90.0);
  while(1);
  */
  while(1);
  
}
