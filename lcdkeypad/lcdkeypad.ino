#include <LiquidCrystal.h>
#include <Keypad.h>



//Altere a sequencia para mudar a ordem de retirada dos fios
//Os valores são as portas
int  sequenciaFios[] = {52, 51, 50, 49};
const int qtdFios = 4;
//Penalidade fio errado
int penalidadeMin = 3;

//Eses vetor guardará a sequencia que foi realizada
//Ele irá controlar quando o fio for desconectado
//Uma vez desconectado ele não será mais contabilizado se for conectado novamente
bool seqRealizada[qtdFios]  = {};
//Esse vetor irá controlar se errou ou acertou ou ainda nao realizou a sequencia
//-1 errou, 0 nao fez, 1 acertou
int seqPontuada[qtdFios]  = {};


//momento em que a bomba iniciou a contagem
int startMillis = 0;




/////////////////////////KEYPAD
const byte ROWS = 4; // Define the number of rows on the keypad
const byte COLS = 3; // Define the number of columns on the keypad
char keys[ROWS][COLS] = { // Matrix defining character to return for each key
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {24, 34, 32, 28}; //connect to the row pins (R0-R3) of the keypad
byte colPins[COLS] = {26, 22, 30}; //connect to the column pins (C0-C2) of the keypad
//initialize an instance of class
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
////////////////////////////////



 
// Define as conexões e cria o objeto para acesso
const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
const int backLight = 10;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int buzzerPIN = 14;
 
// Limites para detecção das teclas, em ordem crescente
struct {
  int limite;
  char *nome;
} teclas[] = {
  {   50, "Direita " },
  {  150, "Cima    " },
  {  310, "Baixo   " },
  {  500, "Esquerda" },
  {  750, "Select  " },
  { 1024, "        " }  // nenhuma tecla apertada
};
 
// Iniciação
void setup() {

  Serial.begin(9600);

  //Inicia os vetores
  //Seta as portas como entrada
  for (int i = 0; i < qtdFios; i++){
    seqRealizada[i] = false;
    seqPontuada[i]  = 0;
    pinMode(sequenciaFios[i],INPUT);  
  }


  //Progama o pino de backlight como saída
  pinMode(backLight, OUTPUT);
  pinMode(buzzerPIN,OUTPUT);
  
  // Inicia o display e coloca uma mensagem
  lcd.begin(16, 2);
  lcd.print("--:--");
  // Acende o backlight
  digitalWrite(backLight, HIGH);
}


bool ativada = false;
int min_ = 9;
int seg_ = 0;

/**
 * Imprime e gerencia no visor 1 traço para cada fio ainda conectado
 * Também faz o gerenciamento da sequência de remoção dos fios
 */
void gerenciaFiosConectados(){
    int proxFio = -1;
    //Descobre qual o proximo fio que tem que ser desativado
    for (int i = 0; i < qtdFios; i++){
        if (seqRealizada[i] == false){
            proxFio = i;
            break;
        }
    }
    
    //Imprime os fios conectados
    int posicaoLCD = 6;
    for (int i = 0; i < qtdFios; i++){
        //Verifica o fio que foi desconectado
        if (seqRealizada[i] == false && !digitalRead(sequenciaFios[i])){
            seqRealizada[i] = true;
            //Detecta qual fio foi removido
            if (proxFio != i){
                //Se tiver sido o fio errado
                min_ -= penalidadeMin;
                //apita 3 vezes para indicar que errou o fio
                /*beep(100,50);
                beep(100,50);
                beep(100,50);*/
                seqPontuada[i] = -1;
            } else {
                //Se foi o fio certo
                seqPontuada[i] = 1;
            }
        }

        lcd.setCursor(posicaoLCD+i,1);
        if (seqPontuada[i] == -1){
            lcd.print("*");
        } else
        if (seqPontuada[i] == 0){
            lcd.print("|");
        } else
        if (seqPontuada[i] == 1){
            lcd.print(" ");
        }
        
    }  
}



void gerenciaTeclado4x3(){
  char key = keypad.getKey();
  if (key){     // If key is pressed, send it to the Serial Monitor Window
    Serial.println(key);
  }  
}



/*

//beep
int beepStopMillis = 0;
bool ligaBeep = true;
int tempoBeep = 50;
//timer
int lastMilliSecond = 0;

//lastMilliSecond     1500
//millis_             2000
//actualSecond        2
//beepStopMillis      1050
//lastSecond          1

void beep(){
  int millis_ = millis();           
  int actualSecond = millis_/1000;
  int lastSecond = lastMilliSecond/1000;
  
  //A cada segundo entra nesse if
  if (lastSecond != actualSecond){
      tone(buzzerPIN, 100);
      beepStopMillis = millis_ + 50;
  } else
  if (beepStopMillis <= millis_){
      noTone(buzzerPIN);  
  }
}


void gerenciarTimer(){

    int millis_ = millis();           
    int actualSecond = millis_/1000;
    int lastSecond = lastMilliSecond/1000;
    
    //A cada segundo entra nesse if
    if (lastSecond != actualSecond){
        lastMilliSecond = millis_;

        //Quando seg chegar em -1 volta para 59
        seg_--;
        if( seg_ == -1){
            seg_ = 59;
            min_--;
        }
    
        // Quando _min for menor que 0 a bomba explode
        if( min_ < 0){
            lcd.setCursor(0,1);
            lcd.print("xxx");
            ativada = false;
        }  
    }
} */


//beep
int beepStopMillis = 0;
bool ligaBeep = true;
int tempoBeep = 50;
//timer
int lastMilliSecond = 0;

//lastMilliSecond     1500
//millis_             2000
//actualSecond        2
//beepStopMillis      1050
//lastSecond          1

//em bez de dividir por 1000, somar mais 1000

void beep(){
  int millis_ = millis();           
  int actualSecond = millis_/1000;
  int lastSecond = lastMilliSecond/1000;
  
  //A cada segundo entra nesse if
  if (lastSecond != actualSecond){
      lastMilliSecond = millis_;

      //beep
      tone(buzzerPIN, 100);
      beepStopMillis = millis_ + 50;
      

      //Quando seg chegar em -1 volta para 59
      seg_--;
      if( seg_ == -1){
          seg_ = 59;
          min_--;
      }
  
      // Quando _min for menor que 0 a bomba explode
      if( min_ < 0){
          lcd.setCursor(0,1);
          lcd.print("xxx");
          ativada = false;
      }  
  } else
  if (beepStopMillis <= millis_){
      noTone(buzzerPIN);  
  }
}


 
// Laço principal
void loop() {

    
  int beepOn = 0;
  int beepOff = 0;

  //Quando ativada começará a contar
  if (ativada){

      //registro o tempo onde a bomba realmente começou a contar
      if (startMillis == 0){
          startMillis = millis();  
      }
  
      gerenciaFiosConectados();
      //gerenciaTeclado4x3();
      
      lcd.setCursor(0,0);
  
      //Imprime o tempo com o zero a esquerda
      if (min_ < 10){
        lcd.print("0");
      }
      lcd.print(min_);
      lcd.print(":");
      if (seg_ < 10){
        lcd.print("0");
      }
      lcd.print(seg_);
      
      
      beep();
      //gerenciarTimer();
      

      /*if (ligaBeep){
          tone(buzzerPIN, 10);
          if (toneStartedMillis == 0)
              toneStartedMillis = millis();
          if (toneStartedMillis+tempoBeep > millis()+startMillis){
              ligaBeep = false;
              noTone(buzzerPIN);
          }
      }*/
  
      
      //Enquanto estiver acima de 5 min, 1 bip por segundo
      /*if (min_ >= 5) {
        beepOn = 50;
        beepOff = 950;
        beep(beepOn, beepOff);
      } else 
      if (min_ >= 1 ){//Quando acima de 3 min 2 bip por segundo
        beepOn = 50;
        beepOff = 450;
        beep(beepOn, beepOff);
        beep(beepOn, beepOff);
      } else { // Abaixo de 1 min 3 bips por segundo
        beepOn = 50;
        beepOff = 240;
        beep(beepOn, beepOff);
        beep(beepOn, beepOff);
        beep(beepOn, beepOff);
      }*/
  
  
      
  }


  
  static int teclaAnt = -1;   // última tecla detectada
 
  // Lê a tensão no pino A0
  int leitura = analogRead(A0);

  // Identifica a tecla apertada pela tensão lida
  int teclaNova;
  for (teclaNova = 0; ; teclaNova++) {
    if (leitura < teclas[teclaNova].limite) {
      break;
    }
  }

  //starta a bomba quando apertar o select
  if (teclaNova == 4){
    ativada = true;  
  }
 
  // Atualiza a tela se pressionou uma nova tecla
  if (teclaNova != teclaAnt) {
    lcd.setCursor(0,1);
    lcd.print(teclas[teclaNova].nome);
    teclaAnt = teclaNova;
  }
 
}
