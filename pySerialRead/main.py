from playsound import playsound
import serial, sys


if (len(sys.argv) <= 1):
    print("Informe a porta serial, ex: python main.py COM1")
    exit(0)


bauds = 9600
if (len(sys.argv) == 3):
    bauds = sys.argv[2]

porta = sys.argv[1]

while True:
   with serial.Serial(porta, bauds, timeout=1) as ser:
        line = ser.readline()
        if line.strip() == "1":
            playsound('explode.mp3')