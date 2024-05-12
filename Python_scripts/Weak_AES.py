import serial
from time import sleep

ser = serial.Serial(port='COM4', baudrate=9600, bytesize=serial.EIGHTBITS, timeout=10, stopbits=serial.STOPBITS_ONE, parity=serial.PARITY_NONE)
data = bytearray([0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d, 0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34])
ser.write(data)

out = ser.read(16)

for byte in out:
    print("0x{0:02x}".format(byte), end=" ")

ser.close()