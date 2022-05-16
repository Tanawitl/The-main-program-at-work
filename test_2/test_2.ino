#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();

#include <Wire.h>
#include <VL53L0X.h>
VL53L0X sensor;

#include"TFT_A.h" //จอภาพ

long elapsedTime = 0;
long nowTime = 0;
const int MPU_addr = 0x68; // I2C address of the MPU-6050
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;
void setup() {
  Serial.begin(9600);
  tft.init();         //สั่งให้หน้าจอเริ่มทำงาน
  tft.setRotation(1); // หมุนหน้าจอ
  TFT_AA();//แสดงจอ
  
  Wire.begin();
  sensor.setTimeout(500);
  if (!sensor.init())
  {
    Serial.println("Failed to detect and initialize sensor!");
    while (1) {}
  }

  // Start continuous back-to-back mode (take readings as
  // fast as possible).  To use continuous timed mode
  // instead, provide a desired inter-measurement period in
  // ms (e.g. sensor.startContinuous(100)).
  sensor.startContinuous();


  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);

  
}
/************************************setup**************************************************/
void loop() 
{tft.fillRect(0, 85, 480, 95, tft.color24to16(0xFFFFFF));//ปิดข้อความกลาง

 elapsedTime = millis();  //ใช้หน่วงเวลา
  //เริ่ม
  
  
  
  if (elapsedTime - nowTime > 1000) //ใช้หน่วงเวลา
  {//เริ่ม

  Serial.print(sensor.readRangeContinuousMillimeters());
  if (sensor.timeoutOccurred()) { Serial.print(" TIMEOUT"); }
  Serial.println();
  tft.print(sensor.readRangeContinuousMillimeters());
  if (sensor.readRangeContinuousMillimeters() == 100){
    //while(2){
    tft.setTextSize(3);
    tft.setTextColor(tft.color24to16(0xFF4500));
    tft.setTextSize(5);//ขนาดข้อความ
    tft.drawString("Be Careful", 100, 100);
    delay(300);
    //}

}
  //จบ
  int a = millis() / 1000;  
  tft.fillRect(0, 0, 120, 25, tft.color24to16(0xFFFFFF));  
  tft.setTextColor(tft.color24to16(0xFF0066));//สีข้อความ
  tft.setTextSize(1);//ขนาดข้อความ
  tft.drawString("Time: ", 10, 10);//ข้อความ
  tft.setCursor(40, 10);
  tft.print(a);
  a++; 

//แสดงระยะ
  tft.fillRect(200, 50, 80, 20, tft.color24to16(0x6699FF));
  tft.setTextColor(tft.color24to16(0xFF0066));//สีข้อความ
  tft.setTextSize(2.5);//ขนาดข้อความ
  tft.setCursor(200,50);
  tft.print(sensor.readRangeContinuousMillimeters());

//แสดงแกน
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 14, true); // request a total of 14 registers
  AcX = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcY = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ = Wire.read() << 8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp = Wire.read() << 8 | Wire.read(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX = Wire.read() << 8 | Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY = Wire.read() << 8 | Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ = Wire.read() << 8 | Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  Serial.print("ACC X :");
  Serial.print(AcX);
  Serial.print("  ACC Y :");
  Serial.print(AcY);
  Serial.print("  ACC Z :");
  Serial.print(AcZ);
  Serial.println();
//แสดงแกนบนจอ    
    tft.fillRect(0, 20, 130, 45, tft.color24to16(0xFFFFFF));  
  tft.setTextColor(tft.color24to16(0xFF0066));//สีข้อความ
  tft.setTextSize(1);//ขนาดข้อความ
  tft.drawString("ACC X : ", 10, 20);//ข้อความ
  tft.setCursor(45, 20);
  tft.print(AcX);
    
  tft.setTextColor(tft.color24to16(0xFF0066));//สีข้อความ
  tft.setTextSize(1);//ขนาดข้อความ
  tft.drawString("ACC Y : ", 10, 30);//ข้อความ
  tft.setCursor(45, 30);
  tft.print(AcY);
  
  tft.setTextColor(tft.color24to16(0xFF0066));//สีข้อความ
  tft.setTextSize(1);//ขนาดข้อความ
  tft.drawString("ACC Z : ", 10, 40);//ข้อความ
  tft.setCursor(45, 40);
  tft.print(AcZ);
  
  nowTime = elapsedTime;//ใช้หน่วงเวลา
  }
} 
