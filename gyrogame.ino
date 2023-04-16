/*
  GYROGAME

  Alexander Hugo Tartari
*/

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

const int MPU = 0x68; // endereço I2C do MPU-6050
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;
Adafruit_SSD1306 display = Adafruit_SSD1306();

int pixX, pixY;
int rdX = 0, rdY = 0;
int cont = 0;


void resetRandomPixel()
{
  rdX = random(6, 22);
  rdY = random(20, 108);
}

void setup()
{
  //  Serial.begin(9600);
  resetRandomPixel();
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B); // registro PWR_MGMT_1
  Wire.write(0);    // definido como zero (ativa o MPU-6050)
  Wire.endTransmission(true);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.clearDisplay();
}

void updateReadings()
{
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 14, true);      // 14 bytes with stop message
  AcX = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcY = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ = Wire.read() << 8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp = Wire.read() << 8 | Wire.read(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX = Wire.read() << 8 | Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY = Wire.read() << 8 | Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ = Wire.read() << 8 | Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
}

void updatePixels()
{
  pixX = map(AcX, -18000, 18000, 32, 0);
  pixY = map(AcY, -18000, 18000, 128, 0);
}

boolean verifyPoint()
{
  if (pixX == rdX && pixY == rdY)
  {
    cont++;
    resetRandomPixel();
    return true;
  }
  return false;
}

void drawPixels()
{
  display.drawPixel(pixY, pixX, 1);
  display.drawPixel(rdY, rdX, 1);

  //  uncomment for bigger player pixel
  //  display.drawPixel(pixY+1, pixX+1, 1);
  //  display.drawPixel(pixY+1, pixX-1, 1);
  //  display.drawPixel(pixY-1, pixX+1, 1);
  //  display.drawPixel(pixY-1, pixX-1, 1);
  //
  //  display.drawPixel(pixY, pixX+1, 1);
  //  display.drawPixel(pixY, pixX-1, 1);
  //  display.drawPixel(pixY-1, pixX, 1);
  //  display.drawPixel(pixY+1, pixX, 1);

  display.setCursor(0, 0);
  display.print(cont);
  display.display();
  display.clearDisplay();
}

void loop()
{
  updateReadings();
  updatePixels();
  verifyPoint();
  drawPixels();
}
