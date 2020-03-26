# include "includes.h"
# include <Servo.h> 

bool bUnlock = true;
Servo sensordirection;

unsigned int g_driveState = STATE_DRIVE_FORWARD;
unsigned int g_driveStateCurrent = STATE_UNDEFINED;
unsigned int g_servoState = STATE_SERVO_UNDEFINED;
unsigned int g_distLeft = 0;
unsigned int g_distRight = 0;
unsigned int g_short1 = 0;
unsigned int g_short2 = 0;
unsigned int g_error = 0;
TIMER_DECLARE(servo);

void setup() 
{
  pinMode(PIN_MOTORRECHTS_ENABLE, OUTPUT);
  pinMode(PIN_MOTORRECHTS_POL1, OUTPUT);
  pinMode(PIN_MOTORRECHTS_POL2, INPUT);

  pinMode(PIN_MOTORLINKS_ENABLE, OUTPUT);
  pinMode(PIN_MOTORLINKS_POL1, OUTPUT);
  pinMode(PIN_MOTORLINKS_POL2, INPUT);
  
  //---------------------------------------------------------------------------------------------
  
  pinMode(PIN_SONIC_TRIGGER,OUTPUT);
  pinMode(PIN_SONIC_ECHO, INPUT);

  sensordirection.attach(PIN_SERVO);
  
  Serial.begin(19200);
}


void loop() 
{
   //unsigned int distance = getDistance ();
   

   while(1)
   {
      
          
          if(g_distLeft>25 || g_distRight>25 && g_driveState == STATE_STOP_MEASURE)
          {
            g_error = 0;
          }
      
//      Serial.print(g_error);
      
      switch (g_driveState)
      {
        case STATE_DRIVE_FORWARD:
          if (g_driveStateCurrent != STATE_DRIVE_FORWARD)
          {
            
            setDirection(FORWARD);
            drive(full);
            g_driveStateCurrent = STATE_DRIVE_FORWARD;
          }
  
          // Distanz messen
          if (g_servoState == STATE_SERVO_UNDEFINED)
          { 
            look(half_left);



            g_servoState = STATE_SERVO_TURNING_LEFT;
            TIMER_START(servo, 350);
          }
          
          if (TIMER_ISDUE(servo))
          {
            if (g_servoState == STATE_SERVO_TURNING_LEFT)
            {
             g_servoState = STATE_SERVO_INPOSITION_LEFT;
            }        
            else if (g_servoState == STATE_SERVO_TURNING_RIGHT)
            {
              g_servoState = STATE_SERVO_INPOSITION_RIGHT;
            }
          }
  
          if (g_servoState == STATE_SERVO_INPOSITION_LEFT)
          {
            g_distLeft = getDistance();
            Serial.print("g_distLeft = ");
            Serial.println(g_distLeft);
           if(g_distLeft < MINIMUM_DIST)
           {
             drive(stay);
             
             g_driveState = STATE_STOP_MEASURE;
             break;
           }
           else
           {
  
            look(half_right);
            g_servoState = STATE_SERVO_TURNING_RIGHT;
            TIMER_START(servo, 350);
           }
          }
  
          if(g_servoState == STATE_SERVO_INPOSITION_RIGHT)
          {
            g_distRight = getDistance();
            Serial.print("g_distRight = ");
            Serial.println(g_distRight);
            g_servoState = STATE_SERVO_UNDEFINED;
  
            // die zwei Distanzen prüfen, ob wir zu nah an einem Hindernis dran sind
            // falls ja, Drehung veranlassen
            if (g_distRight < MINIMUM_DIST)
            {
              drive(stay);
              g_driveState = STATE_STOP_MEASURE;
              break;
            }
          }
          break;
        
        
          case STATE_STOP_MEASURE:
            g_error = g_error + 1;
            g_driveStateCurrent = STATE_STOP_MEASURE;
            look (left);
            delay (500);
            g_short1 = getDistance();
            delay (500);
  
            look (right);
            delay (500);
             g_short2 = getDistance();
            delay (500);
            look(FORWARD);
            if(g_short1 > g_short2)
            {
              g_driveState = STATE_TURN_LEFT;
            }
            else 
            {
              g_driveState = STATE_TURN_RIGHT;
            }
            if(g_error > 5)
            {
              drive(full);
              delay(300) ;
              drive(stay);
              g_error = 0;
              g_servoState = STATE_SERVO_UNDEFINED;
              break;
            }
            break;
        
        
          case STATE_TURN_LEFT:
          turn(left);
          g_driveState = STATE_DRIVE_FORWARD;
          look(FORWARD);
          break;
          
          case STATE_TURN_RIGHT:
          turn(right);
          g_driveState = STATE_DRIVE_FORWARD;
          look(FORWARD);
          g_servoState = STATE_SERVO_UNDEFINED;
          break;

     }

   
   }
}

  

       







//---------------------------------------------------------------------------------------------
void setDirection (int d)
{
if (d == 0)
{
  digitalWrite(PIN_MOTORRECHTS_POL1, HIGH);
  digitalWrite(PIN_MOTORRECHTS_POL2, LOW);

  digitalWrite(PIN_MOTORLINKS_POL1, HIGH);
  digitalWrite(PIN_MOTORLINKS_POL2, LOW);
}
else if (d == 1)
{ 
  digitalWrite(PIN_MOTORRECHTS_POL1, LOW);
  digitalWrite(PIN_MOTORRECHTS_POL2, HIGH);

  digitalWrite(PIN_MOTORLINKS_POL1, LOW);
  digitalWrite(PIN_MOTORLINKS_POL2, HIGH);
}
}
//---------------------------------------------------------------------------------------------
unsigned int getDistance ()
{
// Clear the trigPin by setting it LOW:
  digitalWrite(PIN_SONIC_TRIGGER, LOW);
  delayMicroseconds(5);

  // Trigger the sensor by setting the trigPin high for 10 microseconds:
  digitalWrite(PIN_SONIC_TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_SONIC_TRIGGER, LOW);

  // Read the echoPin, pulseIn() returns the duration (length of the pulse) in microseconds:
  unsigned long duration = pulseIn(PIN_SONIC_ECHO, HIGH);
  // Calculate the distance:
  unsigned int distance = duration*0.034/2;
  Serial.print("Distance = ");
  Serial.print(distance);
  Serial.println(" cm");
  return distance;  
}
//---------------------------------------------------------------------------------------------
void drive(int s)
{
  if (s == stay)
  {
    analogWrite(PIN_MOTORRECHTS_ENABLE, SPEED_STOP);

    analogWrite(PIN_MOTORLINKS_ENABLE, SPEED_STOP);  
  } 

  else if (s == slow)
  {
    analogWrite(PIN_MOTORRECHTS_ENABLE, SPEED_SLOW);

    analogWrite(PIN_MOTORLINKS_ENABLE, SPEED_SLOW);  
  }

  else if (s == moderate)
  {
    analogWrite(PIN_MOTORRECHTS_ENABLE, SPEED_MODERATE);

    analogWrite(PIN_MOTORLINKS_ENABLE, SPEED_MODERATE);  
  }

  else if (s == full)
  {
    analogWrite(PIN_MOTORRECHTS_ENABLE, SPEED_FULL);

    analogWrite(PIN_MOTORLINKS_ENABLE, SPEED_FULL); 
  }
}

void turn (int r)
{
 if (r == left)
 {

      analogWrite(PIN_MOTORRECHTS_ENABLE, SPEED_STOP);
      digitalWrite(PIN_MOTORRECHTS_POL1, HIGH);
      digitalWrite(PIN_MOTORRECHTS_POL2, LOW);

      analogWrite(PIN_MOTORLINKS_ENABLE, SPEED_MODERATE);  
      digitalWrite(PIN_MOTORLINKS_POL1, LOW);
      digitalWrite(PIN_MOTORLINKS_POL2, HIGH);
    
      delay(200);
  
      analogWrite(PIN_MOTORRECHTS_ENABLE, SPEED_STOP);
      digitalWrite(PIN_MOTORRECHTS_POL1, LOW);
      digitalWrite(PIN_MOTORRECHTS_POL2, LOW);
  
      analogWrite(PIN_MOTORLINKS_ENABLE, SPEED_STOP);  
      digitalWrite(PIN_MOTORLINKS_POL1, LOW);
      digitalWrite(PIN_MOTORLINKS_POL2, LOW);

 }
  else if (r == half_left)
  {

      analogWrite(PIN_MOTORRECHTS_ENABLE, SPEED_STOP);
      digitalWrite(PIN_MOTORRECHTS_POL1, HIGH);
      digitalWrite(PIN_MOTORRECHTS_POL2, LOW);

      analogWrite(PIN_MOTORLINKS_ENABLE, SPEED_MODERATE);  
      digitalWrite(PIN_MOTORLINKS_POL1, LOW);
      digitalWrite(PIN_MOTORLINKS_POL2, HIGH);
    
      delay(200);
  
      analogWrite(PIN_MOTORRECHTS_ENABLE, SPEED_STOP);
      digitalWrite(PIN_MOTORRECHTS_POL1, LOW);
      digitalWrite(PIN_MOTORRECHTS_POL2, LOW);
  
      analogWrite(PIN_MOTORLINKS_ENABLE, SPEED_STOP);  
      digitalWrite(PIN_MOTORLINKS_POL1, LOW);
      digitalWrite(PIN_MOTORLINKS_POL2, LOW);

  }
  else if (r == half_right)
  {

      analogWrite(PIN_MOTORRECHTS_ENABLE, SPEED_SLOW);
      digitalWrite(PIN_MOTORRECHTS_POL1, LOW);
      digitalWrite(PIN_MOTORRECHTS_POL2, HIGH);

      analogWrite(PIN_MOTORLINKS_ENABLE, SPEED_STOP);  
      digitalWrite(PIN_MOTORLINKS_POL1, HIGH);
      digitalWrite(PIN_MOTORLINKS_POL2, LOW);
    
      delay(250);
  
      analogWrite(PIN_MOTORRECHTS_ENABLE, SPEED_STOP);
      digitalWrite(PIN_MOTORRECHTS_POL1, LOW);
      digitalWrite(PIN_MOTORRECHTS_POL2, LOW);
  
      analogWrite(PIN_MOTORLINKS_ENABLE, SPEED_STOP);  
      digitalWrite(PIN_MOTORLINKS_POL1, LOW);
      digitalWrite(PIN_MOTORLINKS_POL2, LOW);
     
 
  }
  else if (r == right)
  {

      analogWrite(PIN_MOTORRECHTS_ENABLE, SPEED_MODERATE);
      digitalWrite(PIN_MOTORRECHTS_POL1, LOW);
      digitalWrite(PIN_MOTORRECHTS_POL2, HIGH);

      analogWrite(PIN_MOTORLINKS_ENABLE, SPEED_STOP);  
      digitalWrite(PIN_MOTORLINKS_POL1, HIGH);
      digitalWrite(PIN_MOTORLINKS_POL2, LOW);
    
      delay(130);
  
      analogWrite(PIN_MOTORRECHTS_ENABLE, SPEED_STOP);
      digitalWrite(PIN_MOTORRECHTS_POL1, LOW);
      digitalWrite(PIN_MOTORRECHTS_POL2, LOW);
  
      analogWrite(PIN_MOTORLINKS_ENABLE, SPEED_STOP);  
      digitalWrite(PIN_MOTORLINKS_POL1, LOW);
      digitalWrite(PIN_MOTORLINKS_POL2, LOW);

  }
  else if (r == rightaround)
  {

      analogWrite(PIN_MOTORRECHTS_ENABLE, SPEED_SLOW);
      digitalWrite(PIN_MOTORRECHTS_POL1, LOW);
      digitalWrite(PIN_MOTORRECHTS_POL2, HIGH);
  
      analogWrite(PIN_MOTORLINKS_ENABLE, SPEED_MODERATE);  
      digitalWrite(PIN_MOTORLINKS_POL1, HIGH);
      digitalWrite(PIN_MOTORLINKS_POL2, LOW);
      
      delay(600);
  
      analogWrite(PIN_MOTORRECHTS_ENABLE, SPEED_STOP);
      digitalWrite(PIN_MOTORRECHTS_POL1, LOW);
      digitalWrite(PIN_MOTORRECHTS_POL2, LOW);
  
      analogWrite(PIN_MOTORLINKS_ENABLE, SPEED_STOP);  
      digitalWrite(PIN_MOTORLINKS_POL1, LOW);
      digitalWrite(PIN_MOTORLINKS_POL2, LOW);

  }
  }

//---------------------------------------------------------------------------------------------
void look (int l)
{
  if (l == left)
  {
    sensordirection.write(180);
  }
  else if (l == half_left)
  {
    sensordirection.write(140);
  }
  else if (l == half_right)
  {
    sensordirection.write(40);
  }
  else if (l == right)
  {
    sensordirection.write(0);
  }
  else if (l == FORWARD)
  {
    sensordirection.write(90);
  }
}
//---------------------------------------------------------------------------------------------
