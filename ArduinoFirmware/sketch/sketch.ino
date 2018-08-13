/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/


#include <Servo.h>


#define SERVO_NUM 2 
#define SERVO_PIN_START 8 

Servo servos[SERVO_NUM];


String inString = "";

//0: POT Mode
//1: Srial Mode Servo Select
//2: Srial Mode Servo POS Entry
//3: Serial Mode Servo control
int mode = 0;   

int servoSelected = 0;
bool exitMode = false;



int pos = 0;    // variable to store the servo position
int val = 0;







void potControl()
{
  val = analogRead(0);
  pos = map(val, 0, 1023, 0, 180);

  Serial.println(pos); 

  for(int i = 0; i<SERVO_NUM; i++){
    servos[i].write(pos);
  }

  delay(5);  
}


void serialControl()
{
  val = inString.toInt();
  pos = map(val, 0, 1023, 0, 180);
  
  servos[servoSelected].write(pos); 
}






void setup() {
	for(int i = 0; i<SERVO_NUM; i++){
		servos[i].attach(SERVO_PIN_START+i);
	}

	Serial.begin(9600);
}





void loop() {    
	while (Serial.available() > 0) 
	{
	  handleSerial();	
	}
 
	if(mode == 0)   //0: POT Mode
	{
		potControl();
	}

  if(mode == 3)
  {
    mode == 0;
  }
}



void handleSerial(){  
	exitMode = false;
	int inByte = Serial.read();

  if (inByte == '\n')
  {
    Serial.println("Skipping char"); 
    Serial.print("Current mode:"); 
    Serial.println(mode); 
    return;
  }

	if(mode == 0)
	{  //0: POT Mode
		
		Serial.println("In mode 0"); 
		if(inByte == '+')
		{
			mode = 1;
			exitMode = true;
		}
	}

	if(mode == 1 && !exitMode)
	{  //1: Srial Mode Servo Select
		Serial.println("In mode 1"); 
		if(inByte == '-')
		{
			mode = 0;
			exitMode = true;
		}
		else
		{
			if(inByte < 'a' && inByte > ('a' + SERVO_NUM))
			{
				mode = 0;
				exitMode = true;
			}
			else
			{
				servoSelected = inByte - 65;
				mode = 2;
				exitMode = true;
				inString = "";
			}			
		}
	}
		
	if(mode == 2 && !exitMode)
	{ //2: Srial Mode Servo POS Entry
		Serial.println("In mode 2"); 
		if(isDigit(inByte))
		{
			inString += (char)inByte;
		}
		else
		{
			mode = 3;
			exitMode = true;
		}
		
	}
	
	if(mode == 3 && !exitMode) 
	{  //3: Serial Mode Servo control
		Serial.println("In mode 3"); 
		serialControl();
		mode = 1;
		exitMode = true;
	}


}


