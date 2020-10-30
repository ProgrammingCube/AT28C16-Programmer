import serial, time, sys

#with serial.Serial('COM11', 9600) as ser:
ser = serial.Serial('COM11', 9600)
while True:
    ser.write(b'a')
