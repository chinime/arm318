#include <Vector.h>
#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;
Servo servo6;

int pinServo1 = 3;
int pinServo2 = 5;
int pinServo3 = 6;
int pinServo4 = 9;
int pinServo5 = 10;
int pinServo6 = 11;

const unsigned int SIZE = 5;

int CLOSE = 10;

int TRIJV[SIZE][3] = {{30,150,10},  // INITIAL
                      {30,90,15},  // 3
                      {40,110,45}, // 2
                      {35,130,100}, // New
                      {30,90,15}  // 3
                      };

int startPosTriangle[3] = {30,130,10};

void setup() {
  servo1.attach(pinServo1);
  servo2.attach(pinServo2);
  servo3.attach(pinServo3);
  servo4.attach(pinServo4);
  servo5.attach(pinServo5);
  servo6.attach(pinServo6);

  moveToStartforShapeDrawingTriangle();
}

void loop() {
  drawTriangle();
}

void moveToStartforShapeDrawingTriangle() {
  servo5.write(40);
  servo1.write(startPosTriangle[0]);
  servo3.write(startPosTriangle[1]);
  servo4.write(startPosTriangle[2]);
}

void drawTriangle() {
  moveToStartforShapeDrawingTriangle();
  
  int currPos[3];
  
  for(int i = 0; i < SIZE; i++) {
    for(int j = 0; j < 3; j++) { currPos[j] = TRIJV[i][j]; }
    
    setServoPositionforShape(currPos);

    delay(1000);
  }

  moveToStartforShapeDrawingTriangle();
}

void setServoPositionforShape(int currPos[]) {

  servo1.write(currPos[0]);
  servo3.write(currPos[1]);
  servo4.write(currPos[2]);
}
