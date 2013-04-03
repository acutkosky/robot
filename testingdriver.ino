
#include "/usr/share/arduino/libraries/AccelStepper/AccelStepper.cpp"
#include "/home/olorin/robot/moving/driver.cpp"

DistanceSensor sensor(2);
Driver driver;

void setup() {
  driver.setup();
  sensor.calibrate();
  driver.SetAvgSpeed(250.0);
  Serial.begin(9600);
  
  
}
void loop() {
  float reading = sensor.read();
  Serial.print(driver.right_sensor.read());
  Serial.print("   ");
  Serial.print(driver.left_sensor.read());
  Serial.print("\n\r");
  driver.Forward(900);
  
  while(1);
}
