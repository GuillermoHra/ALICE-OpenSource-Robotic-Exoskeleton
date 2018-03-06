//Pediatric Exo Main code

//TODO1: Define motors direction and number, confirm motor1=knee, motor2=hip
//TODO2: Include contact switches code, add pin numbers
//TODO3: Add FSM code
//TODO4: Test states individually
//TODO5: Test PID for state transition
//TODO6: Test gait cycle for each leg individually
//TODO7: Test full FSM
//TODO8: Test joystick
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
#define PWM_MOTOR_1 5
#define PWM_MOTOR_2 6
#define CURRENT_SEN_1 A2
#define CURRENT_SEN_2 A3
#define EN_PIN_1 A0
#define EN_PIN_2 A1
#define MOTOR_1 0
#define MOTOR_2 1
short usSpeed = 150;  //default motor speed
unsigned short usMotor_Status = BRAKE;
char select_motor;
uint8_t motor = 0;
int serial = 0;

//Declarations for switches
#define SIWTCH_KNEE_BACK_R 
#define SIWTCH_KNEE_FRONT_R
#define SIWTCH_HIP_BACK_R
#define SIWTCH_HIP_BACK_R 

void setup() {
  //pinMode for monster drivers
  pinMode(MOTOR_A1_PIN, OUTPUT);
  pinMode(MOTOR_B1_PIN, OUTPUT);
  pinMode(MOTOR_A2_PIN, OUTPUT);
  pinMode(MOTOR_B2_PIN, OUTPUT);
  pinMode(PWM_MOTOR_1, OUTPUT);
  pinMode(PWM_MOTOR_2, OUTPUT);
  pinMode(CURRENT_SEN_1, OUTPUT);
  pinMode(CURRENT_SEN_2, OUTPUT);  
  pinMode(EN_PIN_1, OUTPUT);
  pinMode(EN_PIN_2, OUTPUT);

  Serial.begin(9600);     
  
}

void loop() {
  digitalWrite(EN_PIN_1, HIGH); //Enable Motor1
  digitalWrite(EN_PIN_2, HIGH); //Enable Motor2

  //Contact switch: Change state when digital read is '0' (switch pressed)

  /* FSM
  MIDSTANCE:
  PRE-SWING
  MID-SWING
  TERMINAL-SWING
  */

  //Move motor1(knee), states begin slowly, then increase speed. High torque at the beginning
  //MIDSTANCE to PRE-SWING
  while(digitalRead(SWITCH_KNEE_BACK != 0)){ //While not touching knee back
    Reverse(MOTOR_1);
  }
  stop(MOTOR_1);

  //PRE-SWING to MID-SWING
  while(digitalRead(SWITCH_HIP_FRONT != 0)){ //
    Forward(MOTOR_2);
  }
  stop(MOTOR_2);

  //MID-SWITN to TERMINAL-SWING
  while(digitalRead(SWITCH_KNEE_FRONT != 0)){ //
    Forward(MOTOR_1);
  }

  //TERMINAL-SWING to MIDSTANCE
  while(digitalRead(SWITCH_HIP_BACK != 0)){ //
    Reverse(MOTOR_2);
  }
  
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
  usMotor_Status = CW;
  motorGo(motor, usMotor_Status, usSpeed);
}

void Reverse(int motor)
{
  Serial.println("Reverse");
  usMotor_Status = CCW;
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
  if(motor == MOTOR_1)
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
    
    analogWrite(PWM_MOTOR_1, pwm); 
  }
  else if(motor == MOTOR_2)
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
    
    analogWrite(PWM_MOTOR_2, pwm);
  }
}


