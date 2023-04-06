#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

//Inicializa o display no endereco 0x27
LiquidCrystal_I2C lcd(0x27,16,2);

//Versao do codigo
String codVer = "v1.0";

//TEMPO
const int MIN_ = 15;
const int SEG_ = 0;

//Penalidade fio ou codigo errado
int penalidadeMin = 1;

//fios
const int fpreto      = 53;
const int fbranco     = 51;
const int fcinza      = 49;
const int froxo       = 47;
const int fazul       = 45;

const int fverde      = 52;
const int famarelo    = 50;
const int flaranja    = 48;
const int fvermelho   = 46;
const int fmarrom     = 44;

const int buzzerPIN = 15; //DISABLED (sete o buzzer para o 15 caso queira desativar)
//const int buzzerPIN = 14; //ENABLED

//CÓDIGOS
String codigos[]      = {"1","22","333"};
const int qtdCodigos  = 3;
int  fiosCertos[]     = {fverde, famarelo, flaranja};

int fiosErrados[]     = {fvermelho, fmarrom, fpreto, fbranco, fcinza, froxo, };
const int qtdFiosCertos = 3;
const int qtdFiosErrados = 6;

bool fiosCertosRealizados[qtdFiosCertos];
bool fiosErradosRealizados[qtdFiosErrados];
bool codigosResolvidos[qtdCodigos];
String codigoAtual = "";


/****
 * TEMPO
 */

bool ativada = false;
int min_ = MIN_;
int seg_ = SEG_;



const int RED   = 37;
const int GREEN = 35;
unsigned long int lastLedMillis = 0;
bool greenRed = false;
bool esperarRearmar = false;


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

  pinMode(GREEN,OUTPUT);
  pinMode(RED,OUTPUT);

  
  //Inicia os vetores
  //Seta as portas como entrada
  for (int i = 0; i < qtdFiosErrados; i++){
    pinMode(fiosErrados[i],INPUT);  
    fiosErradosRealizados[i] = false;
  }
  for (int i = 0; i < qtdFiosCertos; i++){
    pinMode(fiosCertos[i],INPUT);  
    fiosCertosRealizados[i] = false;
  }
  //seta os codigos resolvidos para falso
  for (int i = 0; i < qtdCodigos; i++){
    codigosResolvidos[i] = false;
  }

  //Progama o pino de backlight como saída
  pinMode(buzzerPIN,OUTPUT);
  
  // Inicia o display e coloca uma mensagem
  lcd.init();
  lcd.backlight();
  lcd.print("--:--");
}


void successBeep(){
    digitalWrite(GREEN,HIGH);
    //apita 3 vezes para indicar que errou o fio
    tone(buzzerPIN, 1000);
    delay(50);
    noTone(buzzerPIN);
    delay(50);
    tone(buzzerPIN, 500);
    delay(100);
    noTone(buzzerPIN);
    delay(50);
    tone(buzzerPIN, 100);
    delay(100);
    noTone(buzzerPIN);
    delay(1000);
    digitalWrite(GREEN,LOW);
}


void errorBeep(){
    digitalWrite(RED,HIGH);
    //apita 3 vezes para indicar que errou o fio
    tone(buzzerPIN, 300);
    delay(200);
    noTone(buzzerPIN);
    delay(100);
    tone(buzzerPIN, 300);
    delay(200);
    noTone(buzzerPIN);
    delay(100);
    tone(buzzerPIN, 300);
    delay(200);
    noTone(buzzerPIN);
    delay(1000);
    digitalWrite(RED,LOW);
}


/**
 * Imprime e gerencia no visor 1 traço para cada fio ainda conectado
 * Também faz o gerenciamento da sequência de remoção dos fios
 */
void gerenciaFiosConectados(){
    int proxFio = -1;
    //Descobre qual o proximo fio que tem que ser desativado
    for (int i = 0; i < qtdFiosCertos; i++){
        if (fiosCertosRealizados[i] == false && !digitalRead(fiosCertos[i])){
            fiosCertosRealizados[i] = true;
            //quando desativer um fio correto
            //toca o som de sucesso
            successBeep();
            //imprime no terminal
            Serial.print("wire;");
            Serial.println(i);
        }
    }
    
    //Verifica os fios que nao devem ser desconectados
    for (int i = 0; i < qtdFiosErrados; i++){
        //Verifica o fio que foi desconectado
        if (fiosErradosRealizados[i] == false && !digitalRead(fiosErrados[i])){
            fiosErradosRealizados[i] = true;
            min_ -= penalidadeMin;
            errorBeep();
            Serial.print("wrong wire;");
            Serial.println(i);
        }
    }

    lcd.setCursor(8,0);
    
    //Imprime os fios conectados
    for (int i = 0; i < qtdFiosCertos; i++){
        //Verifica o fio que foi desconectado
        if (fiosCertosRealizados[i] == 0){
            lcd.print("|");
        } else
        if (fiosCertosRealizados[i] == 1){
            lcd.print(" ");
        }
    }  
}



//beep
unsigned long int beepStopMillis = 0;
unsigned long int beepStartMillis1 = 0;
unsigned long int beepStartMillis2 = 0;
unsigned long int beepStartMillis3 = 0;
bool ligaBeep = true;
int tempoBeep = 50;
int freq = 1000;
//timer
unsigned long int lastMilliSecond = 0;

//lastMilliSecond     1500
//millis_             2000
//beepStopMillis      1050

void beep(){
    unsigned long int millis_ = millis();
    
    //A cada segundo entra nesse if
    if (lastMilliSecond+1000 < millis_){
        lastMilliSecond = millis_;
  
        //beep
        tone(buzzerPIN, freq);
        beepStopMillis = millis_ + tempoBeep;        

        if (min_ < 1 && seg_ < 30) {
          tempoBeep = 75; 
          beepStartMillis1 = millis_ + 250;
          beepStartMillis2 = millis_ + 500;
          beepStartMillis3 = millis_ + 750;
        } else
        if (min_ < 1) {
          tempoBeep = 100;                  //0on 100off  350on 450off  700on 800off  
          beepStartMillis1 = millis_ + 350;
          beepStartMillis2 = millis_ + 700;
        } else
        if (min_ < 5) {
          tempoBeep = 100;                  //0on  100off    500on 600off
          beepStartMillis1 = millis_ + 500; 
        }         
        
        
  
        //Quando seg chegar em -1 volta para 59
        seg_ -= 1;
        if( seg_ == -1){
            seg_ = 59;
            min_--;
        }
    
        // Quando _min for menor que 0 a bomba explode
        if( min_ < 0){
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("00:00");
            lcd.setCursor(0,1);
            lcd.print("bye bye!");
            //imprime 'boom' no terminal
            //para o notebook ler e tocar o audio
            Serial.println("boom");
            delay(5000);
            ativada = false;
            noTone(buzzerPIN);
        }  
    } else
    if (beepStopMillis <= millis_){
        noTone(buzzerPIN);
    }

    if (beepStartMillis1 > 0 && beepStartMillis1 <= millis()){
        tone(buzzerPIN, freq);
        beepStopMillis = millis() + tempoBeep;
        beepStartMillis1 = 0;
    } else
    if (beepStartMillis2 > 0 && beepStartMillis2 <= millis()){
        tone(buzzerPIN, freq);
        beepStopMillis = millis() + tempoBeep;
        beepStartMillis2 = 0;
    } else
    if (beepStartMillis3 > 0 && beepStartMillis3 <= millis()){
        tone(buzzerPIN, freq);
        beepStopMillis = millis() + tempoBeep;
        beepStartMillis3 = 0;
    }
    
  
}



void gerenciaCodigosTeclado4x3(){
  
    char key = keypad.getKey();
    if (key){

        //som ao digitar tecla
        tone(buzzerPIN,1000);
        delay(50);
        noTone(buzzerPIN);

        if (key == '*'){
            bool codigoCorreto = false;
            //compara o codigo digitado com todos
            for (int i = 0; i < qtdCodigos; i++){

                if (codigosResolvidos[i]){
                    continue;
                }
              
                if (codigos[i] == codigoAtual){
                    codigoCorreto = true;
                    codigosResolvidos[i] = true;
                    successBeep();
                    lcd.setCursor(0,1);
                    lcd.print("Codigo aceito "); 
                    //após tocar o beep de sucesso
                    //imprime no terminal
                    Serial.print("code;");
                    Serial.println(i);
                    break;
                }
            }
            
            if (codigoCorreto == false){
                min_ -= penalidadeMin;
                errorBeep();
                lcd.setCursor(0,1);
                lcd.print("Codigo recusado ");
                Serial.println("wrong code;"+codigoAtual);
            }
            
            codigoAtual = "";
            delay(1000);
            lcd.setCursor(0,1);
            lcd.print("                    "); 
        } else
        if (key == '#'){
            codigoAtual = "";
            lcd.setCursor(0,1);
            lcd.print("                    ");
        } else {
            codigoAtual = String(codigoAtual + key);
        }

        lcd.setCursor(0,1);
        lcd.print(codigoAtual);
    }
    
  
}


bool verificaObjetivos(){
    bool desarmada = true;
    for (int i = 0; i < qtdFiosCertos; i++){
        if (fiosCertosRealizados[i] == false){
            desarmada = false;
            break;  
        }
    }
    //seta os codigos resolvidos para falso
    for (int i = 0; i < qtdCodigos; i++){
        if (codigosResolvidos[i] == false){
            desarmada = false;
            break;  
        }
    }
    return desarmada;
}



int lastTestBeep = 0;
// Laço principal
void loop() {  
  int ledMillis = 0;
  

  //Após desarmar vai entrar nesse loop esperando o rearme
  //ela sera rearmada automaticamente quando a aplicação python for reiniciada
  if (esperarRearmar){
      return;
  }

  //Quando ativada começará a contar
  if (ativada){

      //registro o tempo onde a bomba realmente começou a contar
      if (lastMilliSecond == 0){
          lastMilliSecond = millis();  
      }
  
      gerenciaFiosConectados();
      gerenciaCodigosTeclado4x3();
      
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


      if (verificaObjetivos()){
          //ativada = false;
          //desarmou a bomba
          lcd.setCursor(0,1);
          lcd.print("DESARMADA       ");
          Serial.println("defused");
          esperarRearmar = true;
          return;
      } else {
          beep();
      }
      
  }


  //Esperando ativação da bomba
  if (ativada == false){

    //pisca o led verde e vermelho para testar os leds
    if (lastLedMillis + 1000 < millis()){
        Serial.println("bomb");
        lastLedMillis = millis();
        greenRed = !greenRed;
        if (greenRed){
            digitalWrite(RED,LOW); 
            digitalWrite(GREEN,HIGH); 
        } else {
            digitalWrite(RED,HIGH); 
            digitalWrite(GREEN,LOW);
        }
    }

    //vai apitar caso tenha algum fio desconectado
    verificaFiosConectadosSETUP();

    //Imprime versao
    lcd.setCursor(12,0);
    lcd.print(codVer);
    lcd.setCursor(0,1);

    //Toca o beep de teste a cada 15 segundos
    //para confirmar que o beep está funcionando
    if (lastTestBeep + 15000 < millis()){
      tone(buzzerPIN,1000);
      delay(50);
      noTone(buzzerPIN);
      lastTestBeep = millis();
    }

    //recebe teclado
    char key = keypad.getKey();

    //inicia a bomba com a tecla *
    if (key == '*'){
        startBomb(); 
    }
  }
 
}


void startBomb(){
    lcd.clear();
    ativada = true;
    //imprime 'planted' no terminal
    //para o notebook ler e tocar o audio
    Serial.println("planted");
    digitalWrite(GREEN,LOW);
    digitalWrite(RED,LOW);  
}

/**
 * Verifica e mostra quais fios estão desconectados
 * na inicialização da bomba
 */

void verificaFiosConectadosSETUP(){
    lcd.setCursor(0,0);
    lcd.print("--:--   ");
    //Verifica os fios que nao devem ser desconectados
    for (int i = 0; i < qtdFiosErrados; i++){
        //Verifica o fio que foi desconectado
        if (!digitalRead(fiosErrados[i])){
            imprimeCorComMalContato(fiosErrados[i]);
            tone(buzzerPIN,100);
            delay(50);
            noTone(buzzerPIN);
            delay(3000);
            return;
        }
    }
    //Imprime os fios conectados
    for (int i = 0; i < qtdFiosCertos; i++){
        //Verifica o fio que foi desconectado
        if (!digitalRead(fiosCertos[i])){
              imprimeCorComMalContato(fiosCertos[i]);
              tone(buzzerPIN,100);
              delay(50);
              noTone(buzzerPIN);
              delay(3000);
              return;
        }        
    }
}


void imprimeCorComMalContato(int i){
    lcd.setCursor(0,0);
    lcd.clear();
    switch(i){
        case fpreto:
            lcd.print("preto");
            break;
        case fbranco:
            lcd.print("branco");
            break;
        case fcinza:
            lcd.print("cinza");
            break;
        case froxo:
            lcd.print("roxo");
            break;
        case fazul:
            lcd.print("azul");
            break;
        case fverde:
            lcd.print("verde");
            break;
        case famarelo:
            lcd.print("amarelo");
            break;
        case flaranja:
            lcd.print("laranja");
            break;
        case fvermelho:
            lcd.print("vermelho");
            break;
        case fmarrom:
            lcd.print("marrom");
            break;
    }
}
