#include <Servo.h>

int ledRed = A0; // OFF, RED, TRI
int ledYellow = A1; // OFF, YELLOW, SQU
int ledBlue = A2; // OFF, BLUE, HEX

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

#define S0 2
#define S1 4
#define S2 7
#define S3 8
#define sensorOut 12

int frequencyRed = 0;
int frequencyGreen = 0;
int frequencyBlue = 0;

bool colorFind = false;
int color_ = -1; // -1 0 1 2 -> default, red, yellow, blue;

int OPEN = 1, CLOSE = 10;

void setup() {
  pinMode(ledRed, OUTPUT);
  pinMode(ledYellow, OUTPUT);
  pinMode(ledBlue, OUTPUT);

  servo1.attach(pinServo1);
  servo2.attach(pinServo2);
  servo3.attach(pinServo3);
  servo4.attach(pinServo4);
  servo5.attach(pinServo5);
  servo6.attach(pinServo6);

  servo5.write(60);
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);
  Serial.begin(9600);

  
  moveToStartforPickandPlace();
  delay(3000);
}

void loop() {
  PickNPlace();
}

void PickNPlace() {
  getFrequency();
  if (!colorFind) {
    checkColor();
  } else {
    moveBlock();
  }

}

void getFrequency() {
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  frequencyRed = pulseIn(sensorOut, LOW);
  Serial.print("Red = ");
  Serial.print(frequencyRed);
  Serial.print("  ");
  delay(100);
  
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  frequencyGreen = pulseIn(sensorOut, LOW);
  Serial.print("Green= ");
  Serial.print(frequencyGreen);
  Serial.print("  ");
  delay(100);
  
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  frequencyBlue = pulseIn(sensorOut, LOW);
  Serial.print("Blue= ");
  Serial.print(frequencyBlue);
  Serial.println("  ");
  delay(100);
}

bool isRed(int R, int G, int B){ return ((R < 45 && R > 20) && (G < 100 && G > 65) && (B < 75 && B > 45)); }

bool isYellow(int R, int G, int B) { return ((R < 40 && R > 18) && (G < 45 && G > 20) && (B < 55 && B > 27)); }

bool isBlue(int R, int G, int B) { return ((R < 110 && R > 55)&& (G < 65 && G > 20) && (B < 45 && B > 15)); }

void checkColor() {
  digitalWrite(ledRed, LOW);
  digitalWrite(ledYellow, LOW);
  digitalWrite(ledBlue, LOW);
  
  if (isRed(frequencyRed, frequencyGreen, frequencyBlue)) {
    digitalWrite(ledRed, HIGH);
    colorFind = true;
    color_ = 0;
  } else if (isYellow(frequencyRed, frequencyGreen, frequencyBlue)) {
    digitalWrite(ledYellow, HIGH);
    colorFind = true;
    color_ = 1;
  } else if (isBlue(frequencyRed, frequencyGreen, frequencyBlue)) {
    digitalWrite(ledBlue, HIGH);
    colorFind = true;
    color_ = 2;
  } else {
    colorFind = false;
    color_ = -1;
  }
}

void moveToStartforPickandPlace() {
  servo1.write(10);
  servo3.write(150);
  servo4.write(50);
  servo5.write(100);
  servo6.write(OPEN);
}

void moveBlock() {
  if(color_==-1) return;
  
  //// Initial
  moveToStartforPickandPlace();
  delay(2000);
  
  //// Pick
  setServoPosition(servo1,40,10);
  setServoPosition(servo3,150,120);
  setServoPosition(servo4,50,70);
  //setServoPosition(servo5,100,100);
  setServoPosition(servo6,OPEN,CLOSE);
  delay(2000);
  
  //// Up
  setServoPosition(servo1,10,40);
  setServoPosition(servo3,120,150);
  setServoPosition(servo4,70,50);
  //setServoPosition(servo5,100,100);
  //setServoPosition(servo6,CLOSE,CLOSE);
  delay(2000);

  // Go to desired position based on color
  if(color_ == 0) {

    //// Red Go
    setServoPosition(servo1,40,70);
    //setServoPosition(servo3,150,150);
    //setServoPosition(servo4,50,50);
    //setServoPosition(servo5,100,100);
    //setServoPosition(servo6,CLOSE,CLOSE);
    delay(2000);

    /// Red Fall
    //setServoPosition(servo1,70,70);
    //setServoPosition(servo3,150,150);
    //setServoPosition(servo4,50,50);
    //setServoPosition(servo5,100,100);
    setServoPosition(servo6,CLOSE,OPEN);
    delay(2000);
    
  } else if(color_ == 1) {
    
    //// Yellow Go
    setServoPosition(servo1,40,100);
    //setServoPosition(servo3,150,150);
    //setServoPosition(servo4,50,50);
    //setServoPosition(servo5,100,100);
    //setServoPosition(servo6,CLOSE,CLOSE);
    delay(2000);

    /// Yellow Fall
    //setServoPosition(servo1,70,70)
    //setServoPosition(servo3,150,150);
    //setServoPosition(servo4,50,50);
    //setServoPosition(servo5,100,100);
    setServoPosition(servo6,CLOSE,OPEN);
    delay(2000);
    
  } else if(color_ == 2) {
    
    //// Blue Go
    setServoPosition(servo1,40,135);
    //setServoPosition(servo3,150,150);
    //setServoPosition(servo4,50,50);
    //setServoPosition(servo5,100,100);
    //setServoPosition(servo6,CLOSE,CLOSE);
    delay(2000);

    /// Blue Fall
    //setServoPosition(servo1,70,70)
    //setServoPosition(servo3,150,150);
    //setServoPosition(servo4,50,50);
    //setServoPosition(servo5,100,100);
    setServoPosition(servo6,CLOSE,OPEN);
    delay(2000);
  }

  // Return Servo to Home position
  moveToStartforPickandPlace();

  colorFind = false;
}

void setServoPosition(Servo servo, int startPosition, int endPosition) {
  if (startPosition > endPosition) {
    for (int i = startPosition; i >= endPosition; i--) {
      servo.write(i);
      delay(30);
    }
  } else {
    for (int i = startPosition; i <= endPosition; i++) {
      servo.write(i);
      delay(30);
    }
  }
}
