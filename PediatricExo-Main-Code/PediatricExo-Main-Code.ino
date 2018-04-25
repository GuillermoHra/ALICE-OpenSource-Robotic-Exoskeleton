// Pediatric Exo Main code

// TODO1: Define motors direction
// TODO2: FSM code (Working on)
// TODO3: Test gait cycle for each leg individually (put attention on the speed)
// TODO4: Test PID for state transition (speed)
// TODO5: Test full FSM (Both legs)
// TODO6: Test joystick
// TODO7: Check Motor direction in other leg (They are mechanically inverted)
// Be aware of loss of absolute reference position (save values to restore them later)
// Remove delay after Serialprints when testing is done

#include <PID_v1.h>

// Declarations for Monster Drivers
#define BRAKE 0
#define CW    1
#define CCW   2
#define CS_THRESHOLD 15   // Definition of safety current
// MOTOR 1
#define MOTOR_A1_PIN 7
#define MOTOR_B1_PIN 8
// MOTOR 2
#define MOTOR_A2_PIN 4
#define MOTOR_B2_PIN 9
#define PWM_MOTOR_KNEE 5
#define PWM_MOTOR_HIP 6
#define CURRENT_SEN_KNEE A2
#define CURRENT_SEN_HIP A3
#define EN_PIN_KNEE A0
#define EN_PIN_HIP A1
#define MOTOR_KNEE 0  // Motor Knee
#define MOTOR_HIP 1  // Motor Hip
//short usSpeed;     // default motor speed = 150
unsigned short usMotor_Status = BRAKE;
char select_motor;
uint8_t motor = 0;
int serial = 0;

// Declarations for potentiometers
#define POT_HIP A13 // Pot analog pin for right hip 
#define POT_KNEE A14 // Pot analog pin for right knee

void setup() {
  // pinMode for monster drivers
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

  // pinMode for potentiometers
  pinMode(POT_HIP, INPUT); 
  pinMode(POT_KNEE, INPUT); 

  Serial.begin(9600);     
  
}

void loop() {
  digitalWrite(EN_PIN_KNEE, HIGH); // Enable Motor1
  digitalWrite(EN_PIN_HIP, HIGH); // Enable Motor2

  /* FSM
  MIDSTANCE
  PRE-SWING
  MID-SWING
  TERMINAL-SWING
 
  Right hip: (30° - (470-505)), (-10° - (598-608)), (15° - (514-526)), (0° - (560-569)) Valor disminuye(ángulo aumenta), Valor aumenta(ángulo disminuye)
  Right knee: (-60° - (385-395)), (0° - (611-616)), (-30° - (478-485)) Valor disminuye(ángulo disminuye), Valor aumenta(ángulo aumenta)
   
  Left hip: (30° - (527-542)), (-10° - (595-605)), (15° - ()), (0° - ())
  Left knee: (-60° - (601-607)), (0° - (474-481)), (-30° - ()) For left knee 0° are 15° due to mechanical failure
  */
  
  delay(10000);
  Serial.println("Get ready");
  delay(3000);

  // Hip and Knee to 0°
  Serial.println("Hip and Knee to 0°");
  delay(1000);
  while((analogRead(POT_HIP) < 560) || (analogRead(POT_HIP) > 569)){
    if (analogRead(POT_HIP) < 560){
      // Move reverse
      while(analogRead(POT_HIP) < 560){
        Reverse(MOTOR_HIP, 65);
        delay(50);
      }
      Stop(MOTOR_HIP);
      
    }
    delay(200);
    if (analogRead(POT_HIP) > 569){
      // Move forward
      while(analogRead(POT_HIP) > 569){
        Forward(MOTOR_HIP, 65);
        delay(50);
      }
      Stop(MOTOR_HIP);
    }
    delay(200);
  }

  while((analogRead(POT_KNEE) < 611) || (analogRead(POT_KNEE) > 616)){
    if (analogRead(POT_KNEE) < 611){
      // Move forward
      while(analogRead(POT_KNEE) < 611){
        Forward(MOTOR_KNEE, 65);
        delay(50);
      }
      Stop(MOTOR_KNEE);
    }
    delay(200);
    if (analogRead(POT_KNEE) > 616){
      // Move reverse
      while(analogRead(POT_KNEE) > 616){
        Reverse(MOTOR_KNEE, 65);
        delay(50);
      }
      Stop(MOTOR_KNEE);
    }
    delay(200); 
  }
  Serial.println("Hip and Knee to 0° done");
  delay(1000); 

  // INITIAL SWING: HIP:0°, KNEE:0->(-60°)
  Serial.println("Initial Swing");
  delay(1000);
  while(analogRead(POT_KNEE) > 390){
    Reverse(MOTOR_KNEE, 65);
    delay(50);
    // PID: Reduce speed when approaching the specified angle 
  }
  Stop(MOTOR_KNEE);
  delay(750);
    
  // MID-SWING: HIP:0->15°, KNEE:(-60°)->(-30°)
  while(analogRead(POT_HIP) > 545){
    Forward(MOTOR_HIP, 65);
    delay(50);
  }
  Stop(MOTOR_HIP);
  delay(750);
  while(analogRead(POT_KNEE) < 480){
    Forward(MOTOR_KNEE, 65);
    delay(50);
  }
  Stop(MOTOR_KNEE);
  delay(750);
  Serial.println("Initial Swing done");
  delay(1000);    
  
  // TERMINAL SWING: HIP:15°->30°, KNEE:(-30°)->0°
  Serial.println("Terminal Swing");
  delay(1000);
  while((analogRead(POT_KNEE) < 612)){
    Forward(MOTOR_KNEE, 65);
    delay(50);
  }
  Stop(MOTOR_KNEE);
  delay(750);
  while(analogRead(POT_HIP) > 520){
    Forward(MOTOR_HIP, 65);
    delay(50);
  }
  Stop(MOTOR_HIP);
  delay(750);
  Serial.println("Terminal Swing done");
  delay(1000);
  
  // MID-STANCE: HIP:30°->0°, KNEE:0°
  Serial.println("Mid-Stance");
  delay(1000);
  while(analogRead(POT_HIP) < 565){
    Reverse(MOTOR_HIP, 35);
    delay(50);
  }
  // Amortiguar
  Stop(MOTOR_HIP);
  delay(750);
  Serial.println("Mid-Stance done");
  delay(1000);

  Serial.println("FSM Done, you have 10 seconds to stop the execution");
  delay(10000);
}

// Functions
void Stop(int motor)
{
  Serial.println("Stop");
  usMotor_Status = BRAKE;
  motorGo(motor, usMotor_Status, 0);
}

void Forward(int motor, short usSpeed)
{
  usMotor_Status = CCW;
  motorGo(motor, usMotor_Status, usSpeed);
}

void Reverse(int motor, short usSpeed)
{
  usMotor_Status = CW;
  motorGo(motor, usMotor_Status, usSpeed);
}

//void IncreaseSpeed()
//{
//  usSpeed = usSpeed + 10;
//  if(usSpeed > 255)
//  {
//    usSpeed = 255;  
//  }
//  
//  Serial.print("Speed +: ");
//  Serial.println(usSpeed);
//}
//
//void DecreaseSpeed()
//{
//  usSpeed = usSpeed - 10;
//  if(usSpeed < 0)
//  {
//    usSpeed = 0;  
//  }
//  
//  Serial.print("Speed -: ");
//  Serial.println(usSpeed);
//}

void motorGo(uint8_t motor, uint8_t direct, uint8_t pwm) // Function that controls the variables: motor(0 or 1), direction (cw or ccw) and pwm (0 - 255);
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


