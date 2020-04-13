#include <digitalWriteFast.h>
//must use const int or #define to use digitalWriteFast
#define RCLK 4
#define SER 2
#define SRCLK 3
#define OUT_EN_ACTIVE_LOW A0
#define WR_EN_ACTIVE_LOW 13

#define EEPROM_D0 5
#define EEPROM_D7 12



void setAddress(int address, bool OUT_EN) {
  shiftOut(SER, SRCLK, MSBFIRST, address >> 8); //shift first 8 bits
  shiftOut(SER, SRCLK, MSBFIRST, address); //shift last 8 bits
  digitalWriteFast(RCLK, LOW);
  digitalWriteFast(RCLK, HIGH); //pulse RCLK
  digitalWriteFast(RCLK, LOW);

  digitalWriteFast(OUT_EN_ACTIVE_LOW, (OUT_EN) ? LOW : HIGH);
}



//returns the byte of data in the EEPROM
byte readEEPROM(int address) {
  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin++) {pinMode(pin, INPUT);} //set pins as input
  setAddress(address, true);
  byte data = B00000000;

  //read data
  for (int pin = EEPROM_D7; pin >= EEPROM_D0; pin--) {
    data = (data << 1) + digitalRead(pin);
  }
  return data;
}



//Write a byte to the EEPROM at the specified address.
void writeEEPROM(int address, byte data) {
  setAddress(address, false);
  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin++) {pinMode(pin, OUTPUT);}

  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin++) {
    digitalWriteFast(pin, data & 1); //ANDing with 1 has the result of outputting the last digit
    data = data >> 1; //shifts to the next bit
  }
  
  digitalWriteFast(WR_EN_ACTIVE_LOW, LOW);
  digitalWriteFast(WR_EN_ACTIVE_LOW, HIGH);
  delay(10);
}



//prints data in first 256 addresses onto serial monitor
void printContents() {
  //creates a list (data) which contains 16 bytes of EEPROM's data
  for (int BYTES = 0; BYTES < 256/*loops 16 times*/; BYTES += 16) {
    byte data[16];

    for (int BIT = 0; BIT <= 15; BIT++) {
      data[BIT] = readEEPROM(BYTES + BIT);
    }

    //prints a line of 16 bytes
    char buf[80];
    sprintf(buf, "%03B:  %02B %02B %02B %02B %02B %02B %02B %02B  %02B %02B %02B %02B %02B %02B %02B %02B",
            BYTES, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7],
            data[8], data[9], data[10], data[11], data[12], data[13], data[14], data[15]);
    Serial.println(buf);
  }
}



void clearEEPROM() {
  Serial.print("Erasing EEPROM");
  for (int address = 0; address <= 2047; address += 1) {
    writeEEPROM(address, 0xff);
    if (address % 64 == 0) {Serial.print(".");} //loading effect
  }
  Serial.println(" done");
}



/*
 * data for 4-bit hex decoder for common cathode 7-segment display (5161AS)
 */
byte data[] = { 0x7e, 0x30, 0x6d, 0x79, 0x33, 0x5b, 0x5f, 0x70, 0x7f, 0x7b, 0x77, 0x1f, 0x4e, 0x3d, 0x4f, 0x47 };



void setup() {
  pinMode(RCLK, OUTPUT);
  pinMode(SRCLK, OUTPUT);
  pinMode(SER, OUTPUT);
  
  digitalWriteFast(WR_EN_ACTIVE_LOW, HIGH);
  pinMode(WR_EN_ACTIVE_LOW, OUTPUT);
  Serial.begin(57600);

  clearEEPROM();

  // Program data bytes
  Serial.print("Programming EEPROM");
  for (int address = 0; address < sizeof(data); address += 1) {
    writeEEPROM(address, data[address]);
    digitalWriteFast(WR_EN_ACTIVE_LOW, HIGH);
    if (address % 64 == 0) {Serial.print(".");} //loading effect
  }
  Serial.println("done");
  
  delayMicroseconds(1); //wait to complete write cycle 
  // Read and print out the contents of the EEPROM
  Serial.println("Reading EEPROM...");
  printContents();
}



void loop() {}
