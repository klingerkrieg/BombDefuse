#include <LiquidCrystal.h>
#include <Keypad.h>


const String codVer = "cod:1";

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

const int resetPIN  = 21;

//CÓDIGOS
// VERSAO 1
String codigosV1[] = {"14122022","2008","00001111011635","13404","6101157","54544455"};
String descricoesV1[] = {"Codigo 0","Codigo 1","Codigo 2","Codigo 3","Codigo 4", "Codigo 5"};
int  sequenciaFiosV1[] = {fvermelho, famarelo, fazul, fverde, fbranco};
int fiosErradosV1[] = {fmarrom, fpreto, flaranja, fcinza, froxo};

// VERSAO 2
String codigosV2[] = {"14122022","6245","0022","56755675","20552353","75004"};
String descricoesV2[] = {"Codigo 0","Codigo 1","Codigo 2","Codigo 3","Codigo 4", "Codigo 5"};
int  sequenciaFiosV2[] = {fbranco, fazul, froxo, fverde, fvermelho};
int fiosErradosV2[] = {fmarrom, fpreto, flaranja, fcinza, famarelo};

// VERSAO 3
String codigosV3[] = {"14122022","01469","23187","44433377766","78198555","17893"};
String descricoesV3[] = {"Codigo 0","Codigo 1","Codigo 2","Codigo 3","Codigo 4", "Codigo 5"};
int  sequenciaFiosV3[] = {famarelo, fazul, fvermelho, fverde, froxo};
int fiosErradosV3[] = {fmarrom, fpreto, fbranco, fcinza, flaranja};


/***
 * Versao da bomba
 */
int versaoEscolhida = 0;
int qtdVersoes = 3;

/**
 * Dificuldade
 */

const int difMax = 2;
int difAtual = 2;


/****
 * Digitar códigos
 */

const int paginas = 6;
String codigos[] = {"","","","","",""};
String descricoes[] = {"Codigo 0","Codigo 1","Codigo 2","Codigo 3","Codigo 4", "Codigo 5"};
bool codigosResolvidos[paginas];
String codigoAtual = "";
int paginaAtual = -1;

/****
 *  Retirada de Fios
 */
//esses dois vetores devem conter todas as cores, para poder
//emitir o som de teste na inicializacao
//se algum fio nao estiver conectado ele apita
const int qtdFios = 5;
int  sequenciaFios[] = {fpreto, fbranco, fcinza, froxo, fazul};
int fiosErrados[] = {fverde, famarelo, flaranja, fvermelho, fmarrom};
const int qtdFiosErrados = 5;


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
    
    
    //Verifica os fios que nao devem ser desconectados
    for (int i = 0; i < qtdFiosErrados; i++){
        //Verifica o fio que foi desconectado
        if (seqFiosErradosRealizados[i] == false && !digitalRead(fiosErrados[i])){
            seqFiosErradosRealizados[i] = true;
            min_ -= penalidadeMin;
            errorBeep();
        }
        //Nao imprime mais os fios que nao devem ser desconectados
        /*if (paginaAtual == -1){
          if (seqFiosErradosRealizados[i] == true){
              lcd.print("*");
          } else
          if (seqFiosErradosRealizados[i] == false){
              lcd.print("|");
          }
        }*/
    }

    if (paginaAtual == -1){
        lcd.setCursor(0,1);
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
            //imprime 'boom' no terminal
            //para o notebook ler e tocar o audio
            Serial.println("boom");
            delay(5000);
            ativada = false;
            noTone(buzzerPIN);
            lcd.clear();
            lcd.print("Reiniciando...");
            delay(1000);
            reset();
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
                    lcd.clear();
                    lcd.setCursor(0,8);
                    lcd.print("Fio");
                } else {
                    min_ -= penalidadeMin;
                    errorBeep();
                    lcd.setCursor(0,1);
                    lcd.print("              ");
                }
                codigoAtual = "";
            } else
            if (key == '#'){
                codigoAtual = "";
                lcd.setCursor(0,1);
                lcd.print("              ");
            } else {
                codigoAtual = String(codigoAtual + key);
            }

            lcd.setCursor(0,1);
            lcd.print(codigoAtual);
        }
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


void reset(){
    digitalWrite(resetPIN,LOW);
}

 
// Laço principal
void loop() {  
  int ledMillis = 0;

  
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


  //Após desarmar vai entrar nesse loop esperando o rearme
  //espera aqui para poder ler o tempo no relogio
  if (esperarRearmar){
      if (teclaAnt != teclaNova){
          if (teclaNova == 4){
              lcd.setCursor(0,1);
              lcd.print("Resetando...");
              delay(1000);
              reset();
          }
      }
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
          esperarRearmar = true;
          return;
      } else {
          beep();
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

    //vai apitar caso tenha algum fio desconectado
    verificaFiosConectadosSETUP();

    //nao deixa botao ser pressionado mais de uma vez
    if (teclaNova != teclaAnt){

          //muda a versao da bomba
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

          //muda a dificuldade com direita/esquerda
          if (teclaNova == 0){
              difAtual++;
              if (difAtual == difMax+1){
                  difAtual = 0;
              }
          } else
          if (teclaNova == 3){
              //para baixo
              difAtual--;
              if (difAtual < 0){
                  difAtual = difMax;
              }
          }

          
          //Imprime a versao escolhida
          lcd.setCursor(10,0);
          lcd.print(codVer);
          lcd.setCursor(0,1);
          lcd.print("v");
          lcd.print(versaoEscolhida+1);
          lcd.print(" diff");
          lcd.print(difAtual);
          
          tone(buzzerPIN,1000);
          delay(50);
          noTone(buzzerPIN);
    }
    // Atualiza a tela se pressionou uma nova tecla
    if (teclaNova != teclaAnt) {
        teclaAnt = teclaNova;
    }

    
    if (teclaNova == 4){

      lcd.clear();

      //seta as configuracoes da versao escolhida
      if (versaoEscolhida == 0){
          memcpy(codigos,       codigosV1,        sizeof(codigosV1[0])*paginas);
          memcpy(descricoes,    descricoesV1,     sizeof(descricoesV1));
          memcpy(sequenciaFios, sequenciaFiosV1,  sizeof(sequenciaFiosV1[0])*qtdFios);
          memcpy(fiosErrados,   fiosErradosV1,    sizeof(fiosErradosV1[0])*qtdFiosErrados);
      } else
      if (versaoEscolhida == 1){
          memcpy(codigos,       codigosV2,        sizeof(codigosV2[0])*paginas);
          memcpy(descricoes,    descricoesV2,     sizeof(descricoesV2[0])*paginas);
          memcpy(sequenciaFios, sequenciaFiosV2,  sizeof(sequenciaFiosV2[0])*qtdFios);
          memcpy(fiosErrados,   fiosErradosV2,    sizeof(fiosErradosV2[0])*qtdFiosErrados);
      } else {
          memcpy(codigos,       codigosV3,        sizeof(codigosV3[0])*paginas);
          memcpy(descricoes,    descricoesV3,     sizeof(descricoesV3[0])*paginas);
          memcpy(sequenciaFios, sequenciaFiosV3,  sizeof(sequenciaFiosV3[0])*qtdFios);
          memcpy(fiosErrados,   fiosErradosV3,    sizeof(fiosErradosV3[0])*qtdFiosErrados);
      }


      //dificuldade 2 nada muda
      if (difAtual == 1){
          //dificuldade 1 o ultimo codigo ja vem resolvido
          codigosResolvidos[paginas-1] = true;
      } else if (difAtual == 0){
          //dificuldade 0 os dois ultimos codigos vem resolvidos
          codigosResolvidos[paginas-1] = true;
          codigosResolvidos[paginas-2] = true;
      }
      
      ativada = true;
      //imprime 'planted' no terminal
      //para o notebook ler e tocar o audio
      Serial.println("planted");
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

              while (codigosResolvidos[paginaAtual] == true) {
                  paginaAtual++;
                  if (paginaAtual >= paginas){
                      paginaAtual = -1;
                      break;
                  }
              }
              if (paginaAtual >= paginas){
                  paginaAtual = -1;
              }
          } else
          if (teclaNova == 2){
          //para baixo
              paginaAtual--;

              if (paginaAtual < -1){
                  paginaAtual = paginas-1;
              }
              while (codigosResolvidos[paginaAtual] == true) {
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
          lcd.clear();

          //Escreve
          lcd.setCursor(8,0);
          if (paginaAtual == -1){
              lcd.print("Fios");
          } else {
              lcd.print(descricoes[paginaAtual]);
          }
          teclaAnt = teclaNova;
      }

  }
 
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
    for (int i = 0; i < qtdFios; i++){
        //Verifica o fio que foi desconectado
        if (!digitalRead(sequenciaFios[i])){
              imprimeCorComMalContato(sequenciaFios[i]);
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
