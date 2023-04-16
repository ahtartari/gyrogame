// MPU-6050 Short Example Sketch
#include<Wire.h>
//#include<movingAvg.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//movingAvg avgX(10);
//movingAvg avgY(10);
const int MPU = 0x68; // endereço I2C do MPU-6050
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

//int mediaX, mediaY;
//int angX, angY;
int pixX, pixY;

int rdX = 0, rdY = 0;
int cont = 0;

Adafruit_SSD1306 display = Adafruit_SSD1306();

void rstRd() {
  rdX = random(6, 22);
  rdY = random(20, 108);
}

void setup() {
  rstRd();
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);  // registro PWR_MGMT_1
  Wire.write(0);     // definido como zero (ativa o MPU-6050)
  Wire.endTransmission(true);
  //  Serial.begin(9600);
  //  avgX.begin();
  //  avgY.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.clearDisplay();
}

void loop() {
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);  // começando com o registro 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 14, true); // solicitar um total de 14 registros
  AcX = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcY = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ = Wire.read() << 8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp = Wire.read() << 8 | Wire.read(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX = Wire.read() << 8 | Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY = Wire.read() << 8 | Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ = Wire.read() << 8 | Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

  //  mediaX = avgX.reading(AcX);
  //  mediaY = avgY.reading(AcY);

  //  Serial.print("X = "); Serial.println(AcX);
  //  Serial.print("Y = "); Serial.println(AcY);
  //  Serial.println();

  //  angX = map(AcX, -32768, 32767, -180, 180);
  //  angY = map(AcY, -32768, 32767, -180, 180);
  //
  //  display.setCursor(0, 0);
  //  display.print("Angulo X: ");
  //  display.setCursor(60, 0);
  //  display.print(angX);
  //  display.setCursor(0, 10);
  //  display.print("Angulo Y: ");
  //  display.setCursor(60, 10);
  //  display.print(angY);
  //  display.display();
  //  display.clearDisplay();

  pixX = map(AcX, -18000, 18000, 32, 0);
  pixY = map(AcY, -18000, 18000, 128, 0);

  if (pixX == rdX && pixY == rdY) {
    cont++;
    rstRd();
  }


  //  Serial.println(pixX);
  display.drawPixel(pixY, pixX, 1);
  display.drawPixel(rdY, rdX, 1);

  display.setCursor(0, 0);
  display.print(cont);

  //  display.drawPixel(pixY+1, pixX+1, 1);
  //  display.drawPixel(pixY+1, pixX-1, 1);
  //  display.drawPixel(pixY-1, pixX+1, 1);
  //  display.drawPixel(pixY-1, pixX-1, 1);
  //
  //  display.drawPixel(pixY, pixX+1, 1);
  //  display.drawPixel(pixY, pixX-1, 1);
  //  display.drawPixel(pixY-1, pixX, 1);
  //  display.drawPixel(pixY+1, pixX, 1);

  //  display.setCursor(pixY, pixX);
  //  display.print("o");
  display.display();
  display.clearDisplay();

  //  Serial.print("Acelerômetro: ");
  //  Serial.print("X = "); Serial.print(AcX);
  //  Serial.print(" | Y = "); Serial.print(AcY);
  //  Serial.print(" | Z = "); Serial.println(AcZ);
  //  // equação para temperatura em graus C da folha de dados
  //  Serial.print("Temperatura: "); Serial.print(Tmp/340.00+36.53); Serial.println(" C ");
  //
  //  Serial.print("Giroscópio: ");
  //  Serial.print("X = "); Serial.print(GyX);
  //  Serial.print(" | Y = "); Serial.print(GyY);
  //  Serial.print(" | Z = "); Serial.println(GyZ);
  //  Serial.println(" ");
}
