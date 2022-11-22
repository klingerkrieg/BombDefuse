from playsound import playsound
import serial, sys


if (len(sys.argv) <= 1):
    print("Informe a porta serial, ex: python main.py COM1")
    exit(0)


bauds = 9600
if (len(sys.argv) == 3):
    bauds = sys.argv[2]

porta = sys.argv[1]
print("Connecting:", porta, " ", bauds)
ser = serial.Serial(porta.strip(), bauds, timeout=1)
try:
    while True:
        line = ser.readline()
        text = line.decode()
        print(text)
        if text.strip() == "boom":
            print("******PLAY SOUND*******")
            playsound('explode.mp3')
            break
except:
    print("******POWER OFF*******")
    playsound('explode.mp3')