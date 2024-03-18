
#include <Wire.h>
#include <Zumo32U4.h>


//QTR Threshold
#define QTR_THRESHOLD     1000  

//speeds for the robots moters
#define REVERSE_SPEED     100  
#define TURN_SPEED        100
#define FORWARD_SPEED     100
#define REVERSE_DURATION  200  
#define TURN_DURATION     100  

//initilises all of the integers needed for the robot to operate
int Left_Turn_Count = 0;
int Right_Turn_Count = 0;
int Forward_Count = 0;
int LeftRightAngleTurnCount = 0;
int RightRightAngleTurnCount = 1;

const uint8_t sensorThreshold = 1;





Zumo32U4LCD display;


Zumo32U4ButtonA buttonA;
Zumo32U4ButtonB buttonB;
Zumo32U4Buzzer buzzer;
Zumo32U4Motors motors;
Zumo32U4LineSensors lineSensors;
Zumo32U4ProximitySensors proxSensors;

#define NUM_SENSORS 3
unsigned int lineSensorValues[NUM_SENSORS];
// robot waits for the button to be pressed
void waitForButtonAndCountDown()
{
  ledYellow(1);
  display.clear();
  display.print(F("Press A"));

  buttonA.waitForButton();

  ledYellow(0);
  display.clear();

}

void setup()
{

  lineSensors.initThreeSensors();
  proxSensors.initThreeSensors();


  waitForButtonAndCountDown();
}

void loop()
{
  if (buttonA.isPressed())
  {
    motors.setSpeeds(0, 0);
    buttonA.waitForRelease();
    waitForButtonAndCountDown();
  }

  display.println(Left_Turn_Count);

  lineSensors.read(lineSensorValues);
  proxSensors.read();
 
      

 if (lineSensorValues[0] > QTR_THRESHOLD)
  {
    leftTurn();//if sensor  detects black line call left turn function
  }
  else if (lineSensorValues[NUM_SENSORS - 1] > QTR_THRESHOLD)
  {
    leftTurn();
  }
  else if (proxSensors.countsFrontWithLeftLeds() >= 6 || proxSensors.countsFrontWithRightLeds() >= 6)
  {
    objectSeen(); //if an boject is seen call object seen function
  }
  else
  {
    Forword(); //call forword function if no line detected
  }

  if(Left_Turn_Count >= 30)
  {
    rightTurn(); //if turn count is greater than 30 call right turn function
  }
  else
  {
    return;
  }

  if(Forward_Count >= 200)
  {
    LRightAngleTurn(); // if forward count is greater than 200 call right angle turn function
  }
  else
  {
    return;
  }
}

void Forword()
{
  // makes robot move forward art a set speed
  motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
  Forward_Count ++;
}
void leftTurn()
{
  // makes a robot make a small left turn 
    motors.setSpeeds(0, 0);
    delay(300);
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    delay(REVERSE_DURATION);
    motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
    delay(TURN_DURATION);
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
    Left_Turn_Count = Left_Turn_Count + 1;
}
void rightTurn()
{ //makes the robot do a 90 degree right turn to get out of being stuck
    motors.setSpeeds(0, 0);
    delay(300);
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    delay(REVERSE_DURATION);
    motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
    delay(TURN_DURATION * 11);
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
    Left_Turn_Count = 0;
}
void objectSeen()
{ // makes the robot stop beep and turn around if it seens an object
    buzzer.playFromProgramSpace(PSTR("!>g32>>c32"));
    
    motors.setSpeeds(0, 0);
    delay(2000);
    motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
    delay(TURN_DURATION * 22);
    

    display.println("object");
}
void LRightAngleTurn()
{//makes the robot do a left right angle turn
      motors.setSpeeds(0, 0);
    delay(300);
    buzzer.playFromProgramSpace(PSTR("!>g32>>c32"));
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    delay(REVERSE_DURATION);
    motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
    delay(TURN_DURATION * 11);
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
    
}






