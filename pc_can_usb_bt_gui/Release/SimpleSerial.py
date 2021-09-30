import serial
import sys
import time

ser = serial.Serial('COM48', baudrate=9600, timeout=1)

while True:
    time.sleep(2)
    data = ser.readline()
    print (data)