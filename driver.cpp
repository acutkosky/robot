#include "driver.h"
#define SIGN(x) (x<0?-1:1)
#define k_p 130 //50.0 //150 for 400
#define k_d 0.1 //0.0  //0.1 fpr 400
#define max_err 0.9//1.25 //0.9 for 400
#define numreadings 3 //6 //3 for 400

#define rightclosed (unsigned char)16
#define leftclosed (unsigned char)8



/* constructor for DistanceSensor class.
 * takes an analog pin number and immediately calibrates
 */ 
DistanceSensor::DistanceSensor(int apin) {
  pin = apin;
  middle_distance=0;
}


void DistanceSensor::calibrate(void) {
  float volts = 0.0;
  for(int i=0;i<100;i++) {
    volts += delay_read(2,5);
    //delay(10);
  }
  middle_distance = volts/100.0;
}

float DistanceSensor::imbalance(void) {
  float r = read();
  if(r>RIGHT_OPEN_THRESHOLD)
    return r-middle_distance;
  return 0;
}

DistanceSensor::DistanceSensor() {
}

/* reads the voltage from a distance sensor */
float DistanceSensor::read(int num) {
  float ret =0;
  for(int i=0;i<num;i++) {
    ret += analogRead(pin);
  }
  return ret/num;
  //  return (analogRead(pin)+analogRead(pin)+analogRead(pin)+analogRead(pin))/4.0;
}

float DistanceSensor::delay_read(int num, int del) {
  float ret = 0;
  for(int i=0;i<num;i++) {
    ret += analogRead(pin);
    delay(del);
  }
  return ret/num;
}

Driver::Driver(int rs_pin,int ls_pin, int fs_pin, int rd_pin,
	       int rm_pin, int ld_pin, int lm_pin) {
  right_sensor = DistanceSensor(rs_pin);
  left_sensor = DistanceSensor(ls_pin);
  forward_sensor = DistanceSensor(fs_pin);
  
  right_stepper = AccelStepper(1,rm_pin,rd_pin);
  left_stepper = AccelStepper(1,lm_pin,ld_pin);

  right_speed=left_speed=target_speed = 1.0;
}

void Driver::setup(void) {

  right_sensor.calibrate();
  left_sensor.calibrate();



  right_stepper.stop();
  left_stepper.stop();
  right_stepper.setSpeed(-1000.0);
  right_stepper.setAcceleration(1000.0);
  right_stepper.setMaxSpeed(1000.0);
  left_stepper.setSpeed(1000.0);
  left_stepper.setAcceleration(1000.0);
  left_stepper.setMaxSpeed(1000.0);
  last_err = 0.0;
  last_time = micros();
  current_speed = 0;
  //Turn(1000/1040.0*90.0);
  
  //forward_sensor.calibrate();

  //Turn(-1000/1040.0*90.0);


  //forward_sensor.middle_distance = 470;


}


void SetTurn(AccelStepper& stepper1,AccelStepper& stepper2,int steps) {
  stepper1.stop();
  stepper2.stop();
  stepper1.move(steps);
  stepper2.move(steps);
}

void RunToTarget(AccelStepper& stepper1, AccelStepper& stepper2) {
  while(stepper1.distanceToGo()!= 0 || stepper2.distanceToGo()!= 0) {
    stepper1.run();
    stepper2.run();
  }
}

void Driver::SetAvgSpeed(float target) {
  target_speed = target;
}




void printnum(const char* str,float num) {
  Serial.print(str);
  Serial.print(num);
  Serial.print("\n\r");
}
/*
 * Turns by rotating each wheel the given number
 * of steps in opposite directions.
 */



void Driver::Turn(int steps) {
  SetTurn(right_stepper,left_stepper,steps);
  //right_stepper.setSpeed(target_speed*SIGN(steps));
  //left_stepper.setSpeed(target_speed*SIGN(steps));
  RunToTarget(right_stepper,left_stepper);
  current_speed = 0;
}

float Driver::Forward(int steps) {
  //right_stepper.stop();
  //left_stepper.stop();
  right_stepper.setCurrentPosition(0);
  left_stepper.setCurrentPosition(0);
  

  float rs,ls;
  float accel = 0.004;

  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW;

  unsigned char curstate=0;
  unsigned char oldstate=0;

  //right_speed = left_speed = target_speed;
  //  right_stepper.setMaxSpeed(1000);
  //  left_stepper.setMaxSpeed(1000);
  //right_stepper.setSpeed(-right_speed);
  //left_stepper.setSpeed(left_speed);
  //float meta_target_speed = last_speed;
  //Serial.println(current_speed);

  //current_speed = 250.0;
  //right_speed = left_speed = current_speed;
  //right_stepper.setSpeed(-right_speed);
  //left_stepper.setSpeed(left_speed);
  while((-right_stepper.currentPosition()+left_stepper.currentPosition())<(2*steps) && forward_sensor.read() < forward_sensor.middle_distance) {
    /* three cases:
     * we are in a hallway (easiest, can use both sensors)
     * in a T junction (just use on sensor)
     * in a crossroads (just trust to luck)
     */


    //    if(current_speed <target_speed) {
    current_speed += 1.0*(target_speed>=current_speed);
      //}
    
    right_speed = left_speed = current_speed;

    rs = right_sensor.read(numreadings);
    ls = left_sensor.read(numreadings);
    float err = 0;
    
    if(rs > RIGHT_OPEN_THRESHOLD) {
      //there is a wall to the right!
      err -=  (1.0/(rs)-1.0/right_sensor.middle_distance)*1800;
      if(curstate & rightclosed)
	curstate ^= rightclosed;
    } else if(!(curstate &rightclosed))
      curstate ^= rightclosed;
        
    if(ls> LEFT_OPEN_THRESHOLD) {
      //there is a wall to the left!
      err +=  (1.0/ls-1.0/left_sensor.middle_distance)*1900;
      if(curstate & leftclosed)
	curstate ^= leftclosed;

    } else if(!(curstate &leftclosed))
      curstate ^= leftclosed;

    if(curstate != oldstate) {
      //we've sensed a square boundary - make sure we go far enough!
      int distance = steps - (-right_stepper.currentPosition()+left_stepper.currentPosition())/2;
      if(distance<TRANSITION_RATIO*SQUARE_LENGTH)
	  steps += (TRANSITION_RATIO*SQUARE_LENGTH-distance);
      oldstate = curstate;
    }
	 

	
	 //unsigned long t= micros();

    /* we are taking the derivative without time because it worked better*/

    //last_time = t;
    //float turn = right_speed-left_speed;
    //err -= turn/80;
    //err = 0.0;

    err = err*SIGN(err)>max_err?max_err*SIGN(err):err;
    err*=k_p*(current_speed/target_speed)*(current_speed/target_speed);
    float D = (err-last_err);
    last_err = err;

    right_speed+=err-D*k_d;
    left_speed -=err+D*k_d;

    //right_speed -= 2*err*SIGN(err);
    //left_speed -= 2*err*SIGN(err);

    /*random hard coded constants*/
    //right_speed = right_speed-D/400.0;//-turn/295;
    //left_speed = left_speed+D/400.0;//+turn/295;


    right_stepper.setSpeed(-right_speed);
    left_stepper.setSpeed(left_speed);

    /*
    Serial.println("true rs,ls,err");
    Serial.println(right_stepper.speed());
    Serial.println(left_stepper.speed());
    Serial.println(err);
    */
    int rstep = right_stepper.runSpeed();
    int lstep = left_stepper.runSpeed();
    /*
    Serial.println("rstep, lstep");
    Serial.println(rstep);
    Serial.println(lstep);
    */
  }
  //delay(1000);
      digitalWrite(13, HIGH);    // turn the LED off by making the voltage LOW
  return (-right_stepper.currentPosition()+left_stepper.currentPosition())/2.0;

}




  
  


