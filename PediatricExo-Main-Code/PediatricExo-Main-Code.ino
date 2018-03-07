//Pediatric Exo Main code

//TODO1: Define motors direction and number (Done)
//TODO2: Include contact switches code, add pin numbers (Done)
//TODO3: Test states individually
//TODO4: Test PID for state transition
//TODO5: Add FSM code
//TODO6: Test gait cycle for each leg individually
//TODO7: Test full FSM
//TODO8: Test joystick
//TODO9: Check Motor direction in other leg (They are mechanically inverted)
#include <PID_v1.h>

//Declarations for Monster Drivers
#define BRAKE 0
#define CW    1
#define CCW   2
#define CS_THRESHOLD 15   // Definition of safety current (Check: "1.3 Monster Shield Example").
//MOTOR 1
#define MOTOR_A1_PIN 7
#define MOTOR_B1_PIN 8
//MOTOR 2
#define MOTOR_A2_PIN 4
#define MOTOR_B2_PIN 9
#define PWM_MOTOR_KNEE 5
#define PWM_MOTOR_HIP 6
#define CURRENT_SEN_KNEE A2
#define CURRENT_SEN_HIP A3
#define EN_PIN_KNEE A0
#define EN_PIN_HIP A1
#define MOTOR_KNEE 0 //Motor Knee
#define MOTOR_HIP 1 //Motor Hip
short usSpeed = 100;  //default motor speed = 150
unsigned short usMotor_Status = BRAKE;
char select_motor;
uint8_t motor = 0;
int serial = 0;

//Declarations for switches
#define SWITCH_KNEE_BACK 21 
#define SWITCH_KNEE_FRONT 20
#define SWITCH_HIP_BACK 19
#define SWITCH_HIP_BACK 18

void setup() {
  //pinMode for monster drivers
  pinMode(MOTOR_A1_PIN, OUTPUT);
  pinMode(MOTOR_B1_PIN, OUTPUT);
  pinMode(MOTOR_A2_PIN, OUTPUT);
  pinMode(MOTOR_B2_PIN, OUTPUT);
  pinMode(PWM_MOTOR_KNEE, OUTPUT);
  pinMode(PWM_MOTOR_HIP, OUTPUT);
  pinMode(CURRENT_SEN_KNEE, OUTPUT);
  pinMode(CURRENT_SEN_HIP, OUTPUT);  
  pinMode(EN_PIN_KNEE, OUTPUT);
  pinMode(EN_PIN_HIP, OUTPUT);

  //pinMode for switches
  pinMode(SWITCH_KNEE_BACK, INPUT); 
  pinMode(SWITCH_KNEE_FRONT, INPUT); 

  Serial.begin(9600);     
  
}

void loop() {
  digitalWrite(EN_PIN_KNEE, HIGH); //Enable Motor1
  digitalWrite(EN_PIN_HIP, HIGH); //Enable Motor2

  //Contact switch: Change state when digital read is '0' (switch pressed)

  /* FSM
  MIDSTANCE:
  PRE-SWING
  MID-SWING
  TERMINAL-SWING
  */

  //MIDSTANCE to PRE-SWING
  while((digitalRead(SWITCH_KNEE_BACK) == 1) && (digitalRead(SWITCH_KNEE_FRONT) == 1)){
    Forward(MOTOR_KNEE);
    
  }
  if(digitalRead(SWITCH_KNEE_FRONT) == 0){
      Reverse(MOTOR_KNEE);
      delay(75); //Time for the switch to change state to 1
      while((digitalRead(SWITCH_KNEE_BACK) == 1) && (digitalRead(SWITCH_KNEE_FRONT) == 1)){
        Reverse(MOTOR_KNEE);
      }
  }
  if(digitalRead(SWITCH_KNEE_BACK) == 0){
    Forward(MOTOR_KNEE);
    delay(75);
  }
    
//  //PRE-SWING to MID-SWING
//  while(digitalRead(SWITCH_HIP_FRONT != 0)){ //
//    Forward(MOTOR_HIP);
//  }
//  stop(MOTOR_HIP);
//  //MID-SWITN to TERMINAL-SWING
//  while(digitalRead(SWITCH_KNEE_FRONT != 0)){ //
//    Forward(MOTOR_KNEE);
//  }
//  //TERMINAL-SWING to MIDSTANCE
//  while(digitalRead(SWITCH_HIP_BACK != 0)){ //
//    Reverse(MOTOR_HIP);
//  }
  
}

//Functions
void Stop(int motor)
{
  Serial.println("Stop");
  usMotor_Status = BRAKE;
  motorGo(motor, usMotor_Status, 0);
}

void Forward(int motor)
{
  Serial.println("Forward");
  usMotor_Status = CCW;
  motorGo(motor, usMotor_Status, usSpeed);
}

void Reverse(int motor)
{
  Serial.println("Reverse");
  usMotor_Status = CW;
  motorGo(motor, usMotor_Status, usSpeed);
}

void IncreaseSpeed()
{
  usSpeed = usSpeed + 10;
  if(usSpeed > 255)
  {
    usSpeed = 255;  
  }
  
  Serial.print("Speed +: ");
  Serial.println(usSpeed);
}

void DecreaseSpeed()
{
  usSpeed = usSpeed - 10;
  if(usSpeed < 0)
  {
    usSpeed = 0;  
  }
  
  Serial.print("Speed -: ");
  Serial.println(usSpeed);
}

void motorGo(uint8_t motor, uint8_t direct, uint8_t pwm) //Function that controls the variables: motor(0 or 1), direction (cw or ccw) and pwm (0 - 255);
{
  if(motor == MOTOR_KNEE)
  {
    if(direct == CW)
    {
      digitalWrite(MOTOR_A1_PIN, LOW); 
      digitalWrite(MOTOR_B1_PIN, HIGH);
    }
    else if(direct == CCW)
    {
      digitalWrite(MOTOR_A1_PIN, HIGH);
      digitalWrite(MOTOR_B1_PIN, LOW);      
    }
    else
    {
      digitalWrite(MOTOR_A1_PIN, LOW);
      digitalWrite(MOTOR_B1_PIN, LOW);            
    }
    
    analogWrite(PWM_MOTOR_KNEE, pwm); 
  }
  else if(motor == MOTOR_HIP)
  {
    if(direct == CW)
    {
      digitalWrite(MOTOR_A2_PIN, LOW);
      digitalWrite(MOTOR_B2_PIN, HIGH);
    }
    else if(direct == CCW)
    {
      digitalWrite(MOTOR_A2_PIN, HIGH);
      digitalWrite(MOTOR_B2_PIN, LOW);      
    }
    else
    {
      digitalWrite(MOTOR_A2_PIN, LOW);
      digitalWrite(MOTOR_B2_PIN, LOW);            
    }
    
    analogWrite(PWM_MOTOR_HIP, pwm);
  }
}


