#include <Wire.h>
int Address = 0x69; // Device address in which is also included the 8th bit for selecting the mode, read in this case.


 uint8_t x, y,z;
 uint8_t ND[60];
void setup() {
  Wire.begin(); // Initiate the Wire library
  Serial.begin(9600);
  delay(100);

}

void loop() {

 //Start measurements
  Wire.beginTransmission(Address);
  Wire.write(0x00);
  Wire.write(0x10);
  Wire.write(0x03);
  Wire.write(0x00);
  uint8_t data[2]={0x03, 0x00};
  Wire.write(CalcCrc(data));
  Wire.endTransmission();
  
  delay(1000);

  //Stop meaurements
  SetPointer(0x01, 0x04);

  delay(100);

  //Read data ready flag
  SetPointer(0x02, 0x02);
  Wire.requestFrom(Address, (uint8_t) 3);
  x=Wire.read();
  y=Wire.read();
  z=Wire.read();

 

  if (y==0x01){//Measurements are ready to read
    SetPointer(0x03,0x00);
    Wire.requestFrom(Address, (uint8_t) 60);
    for(int i=0;i<60;i++) { ND[i]=Wire.read(); }
    for(int i=0;i<60;i++) { 
      if ((i+1)%3==0)
      {
      uint8_t    datax[2]={ND[i-2], ND[i-1]};
        Serial.print(CalcCrc(datax));
      Serial.print("  ");
      }
      Serial.println(ND[i]);}
    }

  Serial.println(" The end! ");

  delay(100);

}

void SetPointer(uint8_t PB1, uint8_t PB2)
{
    Wire.beginTransmission(Address);
      Wire.write(PB1);
  Wire.write(PB2);
  Wire.endTransmission();
  }




uint8_t CalcCrc(uint8_t data[2]) {
  uint8_t crc = 0xFF;
  for(int i = 0; i < 2; i++) {
    crc ^= data[i];
    for(uint8_t bit = 8; bit > 0; --bit) {
      if(crc & 0x80) {
      crc = (crc << 1) ^ 0x31u;
      } else {
        crc = (crc << 1);
       }
     }
   }
  return crc;
}
