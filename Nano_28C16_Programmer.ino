#include <digitalWriteFast.h>
//must use const int or #define to use digitalWriteFast() function on pins
#define RCLK 4
#define SER 2
#define SRCLK 3

#define OUT_EN_ACTIVE_LOW A0
#define WR_EN_ACTIVE_LOW 13

#define EEPROM_D0 5
#define EEPROM_D7 12

/* 
 *        INSERT DATA INTO ARRAY WITH ADDRESSES LOCATED RESPECTIVELY
 * Example: data for 4-bit hex decoder for common cathode 7-segment display (5161AS)
  */
byte data[] = { 0x7e, 0x30, 0x6d, 0x79, 0x33, 0x5b, 0x5f, 0x70, 0x7f, 0x7b, 0x77, 0x1f, 0x4e, 0x3d, 0x4f, 0x47 };




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
  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin++) {pinModeFast(pin, INPUT);} //set pins as input
  setAddress(address, true);
  byte data = B00000000;

  //read data
  for (int pin = EEPROM_D7; pin >= EEPROM_D0; pin--) {
    data = (data << 1) + digitalReadFast(pin);
  }
  return data;
}



//Write a byte to the EEPROM at the specified address.
void writeEEPROM(int address, byte data) {
  setAddress(address, false);
  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin++) {pinModeFast(pin, OUTPUT);}

  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin++) {
    digitalWriteFast(pin, data & 1); //ANDing data with 1 only ANDs the last digit
    data = data >> 1; //shifts to the next bit
  }
  //120ns WR_EN pulse (recommended between 100-1000ns)
  digitalWriteFast(WR_EN_ACTIVE_LOW, LOW);
  digitalWriteFast(WR_EN_ACTIVE_LOW, HIGH);
  
  //DATA POLLING to await end of write cycle
  bool EEPROM_D7_VALUE = readEEPROM(address) & 1;
  while (EEPROM_D7_VALUE != data & 1) { //wait until D7 equals data being written
      EEPROM_D7_VALUE = readEEPROM(address) & 1; 
  } 
}



//Prints data in first printAdresses amount of addresses onto serial monitor
void printContents() {
  //Creates a list (called "data") which contains 16 bytes of EEPROM's data
  /*
   * Change printAdresses to print data in that number of addresses. Number must be divisible by the number of bytes per line (default:8)
   */
  int printAdresses = 256;
  for (int rowOfBytes = 0; rowOfBytes < printAdresses; rowOfBytes += 8) {
    byte data[8]; //8 addresses each line 

    for (int BIT = 0; BIT <= 7; BIT++) {
      data[BIT] = readEEPROM(rowOfBytes + BIT);
    }

    //Prints formatted line of bytes
    char buf[80];
    sprintf(buf, "%03B:  %02B %02B %02B %02B  %02B %02B %02B %02B",
            rowOfBytes, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7]);
    Serial.println(buf);
  }
}



//erase entire 28C16
void clearEEPROM() {
  Serial.print("Erasing EEPROM");
  for (int address = 0; address <= 2047; address += 1) {
    writeEEPROM(address, B11111111);
    if (address % 64 == 0) {Serial.print(".");} //loading effect
  }
  Serial.println(" done");
}



void setup() {
  digitalWriteFast(OUT_EN_ACTIVE_LOW, HIGH);
  pinModeFast(RCLK, OUTPUT);
  pinModeFast(SRCLK, OUTPUT);
  pinModeFast(SER, OUTPUT);
  
  digitalWriteFast(WR_EN_ACTIVE_LOW, HIGH);
  pinModeFast(WR_EN_ACTIVE_LOW, OUTPUT);
  Serial.begin(57600);

  //clearEEPROM();

  // Write into EEPROM from data[] array 
  Serial.print("Programming EEPROM");
  for (int address = 0; address < sizeof(data); address++) {
    writeEEPROM(address, data[address]);
    digitalWriteFast(WR_EN_ACTIVE_LOW, HIGH);
    if (address % 30 == 0) {Serial.print(".");} //loading effect
  }
  Serial.println("done");
  
  // Read and print out the contents of the EEPROM
  Serial.println("Reading EEPROM...");
  printContents();
}

void loop() {}
