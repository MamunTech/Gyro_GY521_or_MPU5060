///////////////////////////////////////////////////////////////////////////////////////
/*Terms of use
Gyro - Arduino pro mini
VCC  -  5V
GND  -  GND
SDA  -  A4
SCL  -  A5

LCD  - Arduino pro mini
VCC  -  5V
GND  -  GND
SDA  -  A4
SCL  -  A5
*//////////////////////////////////////////////////////////////////////////////////////

//Include LCD and I2C library

#include <Wire.h>

//Declaring some global variables
int gyro_x, gyro_y, gyro_z;
int temperature;
long gyro_x_cal, gyro_y_cal, gyro_z_cal;
long loop_timer;
int lcd_loop_counter;
float angle_pitch, angle_roll,angle_yaw;
boolean set_gyro_angles;

void setup() {
  Wire.begin();                                                        //Start I2C as master
  Serial.begin(9600);                                               //Use only for debugging
  
  setup_mpu_6050_registers();                                          //Setup the registers of the MPU-6050 (500dfs / +/-8g) and start the gyro

                                                  

  Serial.println("  MPU-6050 IMU");                                         //Print text to screen

  delay(1500);                                                         //Delay 1.5 second to display the text
                                                        
  Serial.println("Calibrating gyro");                                       //Print text to screen

  for (int cal_int = 0; cal_int < 2000 ; cal_int ++){                  //Run this code 2000 times
    if(cal_int % 125 == 0)
    Serial.print(".");                              //Print a dot on the LCD every 125 readings
    read_mpu_6050_data();                                              //Read the raw acc and gyro data from the MPU-6050
    gyro_x_cal += gyro_x;                                              //Add the gyro x-axis offset to the gyro_x_cal variable
    gyro_y_cal += gyro_y;                                              //Add the gyro y-axis offset to the gyro_y_cal variable
    gyro_z_cal += gyro_z;                                              //Add the gyro z-axis offset to the gyro_z_cal variable
    delay(3);                                                          //Delay 3us to simulate the 250Hz program loop
  }
  gyro_x_cal /= 2000;                                                  //Divide the gyro_x_cal variable by 2000 to get the avarage offset
  gyro_y_cal /= 2000;                                                  //Divide the gyro_y_cal variable by 2000 to get the avarage offset
  gyro_z_cal /= 2000;                                                  //Divide the gyro_z_cal variable by 2000 to get the avarage offset
}

void loop(){

  read_mpu_6050_data();                                                //Read the raw acc and gyro data from the MPU-6050

  gyro_x -= gyro_x_cal;                                                //Subtract the offset calibration value from the raw gyro_x value
  gyro_y -= gyro_y_cal;                                                //Subtract the offset calibration value from the raw gyro_y value
  gyro_z -= gyro_z_cal;                                                //Subtract the offset calibration value from the raw gyro_z value

/*
  Serial.print("angle_pitch_output    ");
  Serial.println(abs(gyro_y));
   Serial.print("angle_roll_output   ");
  Serial.println(abs(gyro_x));

  */
  //Gyro angle calculations
  //0.01526717557 = 1 / ( 65.5)
  angle_pitch = gyro_y * 0.01526717557  ;                                   //Calculate the traveled pitch angle and add this to the angle_pitch variable
  angle_roll = gyro_x * 0.01526717557;                                    //Calculate the traveled roll angle and add this to the angle_roll variable
  angle_yaw   = gyro_z*0.01526717557;
  /*
  Serial.print("angle_pitch_output    ");
  Serial.println(abs(angle_pitch));
   Serial.print("angle_roll_output   ");
  Serial.println(abs(angle_roll));
  Serial.print("angle_Yaw_output   ");
  Serial.println(abs(angle_yaw));
*/
  Serial.print(abs(angle_pitch));
   Serial.print(" ");
  Serial.print(abs(angle_roll));
  Serial.print(" ");
  Serial.println(abs(angle_yaw));
  

}


void read_mpu_6050_data(){                                             //Subroutine for reading the raw gyro and accelerometer data
  Wire.beginTransmission(0x68);                                        //Start communicating with the MPU-6050
  Wire.write(0x41);                                                    //Send the requested starting register
  Wire.endTransmission();                                              //End the transmission
  Wire.requestFrom(0x68,8);                                           //Request 14 bytes from the MPU-6050
  while(Wire.available() < 8);                                        //Wait until all the bytes are received
  temperature = Wire.read()<<8|Wire.read();                            //Add the low and high byte to the temperature variable
  gyro_x = Wire.read()<<8|Wire.read();                                 //Add the low and high byte to the gyro_x variable
  gyro_y = Wire.read()<<8|Wire.read();                                 //Add the low and high byte to the gyro_y variable
  gyro_z = Wire.read()<<8|Wire.read();                                 //Add the low and high byte to the gyro_z variable
}


void setup_mpu_6050_registers(){
  //Activate the MPU-6050
  Wire.beginTransmission(0x68);                                        //Start communicating with the MPU-6050
  Wire.write(0x6B);                                                    //Send the requested starting register
  Wire.write(0x00);                                                    //Set the requested starting register
  Wire.endTransmission();                                              //End the transmission
  //Configure the gyro (500dps full scale)
  Wire.beginTransmission(0x68);                                        //Start communicating with the MPU-6050
  Wire.write(0x1B);                                                    //Send the requested starting register
  Wire.write(0x08);                                                    //Set the requested starting register
  Wire.endTransmission();                                              //End the transmission
}

