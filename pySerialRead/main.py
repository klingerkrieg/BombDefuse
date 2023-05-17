from playsound import playsound
import serial, sys
import time
from datetime import datetime
import sendArduinoData

bauds = 9600
if (len(sys.argv) == 3):
    bauds = sys.argv[2]

#TENTA ENCONTRAR A PORTA SOZINHO
if (len(sys.argv) <= 1):
    result = []
    ports = ['COM%s' % (i + 1) for i in range(10)]
    found = False
    for port in ports:
        try:
            print(port)
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
        if (found):
            break
    if (found == False):
        print("Nenhuma bomba encontrada. Conecte a bomba ou indique a porta serial")
        print("Informe a porta serial, ex: python main.py COM1")
        exit(0)
else:
    #A PORTA TAMBEM PODE SER INFORMADA
    port = sys.argv[1]
    print("Connecting:", port, " ", bauds)
    ser = serial.Serial(port, bauds, timeout=1)


print("Digite o código da equipe no sistema de Rank")
team_id = input()

print("******PLAY ok*******")
playsound('audios/ok.mp3')

explodiu = False

def getTime():
    global start_time
    total = time.time() - start_time
    minutos = int(total / 60)
    segundos = int(total - (minutos*60))
    if minutos < 10:
        minutos = "0"+str(minutos)
    if segundos < 10:
        segundos = "0"+str(segundos)
    tempo = str(minutos)+":"+str(segundos)
    return tempo

fname = datetime.now().strftime("%Y-%m-%d %H.%M.%S")
f = open("queue/"+fname+".txt", "w")

f.write("team;"+team_id+"\n")

start_time = time.time()
f.write("event;time;type;\n")

try:
    while True:
        line = ser.readline()
        text = line.decode()
        #print(text)
        if text.strip() == "planted":
            start_time = time.time()
            f.write("started;"+getTime()+";\n")
            print("******PLAY planted*******")
            playsound('audios/planted.mp3')

        elif text.strip() == "boom":
            f.write("exploded;"+getTime()+";\n")
            print("******PLAY explode*******")
            playsound('audios/explode.mp3')
            explodiu = True
            break

        #se errou um fio ou um codigo
        elif text.strip().startswith("wrong"):
            dtype, number = text.strip().split(";")
            print (dtype, number)
            f.write(dtype+";"+getTime()+";"+number+"\n")

        #se desativou um fio ou um codigo
        elif text.strip().startswith("wire") or text.strip().startswith("code"):
            dtype, number = text.strip().split(";")
            print (dtype, number)
            f.write(dtype+";"+getTime()+";"+number+"\n")

        elif text.strip() == "defused":
            f.write("defused;"+getTime()+";\n")
            print("******PLAY defused*******")
            playsound('audios/defused.mp3')
            explodiu = False
            break
except:
    f.write("exploded;"+getTime()+";\n")
    explodiu = True
    print("******POWER OFF*******")
    playsound('explode.mp3')

f.close()

#mostra como ficou
f = open("queue/"+fname+".txt", "r")
print(f.read())
f.close()

#envia para o banco
sendArduinoData.send_data_to_server()