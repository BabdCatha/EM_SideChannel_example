import serial
from time import sleep
from secrets import token_bytes

#Serial link parameters
port = "COM4"
baudrate = 9600

#Options about the vectors to encrypt
number_of_vectors = 100

#Initializing the serial link with the STM32
ser = serial.Serial(port=port, baudrate=baudrate, bytesize=serial.EIGHTBITS, timeout=10, stopbits=serial.STOPBITS_ONE, parity=serial.PARITY_NONE)

#Setting up the file logging the encrypted vectors
file = open("vectors.txt", "w")

#Generating the test vectors, encrypting them and storing them
for i in range(number_of_vectors):
    data = bytearray(token_bytes(16))
    ser.write(data)

    out = ser.read(16)
    out_str = ""

    for byte in out:
        out_str += "{0:02x}".format(byte)
    print(out_str)
    file.write(out_str + "\n")

    sleep(1)

file.close()

ser.close()