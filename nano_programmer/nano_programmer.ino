#define SHIFT_DATA 2
#define SHIFT_CLK 3
#define SHIFT_LATCH 4
#define EEPROM_D0 5
#define EEPROM_D7 12
#define WRITE_EN 13

//byte data[] = { 0x18, 0xd8, 0xad, 0x00, 0x02, 0x6d, 0x01, 0x02, 0x8d, 0x02, 0x02, 0x4c, 0x00, 0x80 };

byte data[16];

int checksum_check;

int _address = 0;

/*
 * Output the address bits and outputEnable signal using shift registers.
 */
void setAddress(int address, bool outputEnable) {
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, (address >> 8) | (outputEnable ? 0x00 : 0x80));
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, address);

  digitalWrite(SHIFT_LATCH, LOW);
  digitalWrite(SHIFT_LATCH, HIGH);
  digitalWrite(SHIFT_LATCH, LOW);
}


/*
 * Read a byte from the EEPROM at the specified address.
 */
byte readEEPROM(int address) {
  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin += 1) {
    pinMode(pin, INPUT);
  }
  setAddress(address, /*outputEnable*/ true);

  byte data = 0;
  for (int pin = EEPROM_D7; pin >= EEPROM_D0; pin -= 1) {
    data = (data << 1) + digitalRead(pin);
  }
  return data;
}


/*
 * Write a byte to the EEPROM at the specified address.
 */
void writeEEPROM(int address, byte data) {
  setAddress(address, /*outputEnable*/ false);
  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin += 1) {
    pinMode(pin, OUTPUT);
  }

  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin += 1) {
    digitalWrite(pin, data & 1);
    data = data >> 1;
  }
  digitalWrite(WRITE_EN, LOW);
  delayMicroseconds(1);
  digitalWrite(WRITE_EN, HIGH);
  delay(10);
}


/*
 * Read the contents of the EEPROM and print them to the serial monitor.
 */
void printContents() {
  for (int base = 0; base <= 255; base += 16) {
    byte data[16];
    for (int offset = 0; offset <= 15; offset += 1) {
      data[offset] = readEEPROM(base + offset);
    }

    char buf[80];
    sprintf(buf, "%03x:  %02x %02x %02x %02x %02x %02x %02x %02x   %02x %02x %02x %02x %02x %02x %02x %02x",
            base, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7],
            data[8], data[9], data[10], data[11], data[12], data[13], data[14], data[15]);

    Serial.println(buf);
  }
}



void setup() {
  pinMode(SHIFT_DATA, OUTPUT);
  pinMode(SHIFT_CLK, OUTPUT);
  pinMode(SHIFT_LATCH, OUTPUT);
  digitalWrite(WRITE_EN, HIGH);
  pinMode(WRITE_EN, OUTPUT);
  Serial.begin(9600);
  //clear buffer?
}

void loop() {
  //check for programming flag
  while (Serial.available() > 0)
  {
    char inChar = Serial.read();
    //Serial.println(inChar);
    if (inChar == 'e')
    {
      
      //ERASE
      Serial.print("Erasing EEPROM");
      for (int address = 0; address <= 2047; address += 1) {
        writeEEPROM(address, 0xff);
    
        if (address % 64 == 0) {
          Serial.print(".");
        }
      }
      Serial.println(" done");
    }

    //READ
    else if (inChar == 'r')
    {
      //read
      Serial.println("Reading EEPROM");
      printContents();
    }

    //PROGRAM
    else if (inChar == 'p')
    {
      // no crc yet, let's see how this holds up...
      for (uint8_t i = 0; i < 16; i++)
        data[i] = Serial.read();
      for (int address = 0; address < 16; address += 1)
      {
        writeEEPROM(_address + address, data[_address + address]);
      }
      _address += 16;
    }
  }
}
