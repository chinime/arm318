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

const unsigned int SIZE = 8;

int CLOSE = 10;

int SQRJV[SIZE][3] = {{40,110,30},
                      {40,110,45}, 
                      {30,90,15}, 
                      {20,85,20},
                      {20,120,90},
                      {30,130,90},
                      {40,130,90},
                      {44,130,90}};

int startPosSquare[3] = {40,110,30};

void setup() {
  servo1.attach(pinServo1);
  servo2.attach(pinServo2);
  servo3.attach(pinServo3);
  servo4.attach(pinServo4);
  servo5.attach(pinServo5);
  servo6.attach(pinServo6);

  moveToStartforShapeDrawingSquare();
}

void loop() {
  drawSqaure();
}

void moveToStartforShapeDrawingSquare() {
  servo5.write(60);
  servo1.write(startPosSquare[0]);
  servo3.write(startPosSquare[1]);
  servo4.write(startPosSquare[2]);
}

void drawSqaure() {
  moveToStartforShapeDrawingSquare();
  
  int currPos[3];
  
  for(int i = 0; i < SIZE; i++) {
    for(int j = 0; j < 3; j++) { currPos[j] = SQRJV[i][j]; }
    
    setServoPositionforShape(currPos);

    delay(1000);
  }

  moveToStartforShapeDrawingSquare();
}

void setServoPositionforShape(int currPos[]) {

  servo1.write(currPos[0]);
  servo3.write(currPos[1]);
  servo4.write(currPos[2]);
}
