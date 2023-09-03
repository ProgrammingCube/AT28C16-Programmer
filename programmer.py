import os,sys,time
import serial

def loop(rom_f, ser):
    chunkSize = 32
    blocks = 2048/chunkSize
    print("# of blocks: " + str(blocks))
    blockCounter = blocks
    while blockCounter > 0:
        ser.write(b'W')
        time.sleep(0.5)
        blockCounter -= 1
        dataBytes = rom_f.read(chunkSize)
        line = bytearray()
        line.extend(dataBytes)
        ser.write(line)
        time.sleep(0.5)

def main():
    ser = serial.Serial(sys.argv[1], 9600)
    print("Waiting for serial...")
    time.sleep(5)
    print("Ready to run")
    if (sys.argv[2] == "p"):
        romFileName = sys.argv[3]
        if not(os.path.isfile(romFileName)):
            print("Filename not valid/found")
            exit(-1)
        rom_f = open(romFileName, "rb")
        loop(rom_f, ser)
    elif (sys.argv[2] == "e"):
        ser.write(b'E')
        print("Erasing - please wait")
        time.sleep(20)
        print("Erased")
    elif (sys.argv[2] == "r"):
        print("Reading rom")
        ser.write(b'R')
        while True:
            print(str(ser.readline()))
    ser.close()


if __name__ == "__main__":
    main()
