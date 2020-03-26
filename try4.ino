# include "includes.h"
# include <Servo.h> 

Servo sensordirection;

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

  float distance = 1.0 ;
  float short1 = 1.0 ;
  float short2 = 1.0 ;


void loop() {


  // getDistance ();
  distance = getDistance ();

  if (distance > 25)
  {
    setDirection (FORWARD);
    drive (full);
     Serial.print("distance > 20: Distance = ");
     Serial.print(distance);
     Serial.println(" cm");


  }
  
  else
  {
    drive (stay);
    look (left);
    delay (500);
    short1 = getDistance ();
    delay (500);

     Serial.print("distance left: Distance = ");
     Serial.print(short1);
     Serial.println(" cm");
    
    look (right);
    delay (500);
    short2 = getDistance ();
    delay (500);
    
     Serial.print("distance right: Distance = ");
     Serial.print(short2);
     Serial.println(" cm");
    
    look (FORWARD);
   
    
    if (short1 <= 5 || short2 <= 5)
    {
      if (short1 <= 5)
      {
        turn (leftaround);
      }
      else if (short2 <= 5)
      {
        turn (rightaround);
      }
    }
    else if (short1 > short2)
    {
      Serial.println("short1 > short2");
      turn(left);

    }
    
    else if (short1 < short2)
    {
     Serial.println("short1 < short2");
     
     turn(right);

    }
    else 
    {
      Serial.println("else");
      turn(rightaround);

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
float getDistance ()
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
  float distance = duration*0.034/2;
 // Serial.print("Distance = ");
 // Serial.print(distance);
 // Serial.println(" cm");
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
    
      delay(600);
  
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
    
      delay(300);
  
      analogWrite(PIN_MOTORRECHTS_ENABLE, SPEED_STOP);
      digitalWrite(PIN_MOTORRECHTS_POL1, LOW);
      digitalWrite(PIN_MOTORRECHTS_POL2, LOW);
  
      analogWrite(PIN_MOTORLINKS_ENABLE, SPEED_STOP);  
      digitalWrite(PIN_MOTORLINKS_POL1, LOW);
      digitalWrite(PIN_MOTORLINKS_POL2, LOW);
     
 
  }
  else if (r == right)
  {

      analogWrite(PIN_MOTORRECHTS_ENABLE, SPEED_SLOW);
      digitalWrite(PIN_MOTORRECHTS_POL1, LOW);
      digitalWrite(PIN_MOTORRECHTS_POL2, HIGH);

      analogWrite(PIN_MOTORLINKS_ENABLE, SPEED_STOP);  
      digitalWrite(PIN_MOTORLINKS_POL1, LOW);
      digitalWrite(PIN_MOTORLINKS_POL2, HIGH);
    
      delay(600);
  
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
    sensordirection.write(135);
  }
  else if (l == half_right)
  {
    sensordirection.write(45);
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
