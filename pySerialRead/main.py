from playsound import playsound
import serial, sys
import time



bauds = 9600
if (len(sys.argv) == 3):
    bauds = sys.argv[2]

#TENTA ENCONTRAR A PORTA SOZINHO
if (len(sys.argv) <= 1):
    result = []
    ports = ['COM%s' % (i + 1) for i in range(256)]
    found = False
    for port in ports:
        try:
            ser = serial.Serial(port, bauds, timeout=1)
            for i in range(5):
                line = ser.readline()
                text = line.decode()
                if (text.strip() == "bomb"):
                    print("Bomba encontrada")
                    print("Connecting:", port, " ", bauds)
                    found = True
                    break
        except (OSError, serial.SerialException):
            pass
    
    if (found == False):
        print("Nenhuma bomba encontrada. Conecte a bomba ou indique a porta serial")
        print("Informe a porta serial, ex: python main.py COM1")
        exit(0)
else:
    #A PORTA TAMBEM PODE SER INFORMADA
    port = sys.argv[1]
    print("Connecting:", port, " ", bauds)
    ser = serial.Serial(port, bauds, timeout=1)

print("******PLAY ok*******")
playsound('audios/ok.mp3')

explodiu = False

try:
    while True:
        line = ser.readline()
        text = line.decode()
        #print(text)
        if text.strip() == "planted":
            start_time = time.time()
            print("******PLAY planted*******")
            playsound('audios/planted.mp3')
        elif text.strip() == "boom":
            end_time = time.time()
            print("******PLAY explode*******")
            playsound('audios/explode.mp3')
            explodiu = True
            break
        elif text.strip() == "defused":
            end_time = time.time()
            print("******PLAY defused*******")
            playsound('audios/defused.mp3')
            explodiu = False
            break
except:
    end_time = time.time()
    print("******POWER OFF*******")
    playsound('explode.mp3')

total = end_time - start_time

minutos = int(total / 60)
segundos = int(total - (minutos*60))

if minutos < 10:
    minutos = "0"+str(minutos)
if segundos < 10:
    segundos = "0"+str(segundos)

tempo = str(minutos)+":"+str(segundos)

if (explodiu):
    escrever = tempo + " explodiu"
else:
    escrever = tempo + " desarmada"

print(escrever)
with open("tempos.txt", "a") as f:
   f.write(escrever+"\r\n")

time.sleep(3)