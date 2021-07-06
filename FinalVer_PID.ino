// MOTORS
int motor_l_Pin = 5; // D5: motorL
int motor_r_Pin = 3; // D3: motorR
/**** LEDs
 *  Right turn: Red
 *  Left turn: Blue
 *  Straight: Green
 *  Stop at the end: Red & Blue
 */
int LED_R_Pin = 11;
int LED_G_Pin = 10;
int LED_B_Pin = 9;
// IR Input
int IR_c_Pin = 1;
int IR_r_Pin = 2;
int IR_l_Pin = 3;

double rightSpeed = 100, leftSpeed = 100;

void setup() {
  // put your setup code here, to run once:
  //Serial.begin(9600);
  pinMode(motor_l_Pin, OUTPUT);
  pinMode(motor_r_Pin, OUTPUT);
  pinMode(LED_B_Pin, OUTPUT);
  pinMode(LED_G_Pin, OUTPUT);
  pinMode(LED_R_Pin, OUTPUT);
  pinMode(IR_c_Pin, INPUT);
  pinMode(IR_l_Pin, INPUT);
  pinMode(IR_r_Pin, INPUT);
}

void flash(int pin){
    analogWrite(pin, 5);
}

void unflash(int pin){
    analogWrite(pin, 0);
}

double pControllerIR(double error){
  double KpIR = .12;
  //error should be defined as the difference vs how far the difference of ideal is.
  double correction = (KpIR * error);
  //Serial.print("Pcontroller value is ");
  //Serial.print(correction);
  
  return correction;
}

int val_l = 0;
int val_r = 0;
int val_c = 0;
double errorIR = 0;

void loop() {
  val_r = analogRead(IR_r_Pin);
  val_l = analogRead(IR_l_Pin);
  val_c = analogRead(IR_c_Pin);
 /*
  Serial.print(", Right Sensor: ");
  Serial.print(val_r);
  Serial.print(", Left Sensor: ");
  Serial.print(val_l);
  Serial.print(", Center Sensor: ");
  Serial.println(val_c);
*/
  val_l -= 146;
 
  errorIR = ((val_r) - (val_l));
  
  //Serial.print(", errorIR: ");
  //Serial.print(errorIR);
  
  int val = pControllerIR(errorIR);
  
  //Serial.print(", val: ");
  //Serial.println(val);
  
  if ((leftSpeed + val) < 60){
    // turn left
    leftSpeed = 80;
    rightSpeed = 115;
   }
  else if ((rightSpeed - val) < 60){
    // turn right
    rightSpeed = 80;
    leftSpeed = 115;
  }
  else{
    rightSpeed = rightSpeed + val;
    leftSpeed = leftSpeed - val;
  }
  
  if((val_r < 200) && (val_l < 200)){
    // Stop
    leftSpeed = 0;
    rightSpeed = 0;
    flash(LED_R_Pin);
    flash(LED_B_Pin);
    unflash(LED_G_Pin);
  }
  else if ((1.2 * leftSpeed) < rightSpeed){
    // left
    flash(LED_B_Pin);
    unflash(LED_R_Pin);
    unflash(LED_G_Pin);
  }
  else if ((1.2 * leftSpeed) > rightSpeed){
    // right
    flash(LED_R_Pin);
    unflash(LED_G_Pin);
    unflash(LED_B_Pin);
  }
  /*
  else if (((70 < leftSpeed || leftSpeed < 100) || (85 < rightSpeed || rightSpeed < 110)) || ((70 < rightSpeed || rightSpeed < 100) && (85 < leftSpeed || leftSpeed < 110))){
    // straight
    flash(LED_G_Pin);
    unflash(LED_R_Pin);
    unflash(LED_B_Pin);
  }
  /*
  // TH_L
  // R = 200, L = 200, C = 600
  // TH_H
  // R = 550, L = 700, C = N/A(not accurate)
  if((val_r < 200) && (val_l < 200) && (val_c < 600)){
    // Stop
    leftSpeed = 0;
    rightSpeed = 0;
    flash(LED_R_Pin);
    flash(LED_B_Pin);
    unflash(LED_G_Pin);
  }
  else if ((val_r > 550) && (val_l < 230)){
    // left
    flash(LED_B_Pin);
    unflash(LED_R_Pin);
    unflash(LED_G_Pin);
  }
  else if ((val_r < 230) && (val_l > 700)){
    // right
    flash(LED_R_Pin);
    unflash(LED_G_Pin);
    unflash(LED_B_Pin);
  }
  else if ((val_r > 550) && (val_l > 700)){
    // straight
    flash(LED_G_Pin);
    unflash(LED_R_Pin);
    unflash(LED_B_Pin);
  }
  */
/*
  Serial.print(", Right Speed: ");
  Serial.print(rightSpeed);
  Serial.print(", Left Speed: ");
  Serial.println(leftSpeed);
  */
  analogWrite(motor_l_Pin, (1.2*leftSpeed)); // with 6V
  analogWrite(motor_r_Pin, rightSpeed);
}

