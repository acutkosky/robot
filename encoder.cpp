#include "encoder.h"
#define ENC_A 8
#define ENC_B 9
#define ENC_PORT PINB
#define encoderSwitchPin 7
 int waitingforencoder = 1;
  static unsigned int counter = 0;      //this variable will be changed by encoder input
unsigned   int tmpdata;

void setup_encoder(){
 /* Setup encoder pins as inputs */
  pinMode(ENC_A, INPUT);
  digitalWrite(ENC_A, HIGH);
  pinMode(ENC_B, INPUT);
  digitalWrite(ENC_B, HIGH);
  
  
  pinMode(encoderSwitchPin, INPUT);
  digitalWrite(encoderSwitchPin, HIGH); //turn pullup resistor on


}

void wait_for_encoder(){
  Serial.println(sizeof(int));
  while(waitingforencoder < 3){

 int numPresses = 0;
 /*  tmpdata = read_encoder();
    if( tmpdata ) {
      counter += tmpdata;
    }
 */  if(!digitalRead(encoderSwitchPin)){
      while(!digitalRead(encoderSwitchPin)){
      }
      delay(300);
      if(digitalRead(encoderSwitchPin)){
	Serial.print("GO at ");
	Serial.println(counter);
	if ((5<counter)&&(counter < 100))
	  {
	    //clockwise
	    Serial.println("HIGH SPEED");
	    
	    waitingforencoder = 0;
	    
	  } 
	else if(counter < -5)
	  {
	    //counterclockwise
	    
	    Serial.println("LOW SPEED");
	    waitingforencoder = 0;
	    
	  }
	else
	  {
	    Serial.println("nope");
	    counter = 0;
	  }
       //      delay(300);
      }
    }
  }
}


unsigned int read_encoder()
{
  while(digitalRead(encoderSwitchPin)){
    static unsigned int enc_states[] = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0};
    static unsigned int old_AB = 0;
    /**/
    old_AB <<= 2;                   //remember previous state
    old_AB |= ( ENC_PORT & 0x03 );  //add current state
    counter += ( enc_states[( old_AB & 0x0f )]);
  }
  return counter;
}
