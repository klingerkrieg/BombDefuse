#include <LiquidCrystal.h>
#include <Keypad.h>


//TEMPO
const int MIN_ = 6;
const int SEG_ = 0;

//Penalidade fio ou codigo errado
int penalidadeMin = 1;

//fios
const int fazul     = 53;
const int fverde    = 51;
const int famarelo  = 49;
const int flaranja  = 47;
const int fvermelho = 43;
const int froxo     = 52;
const int fcinza    = 50;
const int fbranco   = 48;
const int fpreto    = 46;
const int fmarrom   = 44;

const int resetPIN  = 21;

//CÓDIGOS
// VERSAO 1
String codigosV1[] = {"111","1111","11111"};
String descricoesV1[] = {"Codigo 1","Codigo 2","Codigo 3"};
int  sequenciaFiosV1[] = {52, 51, 50, 49};
int fiosErradosV1[] = {53, 48, 49, 47};

// VERSAO 2
String codigosV2[] = {"222","2222","22222"};
String descricoesV2[] = {"Codigo 1","Codigo 2","Codigo 3"};
int  sequenciaFiosV2[] = {52, 51, 50, 49};
int fiosErradosV2[] = {49, 50, 51, 52};

// VERSAO 3
String codigosV3[] = {"333","3333","33333"};
String descricoesV3[] = {"Codigo 1","Codigo 2","Codigo 3"};
int  sequenciaFiosV3[] = {52, 51, 49, 50};
int fiosErradosV3[] = {53, 48, 49, 47};


/***
 * Versao da bomba
 */
int versaoEscolhida = 0;
int qtdVersoes = 3;


/****
 * Digitar códigos
 */

const int paginas = 3;
String codigos[] = {"","",""};
String descricoes[] = {"Codigo 1","Codigo 2","Codigo 3"};
bool codigosResolvidos[paginas];
String codigoAtual = "";
int paginaAtual = -1;

/****
 *  Retirada de Fios
 */

//Altere a sequencia para mudar a ordem de retirada dos fios
//Os valores são as portas
int  sequenciaFios[] = {52, 51, 50, 49};
const int qtdFios = 4;
int fiosErrados[] = {53, 48, 49, 47};
const int qtdFiosErrados = 4;

//Eses vetor guardará a sequencia que foi realizada
//Ele irá controlar quando o fio for desconectado
//Uma vez desconectado ele não será mais contabilizado se for conectado novamente
bool seqRealizada[qtdFios]  = {};
//Esse vetor irá controlar se errou ou acertou ou ainda nao realizou a sequencia
//-1 errou, 0 nao fez, 1 acertou
int seqPontuada[qtdFios]  = {};
// seq realizada
bool seqFiosErradosRealizados[qtdFiosErrados]  = {};

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

  pinMode(GREEN,OUTPUT);
  pinMode(RED,OUTPUT);

  digitalWrite(resetPIN, HIGH);
  pinMode(resetPIN,OUTPUT);
  

  //Inicia os vetores
  //Seta as portas como entrada
  for (int i = 0; i < qtdFios; i++){
    seqRealizada[i] = false;
    seqPontuada[i]  = 0;
    pinMode(fiosErrados[i],INPUT);  
  }
  for (int i = 0; i < qtdFiosErrados; i++){
    seqFiosErradosRealizados[i] = false;
    pinMode(sequenciaFios[i],INPUT);  
  }
  //seta os codigos resolvidos para falso
  for (int i = 0; i < paginas; i++){
    codigosResolvidos[i] = false;
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
    for (int i = 0; i < qtdFios; i++){
        if (seqRealizada[i] == false){
            proxFio = i;
            break;
        }
    }
    lcd.setCursor(0,1);
    lcd.print(" ");
    //Verifica os fios que nao devem ser desconectados
    for (int i = 0; i < qtdFiosErrados; i++){
        //Verifica o fio que foi desconectado
        if (seqFiosErradosRealizados[i] == false && !digitalRead(fiosErrados[i])){
            seqFiosErradosRealizados[i] = true;
            min_ -= penalidadeMin;
            errorBeep();
        }
        if (paginaAtual == -1){
          if (seqFiosErradosRealizados[i] == true){
              lcd.print("*");
          } else
          if (seqFiosErradosRealizados[i] == false){
              lcd.print("|");
          }
        }
    }

    
    //Imprime os fios conectados
    for (int i = 0; i < qtdFios; i++){
        //Verifica o fio que foi desconectado
        if (seqRealizada[i] == false && !digitalRead(sequenciaFios[i])){
            seqRealizada[i] = true;
            //Detecta qual fio foi removido
            if (proxFio != i){
                //Se tiver sido o fio errado
                min_ -= penalidadeMin;
                errorBeep();
                seqPontuada[i] = -1;
            } else {
                successBeep();
                //Se foi o fio certo
                seqPontuada[i] = 1;
            }
        }

        if (paginaAtual == -1){
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
            delay(5000);
            ativada = false;
            noTone(buzzerPIN);
            lcd.clear();
            lcd.print("Reiniciando...");
            delay(1000);
            digitalWrite(resetPIN,LOW);            
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

    if (paginaAtual > -1){
        //Escreve
        lcd.setCursor(0,1);
        lcd.print(descricoes[paginaAtual]);
        lcd.print(":");

        char key = keypad.getKey();
        if (key){

            //som ao digitar tecla
            tone(buzzerPIN,1000);
            delay(50);
            noTone(buzzerPIN);

            if (key == '*'){
                //insere
                if (codigos[paginaAtual] == codigoAtual){
                    successBeep();
                    codigosResolvidos[paginaAtual] = true;
                    paginaAtual = -1;
                } else {
                    min_ -= penalidadeMin;
                    errorBeep();
                }
                codigoAtual = "";
            } else
            if (key == '#'){
                codigoAtual = "";
            } else {
                codigoAtual = String(codigoAtual + key);
            }
        }

        lcd.print(codigoAtual);
        lcd.print("                ");
      
    }
  
}


bool verificaObjetivos(){
    bool desarmada = true;
    for (int i = 0; i < qtdFios; i++){
        if (seqRealizada[i] == false){
            desarmada = false;
            break;  
        }
    }
    //seta os codigos resolvidos para falso
    for (int i = 0; i < paginas; i++){
        if (codigosResolvidos[i] == false){
            desarmada = false;
            break;  
        }
    }
    return desarmada;
}


 
// Laço principal
void loop() {

  /*Serial.println("------------");
  Serial.print("ativada");
  Serial.println(ativada);
  Serial.print("min_");
  Serial.println(min_);
  Serial.print("seg_");
  Serial.println(seg_);
  Serial.print("beepStopMillis");
  Serial.println(beepStopMillis);
  Serial.print("beepStartMillis1");
  Serial.println(beepStartMillis1);
  Serial.print("beepStartMillis2");
  Serial.println(beepStartMillis2);
  Serial.print("beepStartMillis3");
  Serial.println(beepStartMillis3);
  Serial.print("ligaBeep");
  Serial.println(ligaBeep);
  Serial.print("paginaAtual");
  Serial.println(paginaAtual);*/
  


  
  int ledMillis = 0;

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
          ativada = false;
          //desarmou a bomba
          lcd.setCursor(0,1);
          lcd.print("DESARMADA       ");
      } else {
          beep();
      }
      
  }


  /***        Lê as teclas do shield ***/
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




  if (ativada == false){

    if (lastLedMillis + 1000 < millis()){
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


    //nao deixa botao ser pressionado mais de uma vez
    if (teclaNova != teclaAnt){
          //tecla pra cima
          if (teclaNova == 1){
              versaoEscolhida++;
              if (versaoEscolhida == qtdVersoes){
                  versaoEscolhida = 0;
              }
          } else
          if (teclaNova == 2){
              //para baixo
              versaoEscolhida--;
              if (versaoEscolhida < 0){
                  versaoEscolhida = qtdVersoes-1;
              }
          }
          //Imprime a versao escolhida
          lcd.setCursor(0,1);
          lcd.print(" v");
          lcd.print(versaoEscolhida);
          
          tone(buzzerPIN,1000);
          delay(50);
          noTone(buzzerPIN);
    }
    // Atualiza a tela se pressionou uma nova tecla
    if (teclaNova != teclaAnt) {
        teclaAnt = teclaNova;
    }

    
    if (teclaNova == 4){

      //seta as configuracoes da versao escolhida
      if (versaoEscolhida == 0){
          memcpy(codigos, codigosV1, sizeof(codigosV1[0])*paginas);
          memcpy(descricoes, descricoesV1, sizeof(descricoesV1[0])*qtdFios);
          memcpy(sequenciaFios, sequenciaFiosV1, sizeof(sequenciaFiosV1[0])*qtdFios);
          memcpy(fiosErrados, fiosErradosV1, sizeof(fiosErradosV1[0])*qtdFiosErrados);
      } else
      if (versaoEscolhida == 1){
          memcpy(codigos, codigosV2, sizeof(codigosV2[0])*paginas);
          memcpy(descricoes, descricoesV2, sizeof(descricoesV2[0])*qtdFios);
          memcpy(sequenciaFios, sequenciaFiosV2, sizeof(sequenciaFiosV2[0])*qtdFios);
          memcpy(fiosErrados, fiosErradosV2, sizeof(fiosErradosV2[0])*qtdFiosErrados);
      } else {
          memcpy(codigos, codigosV3, sizeof(codigosV3[0])*paginas);
          memcpy(descricoes, descricoesV3, sizeof(descricoesV3[0])*qtdFios);
          memcpy(sequenciaFios, sequenciaFiosV3, sizeof(sequenciaFiosV3[0])*qtdFios);
          memcpy(fiosErrados, fiosErradosV3, sizeof(fiosErradosV3[0])*qtdFiosErrados);
      }
      
      ativada = true;
      digitalWrite(GREEN,LOW);
      digitalWrite(RED,LOW);
    }
  } else {

      //nao deixa botao ser pressionado mais de uma vez
      if (teclaNova != teclaAnt){
          tone(buzzerPIN,1000);
          delay(50);
          noTone(buzzerPIN);
          //tecla pra cima
          if (teclaNova == 1){
              paginaAtual++;

              while (paginaAtual > -1 and codigosResolvidos[paginaAtual] == true) {
                  paginaAtual++;
                  if (paginaAtual >= paginas){
                      paginaAtual = -1;
                      break;
                  }
              }
          } else
          if (teclaNova == 2){
          //para baixo
              paginaAtual--;

              while (paginaAtual > -1 and codigosResolvidos[paginaAtual] == true) {
                  paginaAtual--;
                  if (paginaAtual < -1){
                      paginaAtual = paginas-1;
                      break;
                  }
              }
          }
      }
      // Atualiza a tela se pressionou uma nova tecla
      if (teclaNova != teclaAnt) {
          teclaAnt = teclaNova;
      }

  }
 
}
