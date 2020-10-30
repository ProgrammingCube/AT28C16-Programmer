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
    print(sys.argv)
    print(sys.argv[1])
    ser = serial.Serial(sys.argv[1], 9600, timeout = 1)
    ser.flush()
    time.sleep(2)

    values = bytearray([4, 9, 62, 144, 56, 30, 147, 3, 210, 89, 111, 78, 184, 151, 17, 129])
    print("Writing values")
    ser.write(values)
    # check for erase/read
    if sys.argv[2] == 'e':
        ser.write(b"e")

    if sys.argv[2] == 'r':
        print("Reading")
        ser.write(b'r')
        #ser.flush()

    # open bin file
    #(b'\xff')
    if sys.argv[2] == 'p':
        try:
            f = open(sys.argv[3], "rb")
        except:
            print(sys.argv[3] + " does not exist.")
        ser.write(b'p')
        
        byte = f.read()
        print("Writing...")
        ser.write(byte)
        #for i in byte:
        #time.sleep(0.01)
        #ser.write(i)
        #print(str(i))
        #byte = f.read(1)
            
        f.close()
        print("Done!")

if __name__ == "__main__":
    main(sys.argv[1:])
