/* Finding the key codes for your remote. More info: https://www.makerguides.com */
#include <IRremote.h> // include the IRremote library
#define RECEIVER_PIN 2 // define the IR receiver pin
IRrecv receiver(RECEIVER_PIN); // create a receiver object of the IRrecv class
decode_results results; // create a results object of the decode_results class


#include <Pixy2.h>//https://github.com/charmedlabs/pixy2/raw/master/releases/arduino/arduino_pixy2-1.0.3.zip
//Variables
Pixy2 pixy;

/*
 # Edited by:  Matt
 # Date:       2015.09.06
 # Version:    1.1
 # Product:    Cherokey 4WD Mobile Platform
 # SKU:        ROB0102/ROB0117

 # Description:
 # Drive 2 motors with this Cherokey 4WD Mobile Platform
 # Connect D4,D5,D6,D7,GND to UNO digital 4,5,6,7,GND

*/


//Motor Definitions
int E1 = 5;     //M1 Speed Control
int E2 = 6;     //M2 Speed Control
int M1 = 7;     //M1 Direction Control
int M2 = 4;     //M2 Direction Control
float T ;

int speedPin_M1 = 5;     //M1 Speed Control
int speedPin_M2 = 6;     //M2 Speed Control
int directionPin_M1 = 4;     //M1 Direction Control
int directionPin_M2 = 7;     //M1 Direction Control   

const int trigPin1 = 42;
const int echoPin1 = 43;

const int trigPin2 = 12;
const int echoPin2 = 13;

#include <Ultrasonic.h>


Ultrasonic sensor1(trigPin1, echoPin1);
Ultrasonic sensor2(trigPin2, echoPin2);

//DIRECTIONS


void carStop(){              
  Serial.println("stop ");   //  Motor Stop
  digitalWrite(speedPin_M2,0);
  digitalWrite(directionPin_M1,LOW);
  digitalWrite(speedPin_M1,0);
  digitalWrite(directionPin_M2,LOW);
}

void carAdvance(int leftSpeed,int rightSpeed){   
  Serial.println("avancer ");    //Move forward
  analogWrite (speedPin_M2,leftSpeed);
  digitalWrite(directionPin_M1,HIGH);
  analogWrite (speedPin_M1,rightSpeed);
  digitalWrite(directionPin_M2,HIGH);
}


//STOP
void stop(void)
{
  Serial.println("stop ");
  digitalWrite(E1, 0);
  digitalWrite(M1, LOW);
  digitalWrite(E2, 0);
  digitalWrite(M2, LOW);
}

//ADVANCE
void advance(char a, char b)
{
  Serial.println("avancer ");
  analogWrite (E1, a);
  digitalWrite(M1, LOW);
  analogWrite (E2, b);
  digitalWrite(M2, HIGH);
}

//MOVE BACKWARDS
void back_off (char a, char b)
{
  Serial.println("bzckoff ");
  analogWrite (E1, a);
  digitalWrite(M1, HIGH);
  analogWrite (E2, b);
  digitalWrite(M2, LOW);
}


//TURN LEFT
void turn_L (char a, char b)
{
  Serial.println("avancer ");
  Serial.println("tot");
  analogWrite (E1, b);
  digitalWrite(M1, HIGH);
  analogWrite (E2, a);
  digitalWrite(M2, HIGH);
}

//TURN RIGHT
void turn_R (char a, char b)
{
  analogWrite (E1, a);
  digitalWrite(M1, LOW);
  analogWrite (E2, b);
  digitalWrite(M2, LOW);
}



void setup() {
  Serial.begin(9600); // begin serial communication with a baud rate of 9600
  digitalWrite(E1, LOW);
  digitalWrite(E2, LOW);
  receiver.enableIRIn(); // enable the receiver
  receiver.blink13(true); // enable blinking of the built-in LED when an IR signal is received
  Ultrasonic sensor1(trigPin1, echoPin1);
  Ultrasonic sensor2(trigPin2, echoPin2);
  pixy.init();
}


void loop() {


    if (receiver.decode(&results)) { // decode the received signal and store it in results
    Serial.println(results.value); // print the values in the Serial Monitor
    
      if ((results.value) == 805878) {
        advance( 30, 180);
        Serial.print("tourner a roite ");
      }

      if ((results.value) == 281590) {
        advance( 200, 30);
      }
      if ((results.value) == 52214) {
        // POUR AVANCER
        //carAdvance(255,255);
        //delay(750);
        advance( 251, 200);
      }
      if ((results.value) == 576502) {
        back_off( 251, 200);
      }
      if ((results.value) == 117750) {
        // ARRET DE LA VOITURE
        //carStop();  
        stop();
      }
      if ((results.value) == 314358) {

        
        while ((results.value) != 117750)
        {
        delay(10);

        float distance2 = sensor1.read();
        float distance1 = sensor2.read();
        if (1 > 21.5) {
          advance( 140, 200);
          delay (100);
          stop();
        }

        if (21.5 > 19.5) {  
          pixy.ccc.getBlocks();
          // If there are detect blocks, print them!
          if (pixy.ccc.numBlocks) {
            Serial.print("Detected ");
            Serial.println(pixy.ccc.numBlocks);
            for (int i = 0; i < pixy.ccc.numBlocks; i++) {
              Serial.print(" 	block ");
              Serial.print(i);
              Serial.print(": ");
              int v = pixy.ccc.blocks[i].m_x;
              float T = (float)v / 320;

              Serial.println(T) ;
              if (T > 0.499) {
            advance( int (T*255), int(T*200));
            delay (100);
            stop();
          }
          if (T < 0.498 && T != 0) {
            back_off( int ((1-T)*255), int((1-T)*200));
            delay (100);
            stop();
          }
            }
        }
        }

        if (100 < 19.5) {
          advance(251, 140);
          delay (100);
          stop();
        }

        if (422 < 20) {
          break;
        }
        if (receiver.decode(&results)) { // decode the received signal and store it in results
          Serial.println("result:" + results.value); // print the values in the Serial Monitor
          if ((results.value) == 117750) {
            break;
          }}
          receiver.resume(); // reset the receiver for the next code
        }

        receiver.resume(); // reset the receiver for the next code
      }
      receiver.resume(); // reset the receiver for the next code
    }
  
}
