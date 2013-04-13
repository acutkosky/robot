                                                                                                                                                                                                                                                                                                                                                                                                                  
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
  pinMode(13,OUTPUT);
   
}
/*
void printmaze(void) {
  for(int i=0;i<GRID_SIZE;i++){
    for(int j=0;j<GRID_SIZE;j++) {
      Serial.print((uint)robot.maze.grid[j][i].distance);
      Serial.print(" ");
    }
    Serial.print("\n\r");
  }
}
*/
void loop() {
  Serial.println("whatttttt");
  float reading = sensor.read();
  while(1) {
  Serial.print(robot.driver.left_sensor.read(10));
  Serial.print("   ");
  Serial.print(robot.driver.right_sensor.read(10));
  Serial.print("   ");
  Serial.print(robot.driver.forward_sensor.read(10));
  Serial.print("\n\r");
  delay(10);
  }
  //robot.driver.Forward(1000);
  //while(1);
  unsigned char not_done = 0;
  //robot.Go(6,EAST);
  //while(1);
  int led =13;
  /*
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);
  */
  //robot.driver.Forward(1000);
  //while(1);
  
  //robot.driver.Turn(200);
  //robot.driver.Forward(400);
  //while(1);
  robot.maze.Reset_Goal(4,4);
  
  not_done = robot.Solve();
  
  while(not_done) {
  robot.maze.Reset_Goal(0,0);
  robot.Solve();
  robot.maze.Reset_Goal(4,4);
  not_done = robot.Solve();
  }
  
  

  //while(1);
  
}
