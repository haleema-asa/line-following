#include <Wire.h>

#define enA 5                  //EnableA command line - should be a PWM pin
#define enB 6                  //EnableB command line - should be a PWM pin
#define I2C_SLAVE_ADDR 0x04    // 4 in hexadecimal
#define L1  25
#define L2  33
#define L3  32
#define R1  0
#define R2  2
#define R3  15









void setup() {
  // put your setup code here, to run once:
 // initialize serial communication at 9600 bits per second:
 
  Wire.begin();  // join i2c bus (address optional for the master) - on the Arduino NANO the default I2C pins are A4 (SDA), A5 (SCL)  

 Serial.begin(9600);
 // do i needto define the pins??
  pinMode (L1, INPUT);
  pinMode (L2, INPUT);
  pinMode (L3, INPUT);
  pinMode (R1, INPUT);
  pinMode (R2, INPUT);
  pinMode (R3, INPUT);


}




void loop() {


  // put your main code here, to run repeatedly:
  int L1Value = analogRead(L1)*(3.3/4095)*-1;//reading in the valus from the sensor a value between 0 - 4095 !check these piin numbers!
  int L2Value = analogRead(L2)*(3.3/4095)*-1;
  int L3Value = analogRead(L3)*(3.3/4095)*-1;
  int R1Value = analogRead(R1)*(3.3/4095)*-1;
  int R2Value = analogRead(R2)*(3.3/4095)*-1;
  int R3Value = analogRead(R3)*(3.3/4095)*-1;
  int L1Distance = -45.5;// distance from center of robot
  int L2Distance = -31;
  int L3Distance = -13;
  int R1Distance = 13;
  int R2Distance = 30.5;
  int R3Distance = 45.5;

  //It is important to note that this assumes the sensors give a high output when exposed to the black line.
  int weighted_average = (((L1Value*L1Distance)+(L2Value*L2Distance)+(L3Value*L3Distance)+(R1Value*R1Distance)+(R2Value*R2Distance)+(R3Value*R3Distance))/(L1Value+L2Value+L3Value+R1Value+R2Value+R3Value));
  // tells you where the line is in relation to the middle of the robot 
  float setpoint = 0;// check what to set this too 
  float error = weighted_average; 
  int Kp = 10;
  float u = Kp*error;
  


  // left motor sepeed = x right motor speed = y stearing angle = z 
  int z = 89 + u; // chage stearing angle 
  float k = 0.5; // to ensure the motor speed is only chnaged minorily in relation to u
  int x = 200;//𝑙𝑒𝑓𝑡𝑀𝑜𝑡𝑜𝑟𝑆𝑝𝑒𝑒𝑑 = 𝑏𝑎𝑠𝑒𝑆𝑝𝑒𝑒𝑑 + 𝐾u 
  int y = 200; //𝑟𝑖𝑔ℎ𝑡𝑀𝑜𝑡𝑜𝑟𝑆𝑝𝑒𝑒𝑑 = 𝑏𝑎𝑠𝑒𝑆𝑝𝑒𝑒𝑑 − 𝐾u
  
 Wire.beginTransmission(I2C_SLAVE_ADDR);  // transmit to device #4
  /* depending on the mirocontroller, the int variable is stored as 32-bits or 16-bits
     if you want to increase the value range, first use a suitable variable type and then modify the code below
     for example; if the variable used to store x and y is 32-bits and you want to use signed values between -2^31 and (2^31)-1
     uncomment the four lines below relating to bits 32-25 and 24-17 for x and y
     for my microcontroller, int is 32-bits hence x and y are AND operated with a 32 bit hexadecimal number - change this if needed

     >> X refers to a shift right operator by X bits
  */
  //Wire.write((byte)((x & 0xFF000000) >> 24)); // bits 32 to 25 of x
  //Wire.write((byte)((x & 0x00FF0000) >> 16)); // bits 24 to 17 of x
  Wire.write((byte)((x & 0x0000FF00) >> 8));  // first byte of x, containing bits 16 to 9
  Wire.write((byte)(x & 0x000000FF));         // second byte of x, containing the 8 LSB - bits 8 to 1
  //Wire.write((byte)((y & 0xFF000000) >> 24)); // bits 32 to 25 of y
  //Wire.write((byte)((y & 0x00FF0000) >> 16)); // bits 24 to 17 of y
  Wire.write((byte)((y & 0x0000FF00) >> 8));  // first byte of y, containing bits 16 to 9
  Wire.write((byte)(y & 0x000000FF));         // second byte of y, containing the 8 LSB - bits 8 to 1
  //Wire.write((byte)((z & 0xFF000000) >> 24)); // bits 32 to 25 of y
  //Wire.write((byte)((z & 0x00FF0000) >> 16)); // bits 24 to 17 of y
  Wire.write((byte)((z & 0x0000FF00) >> 8));  // first byte of y, containing bits 16 to 9
  Wire.write((byte)(z & 0x000000FF));         // second byte of y, containing the 8 LSB - bits 8 to 1

Wire.endTransmission();  // stop transmitting  




}
