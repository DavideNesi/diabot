#include <Servo.h>

#define LEFT_SERVO_PIN 22
#define RIGHT_SERVO_PIN 24
#define NECK_SERVO_PIN 23
#define HEIGHT_SERVO_PIN 26
#define UPPER_ENDSTOP_PIN 28
#define LOWER_ENDSTOP_PIN 29

#define LEFT_SERVO_CENTER 90
#define RIGHT_SERVO_CENTER 90
#define NECK_SERVO_CENTER 90
#define HEIGHT_SERVO_CENTER 87

Servo leftServo;
Servo rightServo;
Servo heightServo;
Servo neckServo;

//L[1:5] R[1:5] N[1:5] H[1:2]

int currentStatus[4] = {3, 3, 3, 1};
int lastOrder[4] = {0, 0, 0, 0};
String receivedOrder = "3;3;3;1";

void setup() {
  Serial.begin(9600);

  Serial.setTimeout(50);

  leftServo.attach(LEFT_SERVO_PIN);
  rightServo.attach(RIGHT_SERVO_PIN);
  neckServo.attach(NECK_SERVO_PIN);
  heightServo.attach(HEIGHT_SERVO_PIN);

  pinMode(UPPER_ENDSTOP_PIN, INPUT_PULLUP);
  pinMode(LOWER_ENDSTOP_PIN, INPUT_PULLUP);
}

void loop() {

  if (Serial.available()) {
    receivedOrder = Serial.readString();
    //Serial.write("\nreceived");
  }

  lastOrder[0] = receivedOrder.charAt(0) - '0';
  lastOrder[1] = receivedOrder.charAt(2) - '0';
  lastOrder[2] = receivedOrder.charAt(4) - '0';
  lastOrder[3] = receivedOrder.charAt(6) - '0';

  

  int upperEndstopValue = digitalRead(UPPER_ENDSTOP_PIN);
  int lowerEndstopValue = digitalRead(LOWER_ENDSTOP_PIN);

  if (lastOrder[0] != 0) {
    leftServo.write(90 + (lastOrder[0] - 3) * 5);
  }else{
    leftServo.write(90 + (currentStatus[0] - 3) * 5);
  }
  
  if (lastOrder[1] != 0) {
    rightServo.write(90 - (lastOrder[1] - 3) * 5);
  }else{
    rightServo.write(90 - (currentStatus[1] - 3) * 5);
  }
  
  if (lastOrder[2] != 0) {
    neckServo.write(90 - (lastOrder[2] - 3) * 25);
  }else{
    neckServo.write(90 - (currentStatus[2] - 3) * 25);
  }

  if (lastOrder[3] != 0) {
    if (lastOrder[3] == 2) {
      if (lowerEndstopValue == 1) {
        heightServo.write(45);
      } else {
        heightServo.write(HEIGHT_SERVO_CENTER);
      }
    } else if (lastOrder[3] == 1) {
      if (upperEndstopValue == 1) {
        heightServo.write(135);
      } else {
        heightServo.write(HEIGHT_SERVO_CENTER);
      }
    }
  }else{
    if (currentStatus[3] == 2) {
      if (lowerEndstopValue == 1) {
        heightServo.write(45);
      } else {
        heightServo.write(HEIGHT_SERVO_CENTER);
      }
    } else if (currentStatus[3] == 1) {
      if (upperEndstopValue == 1) {
        heightServo.write(135);
      } else {
        heightServo.write(HEIGHT_SERVO_CENTER);
      }
    }
  }

  //TODO update status
  currentStatus[0] = (lastOrder[0] == 0) ? currentStatus[0] : lastOrder[0];
  currentStatus[1] = (lastOrder[1] == 0) ? currentStatus[1] : lastOrder[1];
  currentStatus[2] = (lastOrder[2] == 0) ? currentStatus[2] : lastOrder[2];
  currentStatus[3] = (lastOrder[3] == 0) ? currentStatus[3] : lastOrder[3];
}
