/*a genius code manual
 * 1[1 - 2] - FLASHLIGTHS MODE
 * 11 - FLASHLIGHTS ON
 * 12 - FLASHLIGHTS OFF
 * 2[1 - 7] - MOTORS MODE
 * 2[1 - 5] - SET MOTORS SPEED
 * 26 - 1ST MOTORS TRAECTORY
 * 27 - 2ND MOTORS TRAECTORY
 * 3[1-8] - ULTRASOUND MODE, SET TIME (ITERATIONS COUNT)
 * 39 - INFINITY US MODE
 * 4[1-8] - ULTRASOUND MODE, SET TIME (ITERATIONS COUNT)
 * 49 - INFINITY US MODE
 * 5[0 - 9] - STOP ALL PROCESS
 * thank u for using this robot
 */



#include <avr/sleep.h>
#include <avr/power.h>
#include <Wire.h>
#include <MPU6050.h>
#include <I2Cdev.h>
#include <Kalman.h>
#include <NewPing.h>
MPU6050 accelgyro;
int16_t ax, ay, az;
int16_t gx, gy, gz;

//input from pc
int inp;
int globalInp;
int a;

//const values
int lightPin_1 = LED_BUILTIN; //1st light
int lightPin_2 = 8; //2nd light

#define PIN_TRIG 12
#define PIN_ECHO 11
#define MAX_DISTANCE 200

String val;

//ultrasound
NewPing sonar(PIN_TRIG, PIN_ECHO, MAX_DISTANCE);

//motors
int motorsEn_12 = 10;
int motorsEn_34 = 9;
int motor1A = 4;
int motor2A = 5;
int motor3A = 6;
int motor4A = 7;
int speed1 = 250; // speed 1 - 255
int speed2 = 250; // speed 1 - 255

uint32_t timer;

void setup() {
  Serial.setTimeout(50);
  //connection
  Serial.begin(9600);
  //lights
  pinMode(lightPin_1, OUTPUT);
  pinMode(lightPin_2, OUTPUT);
  //motors
  pinMode(motor1A, OUTPUT);
  pinMode(motor2A, OUTPUT);
  pinMode(motor3A, OUTPUT);
  pinMode(motor4A, OUTPUT);

  Wire.begin();
  timer = micros();
  accelgyro.initialize();
}

void loop() {  
  if (Serial.available() > 0) {
    a = Serial.parseInt();
    globalInp = a / 10;
    if (globalInp != 0) {
      Serial.print("Introduced: ");
      Serial.println(globalInp);
    } if (globalInp == 1) {
        Serial.println("Light Mode");
        //lights
        if (Serial.available() > 0) {
          inp = a % 10;
          if (inp != 0) {
            Serial.println(inp);
            if (inp == 1) {
              digitalWrite(lightPin_1, HIGH);
              digitalWrite(lightPin_2, HIGH);
            }
            else if (inp == 2) {
              digitalWrite(lightPin_1, LOW);
              digitalWrite(lightPin_2, LOW);
            }
        }
      }
    }
    else if (globalInp == 2) {
      Serial.println("Motors Mode");
      inp = a % 10;
      if (inp != 0) {
        //motors
        if (inp > 0 && inp < 6) {
          analogWrite(motorsEn_12, inp * 50);
          analogWrite(motorsEn_34, inp * 50);
        }
        else if (inp == 6) {
        digitalWrite(motor1A, HIGH);
        digitalWrite(motor2A, LOW);
        digitalWrite(motor3A, HIGH);
        digitalWrite(motor4A, LOW);
        } else if (inp == 7) {
          digitalWrite(motor1A, LOW);
          digitalWrite(motor2A, HIGH);
          digitalWrite(motor3A, LOW);
          digitalWrite(motor4A, HIGH);
        }
      }
    } else if (globalInp == 3) {
        Serial.println("Ultrasound Mode");
        //ultrasound
        inp = a % 10;
        if (inp != 0 && inp != 9) {
          for (int i = 0; i <= inp * 10; i++) {
            delay(1000);
            unsigned int distance = sonar.ping_cm();
            Serial.println(distance); //edit
          }
        } else if (inp == 9) {
          for (int i = 0; i > -1; i++) {
            delay(1000);
            unsigned int distance = sonar.ping_cm();
            Serial.println(distance); //edit
          }
        }
    } else if (globalInp == 4) {
        inp = a % 10;
        if (inp != 0 && inp != 9) {
          for (int i = 0; i <= inp * 10; i++) {
            accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
            Serial.println("ax, ay, az, gx, gy, gz");
            Serial.print(ax); Serial.print("\t");
            Serial.print(ay); Serial.print("\t");
            Serial.print(az); Serial.print("\t");
            Serial.print(gx); Serial.print("\t");
            Serial.print(gy); Serial.print("\t");
            Serial.println(gz);
         }
        } else if (inp == 9) {
          for (int i = 0; i > -1; i++) {
            accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
            Serial.println("ax, ay, az, gx, gy, gz");
            Serial.print(ax); Serial.print("\t");
            Serial.print(ay); Serial.print("\t");
            Serial.print(az); Serial.print("\t");
            Serial.print(gx); Serial.print("\t");
            Serial.print(gy); Serial.print("\t");
            Serial.println(gz);
         }
       }
    } else if (globalInp == 5) {
      cli();
      sleep_enable();
      sleep_cpu();
    }
  }
}
