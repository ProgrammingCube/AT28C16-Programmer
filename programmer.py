'''
AT28C16 Bin Loader
October 22, 2020 - Patrick Jackson

Use this program to upload your .bin files
to the AT28C16 eeprom. Needs the complementary
Arduino Nano firmware to work.

Syntax:

programmer.py [COM] [e/r/p] [\..\file.bin]
'''

import serial, sys, getopt, time

def main(argv):
    # open serial port
    try:
        ser = serial.Serial(sys.argv[1], 9600)
    except:
        print("COM device not accessable.")
    ser.flush()

    # check for erase/read
    if sys.argv[2] == 'e':
        ser.write(b'e')
        while True:
            bytesToRead = ser.inWaiting()
            byte = ser.read(bytesToRead)
            print(byte)

    if sys.argv[2] == 'r':
        ser.write(b'r')
        while True:
            bytesToRead = ser.inWaiting()
            byte = ser.read(bytesToRead)
            print(byte)

    # open bin file
    #(b'\xff')
    if sys.argv[2] == 'p':
        try:
            f = open(sys.argv[3], "rb")
        except:
            print(sys.argv[3] + " does not exist.")
        ser.write(b'b')
        
        byte = f.read(1)
        print("Writing...")
        while byte:
            time.sleep(0.01)
            ser.write(byte)
            byte = f.read(1)
            
        f.close()
        print("Done!")

if __name__ == "__main__":
    main(sys.argv[1:])
