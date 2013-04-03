
//#include <AccelStepper.h>

#ifndef DRIVER_h
#define DRIVER_h

#define Right_Open_Threshold 200.0 //need to check this
#define Left_Open_Threshold 290.0
struct DistanceSensor {
  int pin;
  int middle_distance;
  DistanceSensor(int);
  DistanceSensor();
  void calibrate(void);
  float read(void);
  float imbalance(void);
};

class Driver {
 public:
  DistanceSensor right_sensor;
  DistanceSensor left_sensor;
  DistanceSensor forward_sensor;
  
  float right_speed;
  float left_speed;
  float target_speed;

  float last_err;
  unsigned long last_time;

  AccelStepper right_stepper;
  AccelStepper left_stepper;
  //Driver();
  Driver(int rs_pin=2,int ls_pin=1, int fs_pin=0, int rd_pin=2,
	 int rm_pin=3, int ld_pin =4, int lm_pin =5);

  void setup(void);

  void SetAvgSpeed(float);

  void Turn(int);

  void Forward(int);

  void SetTargetSpeed(float);
};
void SetTurn(AccelStepper&,AccelStepper&,int);

void RunToTarget(AccelStepper&, AccelStepper&);



#endif
