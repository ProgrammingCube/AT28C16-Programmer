#define SHIFT_DATA 2
#define SHIFT_CLK 3
#define SHIFT_LATCH 4
#define EEPROM_D0 5
#define EEPROM_D7 12
#define WRITE_EN 13

/* data buffer size */
#define RECEIVE_CHUNK_SIZE  32

byte data_in_buffer[RECEIVE_CHUNK_SIZE];
byte data_out_buffer[RECEIVE_CHUNK_SIZE];
int global_address;
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
byte readEEPROM_Byte(int address) {
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
void writeEEPROM_Byte(int address, byte data) {
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
void printContents(int len) {
  for (int base = 0; base <= len; base += 16) {
    byte data[16];
    for (int offset = 0; offset <= 15; offset += 1) {
      data[offset] = readEEPROM_Byte(base + offset);
    }

    char buf[80];
    sprintf(buf, "%04x:  %02x %02x %02x %02x %02x %02x %02x %02x   %02x %02x %02x %02x %02x %02x %02x %02x",
            base, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7],
            data[8], data[9], data[10], data[11], data[12], data[13], data[14], data[15]);

    Serial.println(buf);
  }
}


/*
 * Erase entire EEPROM
 */
void eraseEEPROM(void)
{
  Serial.print("Erasing EEPROM");
  for (int address = 0; address <= 2047; address += 1) {
    writeEEPROM_Byte(address, 0xff);

    if (address % 64 == 0) {
      Serial.print(".");
    }
  }
  Serial.println(" done");
}


/*
 * Reads entire EEPROM
 */
void dumpEEPROM(void)
{
  //
}


/*
 * Writes 64 byte block of data to EEPROM
 */
void writeBlock(void)
{
  int i;
  /* wait for data */
  while (!Serial.available()) {}
  /* wait for data to fill */
  while (Serial.available() < RECEIVE_CHUNK_SIZE) {}
  /* copy data to buffer */
  for (i = 0; i < RECEIVE_CHUNK_SIZE; i++)
  {
    data_in_buffer[i] = Serial.read();
  }
  /* write buffer to EEPROM */
  for (i = 0; i < RECEIVE_CHUNK_SIZE; i++)
  {
    writeEEPROM_Byte(global_address + i, data_in_buffer[i]);
  }
  /* read EEPROM */
  for (i = 0; i < RECEIVE_CHUNK_SIZE; i++)
  {
    data_out_buffer[i] = readEEPROM_Byte(global_address + i);
  }
  /* compare & echo result */
  if (memcmp(data_in_buffer, data_out_buffer, sizeof(data_in_buffer)) != 0)
    Serial.write('N');
  else
    Serial.write('Y');

  /* if correct, increment global_address */
  global_address += RECEIVE_CHUNK_SIZE;
}


void setup() {
  // put your setup code here, to run once:
  pinMode(SHIFT_DATA, OUTPUT);
  pinMode(SHIFT_CLK, OUTPUT);
  pinMode(SHIFT_LATCH, OUTPUT);
  digitalWrite(WRITE_EN, HIGH);
  pinMode(WRITE_EN, OUTPUT);
  Serial.begin(9600);
  /* clear buffer first */
  {
    while(Serial.available() > 0)
      char t = Serial.read();
  }
  /* print message to show ready */
  Serial.print("READY\0");
  /* set global address */
  global_address = 0x00;
}

void loop() {
  /* Data is on serial line */
  if (Serial.available())
  {
    char inByte = Serial.read();
    switch (inByte)
    {
      case  'E':
        eraseEEPROM();
        break;
      case  'R':
        Serial.println("Reading EEPROM");
        printContents(255);
        Serial.write('Z');
        break;
      case  'W':
        writeBlock();
        break;
      case  'D':
        Serial.println("Dumping EEPROM");
        printContents(2048);
        Serial.write('Z');
        break;
      default:
        break;
    }
  }
}