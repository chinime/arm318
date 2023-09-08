#include <ros.h>
#include <rospy_tutorials/Floats.h>
#include <rospy_tutorials/Int32.h>
#include <Servo.h>

#include <StandardCplusplus.h>
#include <serstream>
#include<vector>
#include<iterator>

int pickNPlaceMode = A3;
int drawingMode = A4;
int manaulMode = A5;

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
int pinServo4 = 9
int pinServo5 = 10;
int pinServo6 = 11;

ros::NodeHandle nh;
ros::Subscriber<rospy_tutorials::Floats> sub("/arm_command",servo_cb2);

int mode_ = -1; // Controlling: -1 0 1 2 -> Stale, Manual, Pick and Place, Drawing

int OPEN = 10, CLOSE = 180;

// Pick and Place Specific constants
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

// Shape Drawing Specific
int shape_ = -1; // -1, 0, 1, 2 -> default, triangle, square, hexagon

// Manual Mode Specific 
ros::Subscriber<rospy_tutorials::Floats> sub("/joints_to_aurdino",servo_cb);

bool set = false;
bool goAheadManualMode = false;

void setup() {
  pinMode(pickNPlaceMode, OUTPUT);
  pinMode(drawingMode, OUTPUT);
  pinMode(manaulMode, OUTPUT);
  
  pinMode(ledRed, OUTPUT);
  pinMode(ledYellow, OUTPUT);
  pinMode(ledBlue, OUTPUT);

  servo1.attach(pinServo1);
  servo2.attach(pinServo2);
  servo3.attach(pinServo3);
  servo4.attach(pinServo4);
  servo5.attach(pinServo5);
  servo6.attach(pinServo6);

  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);
  Serial.begin(9600);
}

void loop() { nh.spinOnce(); }

##############################
######## CONTROLLER ##########
##############################

void servo_cb2(const rospy_tutorials::Int32& cmd) {
  mode_ = cmd;
  
  if(cmd==0) {
    set = false;
    digitalWrite(pickNPlaceMode, LOW);
    digitalWrite(drawingMode, LOW);
    digitalWrite(manaulMode, HIGH);
    
    if(!set) {
      moveToStartforManualMode();
      delay(3000);
      set = true;
    } else {
      goAheadManualMode = true;
    }
  } else if(cmd==1) {
    goAheadManualMode = false;
    set = false;
    
    digitalWrite(pickNPlaceMode, HIGH);
    digitalWrite(drawingMode, LOW);
    digitalWrite(manaulMode, LOW);
    
    if(!set) { // Simulate setup
      moveToStartforPickandPlace();
      delay(3000);
      set = true;
    } else {
      PickNPlace();
    }
    
  } else if((cmd==2) || (cmd==3) || (cmd==4)) {
    goAheadManualMode = false;
    set = false;

    digitalWrite(pickNPlaceMode, LOW);
    digitalWrite(drawingMode, HIGH);
    digitalWrite(manaulMode, LOW);
    
    if(!set) {
      moveToStartforShapeDrawing();
      delay(3000);
      set = true;
    } else {
      drawShape(cmd-2);
    }
  }
}

#########################################
############### START ###################
############ PICK AND PLACE  ############
#########################################

void PickNPlace() {
  getFrequency();
  if (!colorFind) {
    checkColor();
  } else {
    moveBlock(color_);
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

void moveBlock(int color) {
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
  if(color == 0) {

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
    
  } else if(color == 1) {
    
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
    
  } else if(color == 2) {
    
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


#################################
########### START ###############
######## SHAPE DRAWING ##########
#################################

vector<vector<float>> SQRJV = {{},
                               {},
                               {}};
vector<vector<float>> TRIJV = ;

vector<vector<float>> HEXJV = ;

vector<int> startPos = {};

void moveToStartforShapeDrawing() {
  servo1.write(startPos[0]);
  // servo2.write();
  servo3.write(startPos[1]);
  servo4.write(startPos[2]);
  servo5.write(startPos[3]);
  servo6.write(startPos[4]);
}

void drawSqaure() {
  moveToStartforShapeDrawing();
  delay(3000);
  
  vector<int> currPosition;

  prevPos = startPos;

  for(int i = 0; i < SQRJV.size(); i++) {
    setServoPosition(servo1, prevPos[0], SQRJV[i][0]);
    setServoPosition(servo3, prevPos[1], SQRJV[i][1]);
    setServoPosition(servo4, prevPos[2], SQRJV[i][2]);
    setServoPosition(servo5, prevPos[3], SQRJV[i][3]);
    setServoPosition(servo6, prevPos[4], SQRJV[i][4]);
    
    prevPos = SQRJV[i];
  }
}

void drawTriangle() {
  moveToStartforShapeDrawing();
  delay(3000);
  
  vector<int> currPosition;

  prevPos = startPos;

  for(int i = 0; i < TRIJV.size(); i++) {
    setServoPosition(servo1, prevPos[0], TRIJV[i][0]);
    setServoPosition(servo3, prevPos[1], TRIJV[i][1]);
    setServoPosition(servo4, prevPos[2], TRIJV[i][2]);
    setServoPosition(servo5, prevPos[3], TRIJV[i][3]);
    setServoPosition(servo6, prevPos[4], TRIJV[i][4]);
    
    prevPos = TRIJV[i];
  }
}

void drawHexagon() {
  moveToStartforShapeDrawing();
  delay(3000);
  
  vector<int> currPosition;

  prevPos = startPos;

  for(int i = 0; i < HEXJV.size(); i++) {
    setServoPosition(servo1, prevPos[0], HEXJV[i][0]);
    setServoPosition(servo3, prevPos[1], HEXJV[i][1]);
    setServoPosition(servo4, prevPos[2], HEXJV[i][2]);
    setServoPosition(servo5, prevPos[3], HEXJV[i][3]);
    setServoPosition(servo6, prevPos[4], HEXJV[i][4]);
    
    prevPos = SQRJV[i];
  }
}

void drawShape(int cmd) {
  if(cmd==0) drawTriangle();
  else if(cmd==1) drawSquare();
  else if(cmd==2) drawHexagon();
}

#################################
########## START ################
####### MANUAL MODE #############
#################################

int global_cur_pos[5]={0,0,0,0,0};

void rotate_servo(int servo,int new_pos,int cur_pos,int dir) {
  int pos = 0;
  
  if (servo==0) {
    // nh.loginfo("Servo1");
    // new_pos=new_pos-5;
    
    if (new_pos <= 0) new_pos=0;
    if (new_pos > 70) new_pos=70;
      
    if (dir == 1) {
      for(pos=cur_pos;pos<= new_pos;pos += 1) {
        servo1.write(pos);
        delay(10);
      }      
    } else if (dir == -1) {
      for(pos=cur_pos; pos >= new_pos; pos -= 1) {
        servo1.write(pos);
        delay(10);
      }      
    }
    //servo1.write(new_pos);
  }
  
//  if (servo==1)
//  {
//    // nh.loginfo("Servo2"); 
//    // new_pos=new_pos+5;
//    
//    if (new_pos <= 0) new_pos=16;
//    else if(new_pos > 158) new_pos=158;
//    if (dir == 1) {
//      for(pos=cur_pos;pos<= new_pos;pos += 1) {
//        servo2.write(pos);
//        delay(10);
//      }      
//    } else if(dir == -1) {
//      for(pos=cur_pos;pos>= new_pos;pos -= 1) {
//        servo2.write(pos);
//        delay(10);
//      }      
//    }
//    //servo2.write(new_pos);
//  }
  
  if (servo==1) {
    // nh.loginfo("Servo3");
    // new_pos=new_pos+10;
   
    if (new_pos<90) new_pos=90;
    if (dir == 1) {
      for(pos=cur_pos;pos<= new_pos;pos += 1) {
        servo3.write(pos);
        delay(10);
      }      
    }
    else if(dir == -1) {
      for(pos=cur_pos;pos>= new_pos;pos -= 1) {
        servo3.write(pos);
        delay(10);
      }      
    }
    //servo3.write(new_pos);
  }
  
  if (servo==2) {
    //nh.loginfo("Servo4");
    if (new_pos<20) new_pos=20;
    if (new_pos>120) new_pos=120;
    
    if (dir == 1) {
      for(pos=cur_pos;pos<= new_pos;pos += 1) {
        servo4.write(pos);
        delay(10);
      }      
    } else if(dir == -1) {
      for(pos=cur_pos;pos>= new_pos;pos -= 1) {
        servo4.write(pos);
        delay(10);
      }      
    }
    //servo4.write(new_pos);
  }
  
  if (servo==3) {
    // nh.loginfo("Servo5");
    if (new_pos<0) new_pos=0;
    if (dir == 1) {
      for(pos=cur_pos;pos<= new_pos;pos += 1) {
        servo5.write(pos);
        delay(10);
      }      
    } else if(dir == -1) {
      for(pos=cur_pos;pos>= new_pos;pos -= 1) {
        servo5.write(pos);
        delay(10);
      }      
    }
    //servo5.write(new_pos);
  }
  
  if (servo==4) {
    // nh.loginfo("Servo6");
    if (new_pos<6) new_pos=6;
    if (dir == 1) {
      for(pos=cur_pos;pos<= new_pos;pos += 1) {
        servo6.write(pos);
        delay(10);
      }      
    } else if(dir == -1) {
      for(pos=cur_pos;pos>= new_pos;pos -= 1) {
        servo6.write(pos);
        delay(10);
      }      
    }
    servo6.write(new_pos);
  }
  
  global_cur_pos[servo]=pos;
}

void servo_cb( const rospy_tutorials::Floats& cmd_msg){
  //nh.loginfo("Command Received ");
  if(goAheadManualMode) {
    int new_pos[5]={cmd_msg.data[0],cmd_msg.data[1],cmd_msg.data[2],cmd_msg.data[3],cmd_msg.data[4]}; // servos: 1 3 4 5 6
    int i=0;
    
    for(i=0;i<5;i++) {
      if (new_pos[i]>global_cur_pos[i]) {
        rotate_servo(i,new_pos[i],global_cur_pos[i],1);
        //global_cur_pos[i]=new_pos[i];
      }
      else if(new_pos[i]<global_cur_pos[i]) {
        rotate_servo(i,new_pos[i],global_cur_pos[i],-1);
        //global_cur_pos[i]=new_pos[i];
      }
      //rotate_servo(i,new_pos[i],global_cur_pos[i],1);
      //delay(100);
    }
  }
}

void moveToStartforManualMode() {
  servo1.write(30); 
  //servo2.write(90); 
  servo3.write(120); 
  servo4.write(80);
  servo5.write(100);
  servo6.write(10);
}
