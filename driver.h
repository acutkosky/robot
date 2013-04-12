
//#include <AccelStepper.h>

#ifndef DRIVER_h
#define DRIVER_h

/*random hard coded constants*/
#define RIGHT_OPEN_THRESHOLD 180.0
#define LEFT_OPEN_THRESHOLD 200.0
#define FORWARD_OPEN_THRESHOLD 300.0


struct DistanceSensor {
  int pin;
  int middle_distance;
  DistanceSensor(int);
  DistanceSensor();
  void calibrate(void);
  float read(int num=4);
  float delay_read(int,int);
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

  float current_speed;

  float last_err;
  unsigned long last_time;

  AccelStepper right_stepper;
  AccelStepper left_stepper;


  /* r = right, l = left, s = sensor, d = direction, m = moving (up/down makes a tick) */
  Driver(int rs_pin=2,int ls_pin=1, int fs_pin=0, int rd_pin=2,
	 int rm_pin=3, int ld_pin =4, int lm_pin =5);


  void setup(void);


  /*speed for forward motion */
  void SetAvgSpeed(float);


  /* int = number of steps in a turn ~ 100 for 90 degrees*/
  void Turn(int);

  /*
   *run forward this many steps, using walls for guidance
   *stops if forward sensor sees a wall
   *returns number of steps moved
   */
  float Forward(int);

  void SetTargetSpeed(float);

  /*is there a wall to the sides*/
  int forward_wall(void);
  int right_wall(void);
  int left_wall(void);
};
void SetTurn(AccelStepper&,AccelStepper&,int);

void RunToTarget(AccelStepper&, AccelStepper&);



#endif
