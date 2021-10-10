#include <Servo.h>


// ---------------------------------------------------------- PINOUTS ---------------------------------------------------------- 
// definition of the inputs and outputs on the Arduino Mega

// ----- SERVOMOTORS -----
//
//                O_O
//                 |
//      D - GAv----------DAv - A
//           |             |
//           |             |
//           |             |         ^
//           |             |         |
//           |             |         |
//           |             |
//           |             |
//      C - GAr----------DAr - B
//    
//    
//    

//-------- SERVOS --------
const int Servo_DAv =6;//ok
const int Servo_DAr =7;//ok
const int Servo_GAv =10;//ok
const int Servo_GAr =11;//ok

const int Servo_Offset_DAv =18;
const int Servo_Offset_DAr =18;
const int Servo_Offset_GAv =-10;
const int Servo_Offset_GAr =-25;

//-------- MOTORS --------
// motor DAv //ok
const int Mot_EN_DAv =4;
const int Mot_INA_DAv =44;
const int Mot_INB_DAv =46;
// motor DAr //ok
const int Mot_EN_DAr =5;
const int Mot_INA_DAr =42;
const int Mot_INB_DAr =40;
// motor GAv
const int Mot_EN_GAv =2;
const int Mot_INA_GAv =41;
const int Mot_INB_GAv =43;
// motor GAr //ok
const int Mot_EN_GAr =3;
const int Mot_INA_GAr =45;
const int Mot_INB_GAr =47;

//-------- OTHERS --------
//Relays A and B: A for servos power, B for motors power

const int Relay_A =52;
const int Relay_B =53;



//---- SERVO DEFINITION -----
Servo Servo_Droite_Avant;
Servo Servo_Droite_Arriere;
Servo Servo_Gauche_Avant;
Servo Servo_Gauche_Arriere;

void kill_switch() {
  digitalWrite(Relay_A, LOW);
  digitalWrite(Relay_B, LOW);
  Serial.println("MOVEMENT ENDED");
  return;
}

//set the dir of motors 1 forward, 0 backward
void set_motor_dir(int A, int B, int C, int D) {
   if(A==1) { //marche avant moteur DAv
      digitalWrite(Mot_INA_DAv, HIGH);
      digitalWrite(Mot_INB_DAv, LOW);
   }
   else if (A==0) { //marche
      digitalWrite(Mot_INA_DAv, LOW);
      digitalWrite(Mot_INB_DAv, HIGH);
   }
   
   if(B==1) { //marche avant moteur DAr
      digitalWrite(Mot_INA_DAr, HIGH);
      digitalWrite(Mot_INB_DAr, LOW);
   }
   else if(B==0) {
      digitalWrite(Mot_INA_DAr, LOW);
      digitalWrite(Mot_INB_DAr, HIGH);
   }

   if(C==1) { //marche avant moteur GAr
      digitalWrite(Mot_INA_GAr, HIGH);
      digitalWrite(Mot_INB_GAr, LOW);
   }
   else if (C==0){
      digitalWrite(Mot_INA_GAr, LOW);
      digitalWrite(Mot_INB_GAr, HIGH);
   }

   if(D==1) { //marche avant moteur GAv
      digitalWrite(Mot_INA_GAv, HIGH);
      digitalWrite(Mot_INB_GAv, LOW);
   }
   else if(D==0) {
      digitalWrite(Mot_INA_GAv, LOW);
      digitalWrite(Mot_INB_GAv, HIGH);
   }
   return;
}

void set_servo_dir(int A, int B, int C, int D) {
  //emptiing the buffer before reading the duration
  delay(10); 
  while(Serial.available() > 0) {
    Serial.read();
    delay(10);
  }
  digitalWrite(Relay_A, HIGH);
  Servo_Droite_Avant.write(A+Servo_Offset_DAv);
  Servo_Droite_Arriere.write(B+Servo_Offset_DAr);
  Servo_Gauche_Avant.write(D+Servo_Offset_GAv);
  Servo_Gauche_Arriere.write(C+Servo_Offset_GAr);
  //waiting for the servos to turn
  int previoustime=millis();        //waiting for period ms while beeing able to read the buffer looking for x; 
    while(millis()-previoustime < 500) {
      if(Serial.available()>0) {
        String text2;
        text2=Serial.readStringUntil(';');
        if(text2 == "x") {
          kill_switch();
          return;
        }
        
      }
    continue;
    }
  return;
}

void accelerate_motors(int time, int v_max) {//time : duration of the acceleration in millis (at least 3000 ms)  v_max : the final value of the pwm signal from 0 to 255
  digitalWrite(Relay_B, HIGH);
  
  //emptiing the buffer before reading the duration
  delay(10); 
  while(Serial.available() > 0) {
    Serial.read();
    delay(10);
  }

  
  int period = time/v_max;
  for (int i =0; i<v_max; i=i+1) {
    analogWrite(Mot_EN_DAv, i);
    analogWrite(Mot_EN_GAv, i);
    analogWrite(Mot_EN_DAr, i);
    analogWrite(Mot_EN_GAr, i);
    
    int previoustime=millis();        //waiting for period ms while beeing able to read the buffer looking for x; 
    while(millis()-previoustime < period) {
      if(Serial.available()>0) {
        String text2;
        text2=Serial.readStringUntil(';');
        if(text2 == "x") {
          kill_switch();
          return;
        }
        
      }
    continue;
    }

  }
  
  return;
}

void stop_motors(int time, int v_max) {//time : duration of the acceleration in millis (at least 3000 ms)  v_max : the final value of the pwm signal from 0 to 255
  int period = time/v_max;
  
  //emptiing the buffer before reading the duration
  delay(10); 
  while(Serial.available() > 0) {
    Serial.read();
    delay(10);
  }
  
  for (int i =v_max; i>0; i=i-1) {
    analogWrite(Mot_EN_DAv, i);
    analogWrite(Mot_EN_GAv, i);
    analogWrite(Mot_EN_DAr, i);
    analogWrite(Mot_EN_GAr, i);
    
    int previoustime=millis();        //waiting for period ms while beeing able to read the buffer looking for x; 
    while(millis()-previoustime < period) {
      if(Serial.available()>0) {
        String text;
        text=Serial.readStringUntil(';');
        if(text == "x") {
          kill_switch();
          return;
        }
      }
    continue;
    }
    
  }

  digitalWrite(Relay_B, LOW);
  return;
}

//function used to create the sequence of rolling the rover on a straigth line forward or backward dire = 1 forward dir = 0 backward
// use the previous functions
//
void rolling_straight(int dir) {
  
  int duration=0;
  int flag=0;
  String text2;
  
  //emptiing the buffer before reading the duration
  delay(10); 
  while(Serial.available() > 0) {
    Serial.read();
    delay(10);
  }
  
  delay(10);
  Serial.println();
  if( dir == 1) {
    Serial.println("--- forward ---");
  }
  else if (dir == 0){
    Serial.println("--- backward ---");
  }
  
  Serial.println("  enter duration (in ms)(acceleration and deceleration already take 6000ms) : ");
  
  while(flag==0) {
    if(Serial.available()>0) {
      text2=Serial.readStringUntil(';');
      duration=text2.toInt();
      flag=1;
    }
    continue;
  }
  Serial.print("  duration selected : ");
  Serial.print(duration);
  Serial.println(" ms");

  //emptiing the buffer after reading the duration
  delay(10); 
  while(Serial.available() > 0) {
    Serial.read();
    delay(10);
  }
  
  
  //set servo directions
  Serial.println("      *Set servo direction");
  set_servo_dir(90,90,90,90);
  Serial.println("        -->done");
  //set motors directions
  if( dir == 1) {
    set_motor_dir(1,1,1,1);
  }
  else if( dir == 0) {
    set_motor_dir(0,0,0,0);
  }
    

  
  //accelerating
  Serial.println("      *Accelerating motors");
  accelerate_motors(3000,255);
  Serial.println("        -->done");
  
  Serial.println("      *Rolling");
  //waiting for the duration and allowing a shutdowb
  int previoustime=millis();
  while(millis()-previoustime < duration) {
    if(Serial.available()>0) {
      text2=Serial.readStringUntil(';');
      if(text2 == "x") {
        kill_switch();
        return;
      }
    }
    continue;
  }
  Serial.println("        -->done");

  //decelearting motors
  Serial.println("      *Decelerating motors");
  stop_motors(3000,255);
  Serial.println("        -->done");
  digitalWrite(Relay_A, LOW);
  if( dir == 1) {
    Serial.println("--- end forward ---");
  }
  else if (dir == 0){
    Serial.println("--- end backward ---");
  }
  return;
}


void switch_relay(char relay_letter) {

  //clearing the serial buffer
  delay(10); 
  while(Serial.available() > 0) {
    Serial.read();
    delay(10);
  }
  delay(10);

  //asking the action
  Serial.println();
  if( relay_letter == 'A') {
    Serial.println("--- Relay_A ---");
  }
  else if (relay_letter == 'B'){
    Serial.println("--- Relay_B ---");
  }

  Serial.println("  switch the relay ON/OFF? : ");

  String text;
  int flag=0;
  while(flag==0) {
    if(Serial.available()>0) {
      text=Serial.readStringUntil(';');
      flag=1;
    }
    continue;
  }

  if(text == "ON") {
    if( relay_letter == 'A') {
      digitalWrite(Relay_A, HIGH);
      Serial.println("  --> Relay_A ON");
      Serial.println("--- End Relay_A ---");
    }
    else if( relay_letter == 'B') {
      digitalWrite(Relay_B, HIGH);
      Serial.println("  --> Relay_B ON");
      Serial.println("--- End Relay_B ---");
    }
  }
  else if(text == "OFF") {
    if( relay_letter == 'A') {
      digitalWrite(Relay_A, LOW);
      Serial.println("  --> Relay_A OFF");
      Serial.println("--- End Relay_A ---");
    }
    else if( relay_letter == 'B') {
      digitalWrite(Relay_B, LOW);
      Serial.println("  --> Relay_B OFF");
      Serial.println("--- End Relay_B ---");
    }
  }

}


// function used to make the robot turn on itself
//
//
void turn_roll(char dir) {
  int turning_speed=220;
  int duration=0;
  int flag=0;
  String text;
  
  //emptiing the buffer before reading the duration
  delay(10); 
  while(Serial.available() > 0) {
    Serial.read();
    delay(10);
  }
  
  delay(10);
  Serial.println();
  if( dir == 'L') {
    Serial.println("--- left turn ---");
  }
  else if (dir == 'R'){
    Serial.println("--- right turn ---");
  }
  
  Serial.println("  enter duration (in ms)(acceleration and deceleration already take 3000ms) : ");
  
  while(flag==0) {
    if(Serial.available()>0) {
      text=Serial.readStringUntil(';');
      duration=text.toInt();
      flag=1;
    }
    continue;
  }
  Serial.print("  duration selected : ");
  Serial.print(duration);
  Serial.println(" ms");

  //emptiing the buffer after reading the duration
  delay(10); 
  while(Serial.available() > 0) {
    Serial.read();
    delay(10);
  }

  //set servo directions
  Serial.println("      *Set servo direction");
  set_servo_dir(180,0,180,0);
  Serial.println("        -->done");
  //set motors directions
  if( dir == 'R') {
    set_motor_dir(0,0,1,1);
  }
  else if( dir == 'L') {
    set_motor_dir(1,1,0,0);
  }


    //accelerating
  Serial.println("      *Accelerating motors");
  accelerate_motors(1500,turning_speed);
  Serial.println("        -->done");
  
  Serial.println("      *Rolling");
  //waiting for the duration and allowing a shutdowb
  int previoustime=millis();
  while(millis()-previoustime < duration) {
    if(Serial.available()>0) {
      text=Serial.readStringUntil(';');
      if(text == "x") {
        kill_switch();
        return;
      }
    }
    continue;
  }
  Serial.println("        -->done");

  //decelearting motors
  Serial.println("      *Decelerating motors");
  stop_motors(1500,turning_speed);
  Serial.println("        -->done");
  digitalWrite(Relay_A, LOW);
  if( dir == 'R') {
    Serial.println("--- end right turn ---");
  }
  else if (dir == 'L'){
    Serial.println("--- end left turn ---");
  }
  return;
  
}

void setup() {
  pinMode(Relay_A, OUTPUT);
  pinMode(Relay_B, OUTPUT);
  pinMode(Mot_EN_DAv, OUTPUT);
  pinMode(Mot_INA_DAv, OUTPUT);
  pinMode(Mot_INB_DAv, OUTPUT);
  pinMode(Mot_EN_DAr, OUTPUT);
  pinMode(Mot_INA_DAr, OUTPUT);
  pinMode(Mot_INB_DAr, OUTPUT);
  pinMode(Mot_EN_GAv, OUTPUT);
  pinMode(Mot_INA_GAv, OUTPUT);
  pinMode(Mot_INB_GAv, OUTPUT);
  pinMode(Mot_EN_GAr, OUTPUT);
  pinMode(Mot_INA_GAr, OUTPUT);
  pinMode(Mot_INB_GAr, OUTPUT);
  //pinMode(LED_BUILTIN, OUTPUT);

  //digitalWrite(Relay_A, HIGH);
  //digitalWrite(Relay_B, HIGH);

  Servo_Droite_Avant.attach(Servo_DAv);
  Servo_Droite_Arriere.attach(Servo_DAr);
  Servo_Gauche_Avant.attach(Servo_GAv);
  Servo_Gauche_Arriere.attach(Servo_GAr);

  Serial.begin(9600);
  delay(10);
  Serial.setTimeout(100);

  Serial.println();
  Serial.println(" ~~~ROVER SR-01~~~");
  Serial.println();
  Serial.println("list of functions :");
  Serial.println("  - x;            to shutdown all motors");
  Serial.println("  - forward;      to go forward");
  Serial.println("  - backward;     to go backward");
  Serial.println("  - left_turn;    to rotate left");
  Serial.println("  - right_turn;   to rotate right");
  Serial.println("  - Relay_A;      to swith the state of the Relay A (servos)");
  Serial.println("  - Relay_B;     to swith the state of the Relay B (motors)");
}



void loop() {
  
  String text;
  if(Serial.available()>0) {
    
    text=Serial.readStringUntil(';');

    //Serial.println(text);
    if(text == "forward") {
      rolling_straight(1);
    }
    else if (text == "backward") {
      rolling_straight(0);
    }
    else if (text == "left_turn") {
      turn_roll('L');
    }
    else if (text == "right_turn") {
      turn_roll('R');
    }
    else if (text == "Relay_A") {
      switch_relay('A');
    }
    else if (text == "Relay_B") {
      switch_relay('B');
    }
    //emptiing the buffer before reading the duration
    delay(10); 
    while(Serial.available() > 0) {
      Serial.read();
      delay(10);
    }
  }
  /*
  set_servo_dir(90,90,90,90);
  delay(1000);*/
}
